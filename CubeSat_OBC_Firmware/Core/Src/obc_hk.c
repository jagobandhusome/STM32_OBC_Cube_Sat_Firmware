/**
 * @file    obc_hk.c
 * @brief   Housekeeping collection and boot-count tracking.
 */
#include "obc_hk.h"
#include "obc_cmd.h"
#include "board.h"
#include "console.h"
#include "obc.h"

#include "stm32h7xx_hal.h"

#define OBC_HK_BOOT_MAGIC             (0x534F4152UL) /* 'SOAR' */
#define OBC_HK_DEFAULT_PERIOD_MS      (1000U)

extern RTC_HandleTypeDef hrtc;

static ObcHkTelemetry s_latest;
static ObcMode s_mode = OBC_MODE_NOMINAL;
static uint32_t s_period_ms = OBC_HK_DEFAULT_PERIOD_MS;
static uint32_t s_last_collect_ms;
static uint8_t s_last_opcode;
static uint8_t s_last_status;

static uint32_t ReadResetFlags(void)
{
    const uint32_t flags = RCC->RSR;
    __HAL_RCC_CLEAR_RESET_FLAGS();
    return flags;
}

static uint32_t NextBootCount(void)
{
    uint32_t magic;
    uint32_t count;

    magic = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
    count = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);

    if (magic != OBC_HK_BOOT_MAGIC)
    {
        count = 0U;
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, OBC_HK_BOOT_MAGIC);
    }

    count++;
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, count);
    return count;
}

void ObcHk_Init(void)
{
    s_mode = OBC_MODE_NOMINAL;
    s_period_ms = OBC_HK_DEFAULT_PERIOD_MS;
    s_last_collect_ms = HAL_GetTick();
    s_last_opcode = 0U;
    s_last_status = OBC_ST_OK;
    s_latest.boot_count = NextBootCount();
    s_latest.reset_flags = ReadResetFlags();
    ObcHk_Collect(&s_latest);
}

void ObcHk_Collect(ObcHkTelemetry *out)
{
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};

    if (out == NULL)
    {
        return;
    }

    out->sequence++;
    out->uptime_s = HAL_GetTick() / 1000U;
    out->mode = (uint8_t)s_mode;
    out->button1_pressed = Board_IsButton1Pressed() ? 1U : 0U;
    out->button2_pressed = Board_IsButton2Pressed() ? 1U : 0U;
    out->status_led = (Board_GetStatusLed() == GPIO_PIN_SET) ? 1U : 0U;
    out->last_opcode = s_last_opcode;
    out->last_status = s_last_status;

    if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK)
    {
        (void)HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
        out->rtc_year = date.Year;
        out->rtc_month = date.Month;
        out->rtc_day = date.Date;
        out->rtc_hour = time.Hours;
        out->rtc_minute = time.Minutes;
        out->rtc_second = time.Seconds;
    }
}

void ObcHk_Refresh(void)
{
    ObcHk_Collect(&s_latest);
}

void ObcHk_Service(void)
{
    const uint32_t now = HAL_GetTick();

    if ((now - s_last_collect_ms) < s_period_ms)
    {
        return;
    }

    s_last_collect_ms = now;
    ObcHk_Refresh();
}

const ObcHkTelemetry *ObcHk_GetLatest(void)
{
    return &s_latest;
}

void ObcHk_SetMode(ObcMode mode)
{
    s_mode = mode;
}

ObcMode ObcHk_GetMode(void)
{
    return s_mode;
}

void ObcHk_SetPeriodMs(uint32_t period_ms)
{
    if (period_ms < 100U)
    {
        period_ms = 100U;
    }
    if (period_ms > 60000U)
    {
        period_ms = 60000U;
    }
    s_period_ms = period_ms;
}

uint32_t ObcHk_GetPeriodMs(void)
{
    return s_period_ms;
}

void ObcHk_NoteCommand(uint8_t opcode, uint8_t status)
{
    s_last_opcode = opcode;
    s_last_status = status;
}

void ObcHk_Print(void)
{
    const ObcHkTelemetry *hk = &s_latest;
    const char *mode_name = "NOMINAL";

    if (hk->mode == (uint8_t)OBC_MODE_SAFE)
    {
        mode_name = "SAFE";
    }
    else if (hk->mode == (uint8_t)OBC_MODE_TEST)
    {
        mode_name = "TEST";
    }

    Console_Printf("HK seq=%lu uptime=%lus boot=%lu mode=%s\r\n",
                   (unsigned long)hk->sequence,
                   (unsigned long)hk->uptime_s,
                   (unsigned long)hk->boot_count,
                   mode_name);
    Console_Printf("    RTC 20%02u-%02u-%02u %02u:%02u:%02u  reset=0x%08lX\r\n",
                   hk->rtc_year, hk->rtc_month, hk->rtc_day,
                   hk->rtc_hour, hk->rtc_minute, hk->rtc_second,
                   (unsigned long)hk->reset_flags);
    Console_Printf("    btn1=%u btn2=%u led3=%u last_op=0x%02X st=0x%02X period=%lums\r\n",
                   hk->button1_pressed, hk->button2_pressed, hk->status_led,
                   hk->last_opcode, hk->last_status,
                   (unsigned long)s_period_ms);
}
