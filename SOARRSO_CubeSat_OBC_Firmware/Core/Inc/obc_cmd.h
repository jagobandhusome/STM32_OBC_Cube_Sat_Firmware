/**
 * @file    obc_cmd.h
 * @brief   Command protocol for the SOARRSO CubeSat OBC.
 *
 * Human console: type a command and press Enter (hk, ping, ver, A, ?).
 * Machine frame:  A5 5A | op | len | payload[len] | crc16_le
 * CRC-16/CCITT-FALSE is computed over op, len, and payload.
 */
#ifndef OBC_CMD_H
#define OBC_CMD_H

#include <stdint.h>

#define OBC_CMD_SYNC0                 (0xA5U)
#define OBC_CMD_SYNC1                 (0x5AU)
#define OBC_CMD_MAX_PAYLOAD           (64U)

#define OBC_OP_PING                   (0x01U)
#define OBC_OP_GET_VERSION            (0x02U)
#define OBC_OP_GET_HK                 (0x03U)
#define OBC_OP_SET_HK_PERIOD          (0x04U)
#define OBC_OP_SET_MODE               (0x05U)
#define OBC_OP_SET_LED                (0x06U)
#define OBC_OP_HWTEST                 (0x10U)
#define OBC_OP_REPLY                  (0x80U)

#define OBC_ST_OK                     (0x00U)
#define OBC_ST_BAD_CRC                (0x01U)
#define OBC_ST_BAD_OP                 (0x02U)
#define OBC_ST_BAD_ARG                (0x03U)
#define OBC_ST_DENIED                 (0x04U)

void ObcCmd_Init(void);
void ObcCmd_Service(void);

#endif /* OBC_CMD_H */
