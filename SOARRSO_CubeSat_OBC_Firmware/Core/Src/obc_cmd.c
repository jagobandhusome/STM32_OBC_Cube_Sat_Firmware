/**
 * @file    obc_cmd.c
 * @brief   ASCII line commands and binary A5 5A frames.
 */
#include "obc_cmd.h"
#include "obc_crc16.h"
#include "obc_hk.h"
#include "board.h"
#include "console.h"
#include "soarrso_obc.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define OBC_CMD_LINE_MAX              (80U)

typedef enum
{
    RX_IDLE = 0,
    RX_SYNC1,
    RX_OP,
    RX_LEN,
    RX_PAYLOAD,
    RX_CRC0,
    RX_CRC1
} RxState;

static RxState s_rx_state = RX_IDLE;
static uint8_t s_op;
static uint8_t s_len;
static uint8_t s_index;
static uint8_t s_payload[OBC_CMD_MAX_PAYLOAD];
static uint8_t s_crc_lo;
static char s_line[OBC_CMD_LINE_MAX];
static uint8_t s_line_len;

static void SendBinaryReply(uint8_t op, const uint8_t *payload, uint8_t length);
static void HandleOpcode(uint8_t op, const uint8_t *payload, uint8_t length);
static void HandleLine(char *line);
static int EqualsIgnoreCase(const char *a, const char *b);
static void SkipSpaces(char **cursor);

void ObcCmd_Init(void)
{
    s_rx_state = RX_IDLE;
    s_line_len = 0U;
}

static void SendBinaryReply(uint8_t op, const uint8_t *payload, uint8_t length)
{
    UART_HandleTypeDef *uart = Console_GetUart();
    uint8_t frame[8U + OBC_CMD_MAX_PAYLOAD];
    uint16_t crc;
    uint8_t i;

    if ((uart == NULL) || (length > OBC_CMD_MAX_PAYLOAD))
    {
        return;
    }

    frame[0] = OBC_CMD_SYNC0;
    frame[1] = OBC_CMD_SYNC1;
    frame[2] = (uint8_t)(op | OBC_OP_REPLY);
    frame[3] = length;
    for (i = 0U; i < length; i++)
    {
        frame[4U + i] = (payload != NULL) ? payload[i] : 0U;
    }

    crc = ObcCrc16(&frame[2], (size_t)length + 2U);
    frame[4U + length] = (uint8_t)(crc & 0xFFU);
    frame[5U + length] = (uint8_t)(crc >> 8);

    (void)HAL_UART_Transmit(uart, frame, (uint16_t)(6U + length), 1000);
}

static void HandleOpcode(uint8_t op, const uint8_t *payload, uint8_t length)
{
    uint8_t reply[sizeof(ObcHkTelemetry) + 1U];
    uint8_t status = OBC_ST_OK;

    switch (op)
    {
    case OBC_OP_PING:
        reply[0] = OBC_ST_OK;
        SendBinaryReply(op, reply, 1U);
        break;

    case OBC_OP_GET_VERSION:
        reply[0] = SOARRSO_FW_VERSION_MAJOR;
        reply[1] = SOARRSO_FW_VERSION_MINOR;
        reply[2] = SOARRSO_FW_VERSION_PATCH;
        SendBinaryReply(op, reply, 3U);
        break;

    case OBC_OP_GET_HK:
    {
        const ObcHkTelemetry *hk;
        ObcHk_Refresh();
        hk = ObcHk_GetLatest();
        memcpy(reply, hk, sizeof(*hk));
        SendBinaryReply(op, reply, (uint8_t)sizeof(*hk));
        break;
    }

    case OBC_OP_SET_HK_PERIOD:
        if (length < 2U)
        {
            status = OBC_ST_BAD_ARG;
        }
        else
        {
            const uint32_t period = (uint32_t)payload[0] | ((uint32_t)payload[1] << 8);
            ObcHk_SetPeriodMs(period);
        }
        reply[0] = status;
        SendBinaryReply(op, reply, 1U);
        break;

    case OBC_OP_SET_MODE:
        if ((length < 1U) || (payload[0] > (uint8_t)OBC_MODE_TEST))
        {
            status = OBC_ST_BAD_ARG;
        }
        else
        {
            ObcHk_SetMode((ObcMode)payload[0]);
        }
        reply[0] = status;
        SendBinaryReply(op, reply, 1U);
        break;

    case OBC_OP_SET_LED:
        if (length < 1U)
        {
            status = OBC_ST_BAD_ARG;
        }
        else
        {
            Board_SetStatusLed((payload[0] != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        reply[0] = status;
        SendBinaryReply(op, reply, 1U);
        break;

    case OBC_OP_HWTEST:
        if (ObcHk_GetMode() == OBC_MODE_SAFE)
        {
            status = OBC_ST_DENIED;
            reply[0] = status;
            SendBinaryReply(op, reply, 1U);
            break;
        }
        if (length < 1U)
        {
            status = OBC_ST_BAD_ARG;
            reply[0] = status;
            SendBinaryReply(op, reply, 1U);
            break;
        }
        Console_DispatchLetter((char)payload[0]);
        reply[0] = OBC_ST_OK;
        SendBinaryReply(op, reply, 1U);
        break;

    default:
        status = OBC_ST_BAD_OP;
        reply[0] = status;
        SendBinaryReply(op, reply, 1U);
        break;
    }

    ObcHk_NoteCommand(op, status);
}

static int EqualsIgnoreCase(const char *a, const char *b)
{
    while ((*a != '\0') && (*b != '\0'))
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0') && (*b == '\0');
}

static void SkipSpaces(char **cursor)
{
    while ((**cursor == ' ') || (**cursor == '\t'))
    {
        (*cursor)++;
    }
}

static void HandleLine(char *line)
{
    char *cursor = line;
    char *verb;
    uint8_t status = OBC_ST_OK;

    SkipSpaces(&cursor);
    if (*cursor == '\0')
    {
        return;
    }

    verb = cursor;
    while ((*cursor != '\0') && (*cursor != ' ') && (*cursor != '\t'))
    {
        cursor++;
    }
    if (*cursor != '\0')
    {
        *cursor = '\0';
        cursor++;
        SkipSpaces(&cursor);
    }

    if (EqualsIgnoreCase(verb, "ping"))
    {
        Console_Print("PONG\r\n");
        ObcHk_NoteCommand(OBC_OP_PING, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "ver") || EqualsIgnoreCase(verb, "version"))
    {
        Console_Printf("%s firmware %s\r\n", SOARRSO_PRODUCT_NAME, SOARRSO_FW_VERSION_STRING);
        ObcHk_NoteCommand(OBC_OP_GET_VERSION, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "hk"))
    {
        ObcHk_Refresh();
        ObcHk_Print();
        ObcHk_NoteCommand(OBC_OP_GET_HK, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "hkperiod"))
    {
        if (*cursor == '\0')
        {
            Console_Printf("hk period %lu ms\r\n", (unsigned long)ObcHk_GetPeriodMs());
        }
        else
        {
            ObcHk_SetPeriodMs((uint32_t)strtoul(cursor, NULL, 10));
            Console_Printf("hk period set %lu ms\r\n", (unsigned long)ObcHk_GetPeriodMs());
        }
        ObcHk_NoteCommand(OBC_OP_SET_HK_PERIOD, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "mode"))
    {
        if (EqualsIgnoreCase(cursor, "safe"))
        {
            ObcHk_SetMode(OBC_MODE_SAFE);
        }
        else if (EqualsIgnoreCase(cursor, "nominal"))
        {
            ObcHk_SetMode(OBC_MODE_NOMINAL);
        }
        else if (EqualsIgnoreCase(cursor, "test"))
        {
            ObcHk_SetMode(OBC_MODE_TEST);
        }
        else if (*cursor != '\0')
        {
            Console_Print("usage: mode safe|nominal|test\r\n");
            ObcHk_NoteCommand(OBC_OP_SET_MODE, OBC_ST_BAD_ARG);
            return;
        }
        Console_Printf("mode %u\r\n", (unsigned int)ObcHk_GetMode());
        ObcHk_NoteCommand(OBC_OP_SET_MODE, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "led"))
    {
        if (EqualsIgnoreCase(cursor, "on"))
        {
            Board_SetStatusLed(GPIO_PIN_SET);
        }
        else if (EqualsIgnoreCase(cursor, "off"))
        {
            Board_SetStatusLed(GPIO_PIN_RESET);
        }
        else
        {
            Console_Print("usage: led on|off\r\n");
            ObcHk_NoteCommand(OBC_OP_SET_LED, OBC_ST_BAD_ARG);
            return;
        }
        ObcHk_NoteCommand(OBC_OP_SET_LED, OBC_ST_OK);
        return;
    }
    if (EqualsIgnoreCase(verb, "help") || EqualsIgnoreCase(verb, "?"))
    {
        Console_PrintHelp();
        return;
    }

    if ((verb[0] != '\0') && (verb[1] == '\0'))
    {
        char letter = verb[0];
        if ((letter >= 'a') && (letter <= 'z'))
        {
            letter = (char)(letter - (char)32);
        }

        if (ObcHk_GetMode() == OBC_MODE_SAFE)
        {
            Console_Print("SAFE mode: hardware tests denied. Use 'mode test' or 'mode nominal'.\r\n");
            ObcHk_NoteCommand(OBC_OP_HWTEST, OBC_ST_DENIED);
            return;
        }

        Console_DispatchLetter(letter);
        ObcHk_NoteCommand(OBC_OP_HWTEST, status);
        return;
    }

    Console_Printf("unknown command: %s\r\n", verb);
    ObcHk_NoteCommand(0xFFU, OBC_ST_BAD_OP);
}

static void FeedByte(uint8_t byte)
{
    switch (s_rx_state)
    {
    case RX_IDLE:
        if (byte == OBC_CMD_SYNC0)
        {
            s_rx_state = RX_SYNC1;
        }
        else if ((byte == '\r') || (byte == '\n'))
        {
            if (s_line_len > 0U)
            {
                s_line[s_line_len] = '\0';
                HandleLine(s_line);
                s_line_len = 0U;
            }
        }
        else if (byte == 0x08U)
        {
            if (s_line_len > 0U)
            {
                s_line_len--;
            }
        }
        else if ((s_line_len + 1U) < OBC_CMD_LINE_MAX)
        {
            s_line[s_line_len++] = (char)byte;
        }
        break;

    case RX_SYNC1:
        s_rx_state = (byte == OBC_CMD_SYNC1) ? RX_OP : RX_IDLE;
        break;

    case RX_OP:
        s_op = byte;
        s_rx_state = RX_LEN;
        break;

    case RX_LEN:
        s_len = byte;
        s_index = 0U;
        s_rx_state = (s_len == 0U) ? RX_CRC0 : RX_PAYLOAD;
        if (s_len > OBC_CMD_MAX_PAYLOAD)
        {
            s_rx_state = RX_IDLE;
        }
        break;

    case RX_PAYLOAD:
        s_payload[s_index++] = byte;
        if (s_index >= s_len)
        {
            s_rx_state = RX_CRC0;
        }
        break;

    case RX_CRC0:
        s_crc_lo = byte;
        s_rx_state = RX_CRC1;
        break;

    case RX_CRC1:
    {
        uint8_t check[2U + OBC_CMD_MAX_PAYLOAD];
        uint16_t expected;
        uint16_t actual;
        uint8_t i;

        check[0] = s_op;
        check[1] = s_len;
        for (i = 0U; i < s_len; i++)
        {
            check[2U + i] = s_payload[i];
        }
        expected = ObcCrc16(check, (size_t)s_len + 2U);
        actual = (uint16_t)s_crc_lo | ((uint16_t)byte << 8);
        s_rx_state = RX_IDLE;

        if (expected != actual)
        {
            uint8_t nack = OBC_ST_BAD_CRC;
            SendBinaryReply(s_op, &nack, 1U);
            ObcHk_NoteCommand(s_op, OBC_ST_BAD_CRC);
        }
        else
        {
            HandleOpcode(s_op, s_payload, s_len);
        }
        break;
    }

    default:
        s_rx_state = RX_IDLE;
        break;
    }
}

void ObcCmd_Service(void)
{
    UART_HandleTypeDef *uart = Console_GetUart();
    uint8_t byte = 0U;

    if (uart == NULL)
    {
        return;
    }

    while (HAL_UART_Receive(uart, &byte, 1U, 0U) == HAL_OK)
    {
        FeedByte(byte);
    }
}
