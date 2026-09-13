/**
 * @file    console.c
 * @brief   USART3 command console for the CubeSat OBC.
 */
#include "console.h"
#include "obc.h"
#include "board_pins.h"
#include "hw_tests.h"
#include "ext_memory.h"
#include "can_app.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;

static UART_HandleTypeDef *s_console_uart = NULL;

void Console_Init(UART_HandleTypeDef *huart)
{
    s_console_uart = huart;
}

UART_HandleTypeDef *Console_GetUart(void)
{
    return s_console_uart;
}

void Console_Print(const char *text)
{
    if ((s_console_uart == NULL) || (text == NULL))
    {
        return;
    }

    const size_t length = strlen(text);
    if (length > 0U)
    {
        (void)HAL_UART_Transmit(s_console_uart, (uint8_t *)text, (uint16_t)length, 1000);
    }
}

void Console_Printf(const char *fmt, ...)
{
    char buffer[160];
    va_list args;

    va_start(args, fmt);
    (void)vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Console_Print(buffer);
}

void Console_PrintBanner(void)
{
    Console_Print("\r\n\r\n");
    Console_Print("  ============================================================\r\n");
    Console_Print("   CubeSat OBC\r\n");
    Console_Print("   On-Board Computer  |  STM32H753ZI  |  FW " OBC_FW_VERSION_STRING "\r\n");
    Console_Print("  ============================================================\r\n");
    Console_PrintHelp();
}

void Console_PrintHelp(void)
{
    Console_Print("\r\n  Flight commands (type the word, then Enter):\r\n");
    Console_Print("    ping            Link check, replies PONG\r\n");
    Console_Print("    ver             Firmware version\r\n");
    Console_Print("    hk              Print latest housekeeping\r\n");
    Console_Print("    hkperiod [ms]   Get or set HK period\r\n");
    Console_Print("    mode safe|nominal|test\r\n");
    Console_Print("    led on|off      Status LED (LED3)\r\n");
    Console_Print("    help            This menu\r\n");
    Console_Print("\r\n  Hardware tests (one letter, then Enter; blocked in SAFE):\r\n");
    Console_Print("    A  External SRAM chip-select test\r\n");
    Console_Print("    B  External SRAM write pattern\r\n");
    Console_Print("    C  External SRAM read / verify\r\n");
    Console_Print("    D  I2C1 pin toggle\r\n");
    Console_Print("    E  I2C2 pin toggle\r\n");
    Console_Print("    F  I2C4 pin toggle\r\n");
    Console_Print("    H  UART4 echo test\r\n");
    Console_Print("    I  UART2 echo test\r\n");
    Console_Print("    J  GPIO sweep\r\n");
    Console_Print("    K  Analogue pin sweep\r\n");
    Console_Print("    L  PPS 1 Hz pulse train\r\n");
    Console_Print("    M  LVDS DR -> MOSI loopback\r\n");
    Console_Print("    N  LVDS DR -> NSS loopback\r\n");
    Console_Print("    O  LVDS DR -> SCK loopback\r\n");
    Console_Print("    P  LVDS EXTI9 -> MOSI loopback\r\n");
    Console_Print("    Q  LVDS EXTI9 -> NSS loopback\r\n");
    Console_Print("    R  LVDS EXTI9 -> SCK loopback\r\n");
    Console_Print("    S  CAN1 loopback (10 s)\r\n");
    Console_Print("    T  CAN2 loopback (10 s)\r\n");
    Console_Print("    V  Firmware version\r\n");
    Console_Print("\r\n  Binary frames: A5 5A | op | len | payload | crc16_le\r\n\r\n");
}

void Console_DispatchLetter(char command)
{
    switch (command)
    {
    case 'A':
        Console_Print("Starting external SRAM chip-select test\r\n");
        ExtMemory_ChipSelectTest();
        break;
    case 'B':
        Console_Print("Starting external SRAM write test\r\n");
        ExtMemory_WriteTest();
        break;
    case 'C':
        Console_Print("Starting external SRAM read test\r\n");
        ExtMemory_ReadTest();
        break;
    case 'D':
        HwTest_I2cBus(I2C1_SCL_GPIO_PORT, I2C1_SCL_PIN, I2C1_SDA_GPIO_PORT, I2C1_SDA_PIN, "I2C1");
        break;
    case 'E':
        HwTest_I2cBus(I2C2_SCL_GPIO_PORT, I2C2_SCL_PIN, I2C2_SDA_GPIO_PORT, I2C2_SDA_PIN, "I2C2");
        break;
    case 'F':
        HwTest_I2cBus(I2C4_SCL_GPIO_PORT, I2C4_SCL_PIN, I2C4_SDA_GPIO_PORT, I2C4_SDA_PIN, "I2C4");
        break;
    case 'H':
        HwTest_UartEcho(&huart4, "UART4");
        break;
    case 'I':
        HwTest_UartEcho(&huart2, "UART2");
        break;
    case 'J':
        HwTest_GpioSweep();
        break;
    case 'K':
        HwTest_AnaloguePins();
        break;
    case 'L':
        HwTest_Pps();
        break;
    case 'M':
        HwTest_LvdsDrToMosi();
        break;
    case 'N':
        HwTest_LvdsDrToNss();
        break;
    case 'O':
        HwTest_LvdsDrToSck();
        break;
    case 'P':
        HwTest_LvdsExti9ToMosi();
        break;
    case 'Q':
        HwTest_LvdsExti9ToNss();
        break;
    case 'R':
        HwTest_LvdsExti9ToSck();
        break;
    case 'S':
        CanApp_LoopbackTest(0U);
        break;
    case 'T':
        CanApp_LoopbackTest(1U);
        break;
    case 'V':
        Console_Printf("%s firmware %s\r\n", OBC_PRODUCT_NAME, OBC_FW_VERSION_STRING);
        break;
    case '?':
        Console_PrintHelp();
        break;
    default:
        break;
    }
}
