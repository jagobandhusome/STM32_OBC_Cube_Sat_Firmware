/**
 * @file    board_pins.h
 * @brief   Named GPIO map for the CubeSat OBC development board.
 */
#ifndef BOARD_PINS_H
#define BOARD_PINS_H

#include "main.h"

#define LED1_GPIO_PORT                GPIOG
#define LED1_PIN                      GPIO_PIN_6
#define LED2_GPIO_PORT                GPIOG
#define LED2_PIN                      GPIO_PIN_7
#define LED3_GPIO_PORT                GPIOG
#define LED3_PIN                      GPIO_PIN_8

#define USER_BUTTON1_GPIO_PORT        GPIOC
#define USER_BUTTON1_PIN              GPIO_PIN_6
#define USER_BUTTON2_GPIO_PORT        GPIOC
#define USER_BUTTON2_PIN              GPIO_PIN_4

#define PPS_GPIO_PORT                 GPIOC
#define PPS_PIN                       GPIO_PIN_13

#define I2C1_SCL_GPIO_PORT            GPIOB
#define I2C1_SCL_PIN                  GPIO_PIN_6
#define I2C1_SDA_GPIO_PORT            GPIOB
#define I2C1_SDA_PIN                  GPIO_PIN_7

#define I2C2_SCL_GPIO_PORT            GPIOB
#define I2C2_SCL_PIN                  GPIO_PIN_10
#define I2C2_SDA_GPIO_PORT            GPIOB
#define I2C2_SDA_PIN                  GPIO_PIN_11

#define I2C4_SCL_GPIO_PORT            GPIOB
#define I2C4_SCL_PIN                  GPIO_PIN_8
#define I2C4_SDA_GPIO_PORT            GPIOB
#define I2C4_SDA_PIN                  GPIO_PIN_9

#define LVDS_DR_GPIO_PORT             GPIOG
#define LVDS_DR_PIN                   GPIO_PIN_15
#define LVDS_EXTI9_GPIO_PORT          GPIOG
#define LVDS_EXTI9_PIN                GPIO_PIN_9
#define LVDS_MOSI_GPIO_PORT           GPIOD
#define LVDS_MOSI_PIN                 GPIO_PIN_7
#define LVDS_NSS_GPIO_PORT            GPIOG
#define LVDS_NSS_PIN                  GPIO_PIN_10
#define LVDS_SCK_GPIO_PORT            GPIOG
#define LVDS_SCK_PIN                  GPIO_PIN_11

#define EXT_SRAM_BASE_ADDRESS         (0xC0000000UL)
#define EXT_SRAM_SIZE_BYTES           (0x00800000UL)
#define EXT_SRAM_CHIP_SIZE_BYTES      (0x00400000UL)
#define EXT_SRAM_CHIP_COUNT           (2U)

#define FMC_BCR1_REG                  ((volatile uint32_t *)0x52004000UL)
#define FMC_BTR1_REG                  ((volatile uint32_t *)0x52004004UL)
#define FMC_BCR1_SRAM_ASYNC           (0x81201091UL)
#define FMC_BTR1_SRAM_ASYNC           (0x00000202UL)

#endif /* BOARD_PINS_H */
