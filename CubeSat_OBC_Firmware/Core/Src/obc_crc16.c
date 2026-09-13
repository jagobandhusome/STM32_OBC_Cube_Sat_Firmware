/**
 * @file    obc_crc16.c
 * @brief   CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF).
 */
#include "obc_crc16.h"

uint16_t ObcCrc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFFU;
    size_t i;
    uint8_t bit;

    if (data == NULL)
    {
        return crc;
    }

    for (i = 0U; i < length; i++)
    {
        crc ^= (uint16_t)data[i] << 8;
        for (bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)((crc << 1) ^ 0x1021U);
            }
            else
            {
                crc = (uint16_t)(crc << 1);
            }
        }
    }

    return crc;
}
