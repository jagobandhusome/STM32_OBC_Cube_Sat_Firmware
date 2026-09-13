/**
 * @file    obc.h
 * @brief   CubeSat OBC product identity and firmware version.
 */
#ifndef OBC_H
#define OBC_H

#define OBC_PRODUCT_NAME          "CubeSat OBC"
#define OBC_FIRMWARE_NAME         "CubeSat OBC Flight Test"
#define OBC_FW_VERSION_MAJOR      1
#define OBC_FW_VERSION_MINOR      1
#define OBC_FW_VERSION_PATCH      0
#define OBC_FW_VERSION_STRING     "1.1.0"
#define OBC_CLI_BAUD              115200U

#define OBC_CONSOLE_TIMEOUT_MS    0U
#define OBC_UART_TEST_TIMEOUT_MS  10000U
#define OBC_LOOPBACK_DURATION_MS  10000U

#endif /* OBC_H */
