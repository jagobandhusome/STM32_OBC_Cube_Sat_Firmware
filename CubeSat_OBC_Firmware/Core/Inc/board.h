/**
 * @file    board.h
 * @brief   Board-level services: LEDs, user buttons, heartbeat.
 */
#ifndef BOARD_H
#define BOARD_H

#include "stm32h7xx_hal.h"

void Board_Init(void);
void Board_Service(void);
void Board_SetStatusLed(GPIO_PinState state);
GPIO_PinState Board_GetStatusLed(void);
int Board_IsButton1Pressed(void);
int Board_IsButton2Pressed(void);

#endif /* BOARD_H */
