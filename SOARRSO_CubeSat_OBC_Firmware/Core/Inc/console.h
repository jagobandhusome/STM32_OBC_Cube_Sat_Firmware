/**
 * @file    console.h
 * @brief   USART3 command console for the SOARRSO CubeSat OBC.
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include "stm32h7xx_hal.h"
#include <stddef.h>

void Console_Init(UART_HandleTypeDef *huart);
void Console_Print(const char *text);
void Console_Printf(const char *fmt, ...);
void Console_PrintBanner(void);
void Console_PrintHelp(void);
void Console_DispatchLetter(char command);

UART_HandleTypeDef *Console_GetUart(void);

#endif /* CONSOLE_H */
