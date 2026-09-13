/**
 * @file    board.c
 * @brief   Board-level LED heartbeat and user-button handling.
 */
#include "board.h"
#include "board_pins.h"

void Board_Init(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET);
}

void Board_SetStatusLed(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, state);
}

GPIO_PinState Board_GetStatusLed(void)
{
    return HAL_GPIO_ReadPin(LED3_GPIO_PORT, LED3_PIN);
}

int Board_IsButton1Pressed(void)
{
    return (HAL_GPIO_ReadPin(USER_BUTTON1_GPIO_PORT, USER_BUTTON1_PIN) == GPIO_PIN_RESET) ? 1 : 0;
}

int Board_IsButton2Pressed(void)
{
    return (HAL_GPIO_ReadPin(USER_BUTTON2_GPIO_PORT, USER_BUTTON2_PIN) == GPIO_PIN_RESET) ? 1 : 0;
}

void Board_Service(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);

    if (HAL_GPIO_ReadPin(USER_BUTTON2_GPIO_PORT, USER_BUTTON2_PIN) == GPIO_PIN_RESET)
    {
        Board_SetStatusLed(GPIO_PIN_SET);
    }

    if (HAL_GPIO_ReadPin(USER_BUTTON1_GPIO_PORT, USER_BUTTON1_PIN) == GPIO_PIN_RESET)
    {
        Board_SetStatusLed(GPIO_PIN_RESET);
    }
}
