/**
 * @file    obc_hk.h
 * @brief   Flight-style housekeeping for the SOARRSO CubeSat OBC.
 */
#ifndef OBC_HK_H
#define OBC_HK_H

#include <stdint.h>

typedef enum
{
    OBC_MODE_SAFE = 0,
    OBC_MODE_NOMINAL = 1,
    OBC_MODE_TEST = 2
} ObcMode;

typedef struct
{
    uint32_t sequence;
    uint32_t uptime_s;
    uint32_t boot_count;
    uint32_t reset_flags;
    uint8_t mode;
    uint8_t button1_pressed;
    uint8_t button2_pressed;
    uint8_t status_led;
    uint8_t last_opcode;
    uint8_t last_status;
    uint8_t rtc_year;
    uint8_t rtc_month;
    uint8_t rtc_day;
    uint8_t rtc_hour;
    uint8_t rtc_minute;
    uint8_t rtc_second;
} __attribute__((packed)) ObcHkTelemetry;

void ObcHk_Init(void);
void ObcHk_Service(void);
void ObcHk_Refresh(void);
void ObcHk_Collect(ObcHkTelemetry *out);
const ObcHkTelemetry *ObcHk_GetLatest(void);
void ObcHk_SetMode(ObcMode mode);
ObcMode ObcHk_GetMode(void);
void ObcHk_SetPeriodMs(uint32_t period_ms);
uint32_t ObcHk_GetPeriodMs(void);
void ObcHk_NoteCommand(uint8_t opcode, uint8_t status);
void ObcHk_Print(void);

#endif /* OBC_HK_H */
