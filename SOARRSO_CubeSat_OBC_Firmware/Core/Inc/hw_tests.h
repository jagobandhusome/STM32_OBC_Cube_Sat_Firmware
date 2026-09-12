/**
 * @file    hw_tests.h
 * @brief   Peripheral bring-up tests for the SOARRSO CubeSat OBC board.
 */
#ifndef HW_TESTS_H
#define HW_TESTS_H

#include "stm32h7xx_hal.h"

void HwTest_UartEcho(UART_HandleTypeDef *huart, const char *port_name);
void HwTest_GpioSweep(void);
void HwTest_AnaloguePins(void);
void HwTest_Pps(void);
void HwTest_I2cBus(GPIO_TypeDef *scl_port, uint16_t scl_pin,
                   GPIO_TypeDef *sda_port, uint16_t sda_pin,
                   const char *bus_name);
void HwTest_LvdsDrToMosi(void);
void HwTest_LvdsDrToNss(void);
void HwTest_LvdsDrToSck(void);
void HwTest_LvdsExti9ToMosi(void);
void HwTest_LvdsExti9ToNss(void);
void HwTest_LvdsExti9ToSck(void);

#endif /* HW_TESTS_H */
