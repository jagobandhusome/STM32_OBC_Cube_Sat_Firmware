/**
 * @file    hw_tests.c
 * @brief   Peripheral bring-up tests for the SOARRSO CubeSat OBC board.
 */
#include "hw_tests.h"
#include "board_pins.h"
#include "console.h"
#include "soarrso_obc.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} PinMap;

static const PinMap s_gpio_map[] =
{
    {GPIOB, GPIO_PIN_5},
    {GPIOG, GPIO_PIN_14},
    {GPIOF, GPIO_PIN_10},
    {GPIOF, GPIO_PIN_8},
    {GPIOF, GPIO_PIN_7},
    {GPIOF, GPIO_PIN_6},
    {GPIOA, GPIO_PIN_8},
    {GPIOC, GPIO_PIN_9},
    {GPIOB, GPIO_PIN_14},
    {GPIOB, GPIO_PIN_15},
    {GPIOG, GPIO_PIN_12},
    {GPIOD, GPIO_PIN_6},
    {GPIOD, GPIO_PIN_3},
    {GPIOD, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_12},
    {GPIOA, GPIO_PIN_10},
    {GPIOA, GPIO_PIN_9},
    {GPIOC, GPIO_PIN_8},
    {GPIOB, GPIO_PIN_2},
    {GPIOA, GPIO_PIN_5},
    {GPIOG, GPIO_PIN_9},
    {GPIOD, GPIO_PIN_7},
    {GPIOG, GPIO_PIN_11},
    {GPIOG, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_9},
    {GPIOB, GPIO_PIN_8},
    {GPIOG, GPIO_PIN_15},
    {GPIOB, GPIO_PIN_11},
    {GPIOB, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_6}
};

static const PinMap s_analogue_map[] =
{
    {GPIOF, GPIO_PIN_11},
    {GPIOA, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_1},
    {GPIOC, GPIO_PIN_5},
    {GPIOC, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_3},
    {GPIOC, GPIO_PIN_0},
    {GPIOC, GPIO_PIN_1}
};

static void LoopbackIo(GPIO_TypeDef *input_port, uint16_t input_pin,
                       GPIO_TypeDef *output_port, uint16_t output_pin,
                       const char *name)
{
    const uint32_t start = HAL_GetTick();
    const uint32_t end = start + SOARRSO_LOOPBACK_DURATION_MS;

    Console_Printf("Starting %s loopback for %u ms\r\n", name, SOARRSO_LOOPBACK_DURATION_MS);

    while (HAL_GetTick() < end)
    {
        const GPIO_PinState state = HAL_GPIO_ReadPin(input_port, input_pin);
        HAL_GPIO_WritePin(output_port, output_pin, state);
    }

    Console_Print("Loopback complete\r\n");
}

void HwTest_UartEcho(UART_HandleTypeDef *huart, const char *port_name)
{
    uint8_t dummy = 0U;
    uint8_t received = 0U;
    HAL_StatusTypeDef status;

    Console_Printf("Starting %s echo test, waiting for one character\r\n", port_name);

    (void)HAL_UART_Receive(huart, &dummy, 1U, 0U);
    status = HAL_UART_Receive(huart, &received, 1U, SOARRSO_UART_TEST_TIMEOUT_MS);

    if (status == HAL_TIMEOUT)
    {
        Console_Printf("%s: nothing received\r\n", port_name);
        return;
    }

    Console_Printf("%s: received 0x%02X, echoing\r\n", port_name, received);
    (void)HAL_UART_Transmit(huart, &received, 1U, 1000);
}

void HwTest_GpioSweep(void)
{
    unsigned int index;

    Console_Print("GPIO sweep: pulsing each mapped output\r\n");

    for (index = 0U; index < (sizeof(s_gpio_map) / sizeof(s_gpio_map[0])); index++)
    {
        Console_Printf("GPIO %02u high\r\n", index);
        HAL_GPIO_WritePin(s_gpio_map[index].port, s_gpio_map[index].pin, GPIO_PIN_SET);
        HAL_Delay(25);

        Console_Printf("GPIO %02u low\r\n", index);
        HAL_GPIO_WritePin(s_gpio_map[index].port, s_gpio_map[index].pin, GPIO_PIN_RESET);
        HAL_Delay(25);
    }

    Console_Print("GPIO sweep complete\r\n");
}

void HwTest_AnaloguePins(void)
{
    unsigned int index;

    Console_Print("Analogue pin sweep\r\n");

    for (index = 0U; index < (sizeof(s_analogue_map) / sizeof(s_analogue_map[0])); index++)
    {
        Console_Printf("Analogue pin %u high\r\n", index);
        HAL_GPIO_WritePin(s_analogue_map[index].port, s_analogue_map[index].pin, GPIO_PIN_SET);
        HAL_Delay(250);

        Console_Printf("Analogue pin %u low\r\n", index);
        HAL_GPIO_WritePin(s_analogue_map[index].port, s_analogue_map[index].pin, GPIO_PIN_RESET);
        HAL_Delay(250);
    }

    Console_Print("Analogue pin sweep complete\r\n");
}

void HwTest_Pps(void)
{
    unsigned int pulse;

    Console_Print("PPS test: 10 pulses at 1 Hz\r\n");

    for (pulse = 0U; pulse < 10U; pulse++)
    {
        HAL_GPIO_WritePin(PPS_GPIO_PORT, PPS_PIN, GPIO_PIN_SET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(PPS_GPIO_PORT, PPS_PIN, GPIO_PIN_RESET);
        HAL_Delay(500);
    }

    Console_Print("PPS test complete\r\n");
}

void HwTest_I2cBus(GPIO_TypeDef *scl_port, uint16_t scl_pin,
                   GPIO_TypeDef *sda_port, uint16_t sda_pin,
                   const char *bus_name)
{
    Console_Printf("%s: clock and data low\r\n", bus_name);
    HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET);
    HAL_Delay(250);

    Console_Printf("%s: clock and data high\r\n", bus_name);
    HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_SET);
    HAL_Delay(250);

    Console_Printf("%s: clock and data low\r\n", bus_name);
    HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET);
}

void HwTest_LvdsDrToMosi(void)
{
    LoopbackIo(LVDS_DR_GPIO_PORT, LVDS_DR_PIN, LVDS_MOSI_GPIO_PORT, LVDS_MOSI_PIN, "LVDS DR->MOSI");
}

void HwTest_LvdsDrToNss(void)
{
    LoopbackIo(LVDS_DR_GPIO_PORT, LVDS_DR_PIN, LVDS_NSS_GPIO_PORT, LVDS_NSS_PIN, "LVDS DR->NSS");
}

void HwTest_LvdsDrToSck(void)
{
    LoopbackIo(LVDS_DR_GPIO_PORT, LVDS_DR_PIN, LVDS_SCK_GPIO_PORT, LVDS_SCK_PIN, "LVDS DR->SCK");
}

void HwTest_LvdsExti9ToMosi(void)
{
    LoopbackIo(LVDS_EXTI9_GPIO_PORT, LVDS_EXTI9_PIN, LVDS_MOSI_GPIO_PORT, LVDS_MOSI_PIN, "LVDS EXTI9->MOSI");
}

void HwTest_LvdsExti9ToNss(void)
{
    LoopbackIo(LVDS_EXTI9_GPIO_PORT, LVDS_EXTI9_PIN, LVDS_NSS_GPIO_PORT, LVDS_NSS_PIN, "LVDS EXTI9->NSS");
}

void HwTest_LvdsExti9ToSck(void)
{
    LoopbackIo(LVDS_EXTI9_GPIO_PORT, LVDS_EXTI9_PIN, LVDS_SCK_GPIO_PORT, LVDS_SCK_PIN, "LVDS EXTI9->SCK");
}
