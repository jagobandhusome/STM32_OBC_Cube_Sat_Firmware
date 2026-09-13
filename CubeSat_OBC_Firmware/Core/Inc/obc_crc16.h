/**
 * @file    obc_crc16.h
 * @brief   CRC-16/CCITT-FALSE for CubeSat OBC command frames.
 */
#ifndef OBC_CRC16_H
#define OBC_CRC16_H

#include <stddef.h>
#include <stdint.h>

uint16_t ObcCrc16(const uint8_t *data, size_t length);

#endif /* OBC_CRC16_H */
