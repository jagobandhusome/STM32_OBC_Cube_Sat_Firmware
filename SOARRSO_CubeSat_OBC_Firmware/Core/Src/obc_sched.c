/**
 * @file    obc_sched.c
 * @brief   Cooperative periodic tasks: command, board, housekeeping.
 */
#include "obc_sched.h"
#include "obc_cmd.h"
#include "obc_hk.h"
#include "board.h"

#include "stm32h7xx_hal.h"

typedef struct
{
    const char *name;
    uint32_t period_ms;
    uint32_t last_run_ms;
    void (*run)(void);
} ObcTask;

static void TaskCommand(void)
{
    ObcCmd_Service();
}

static void TaskBoard(void)
{
    Board_Service();
}

static void TaskHousekeeping(void)
{
    ObcHk_Service();
}

static ObcTask s_tasks[] =
{
    { "CMD", 0U,    0U, TaskCommand },
    { "BOARD", 250U, 0U, TaskBoard },
    { "HK",  100U,  0U, TaskHousekeeping }
};

void ObcSched_Init(void)
{
    const uint32_t now = HAL_GetTick();
    unsigned int i;

    for (i = 0U; i < (sizeof(s_tasks) / sizeof(s_tasks[0])); i++)
    {
        s_tasks[i].last_run_ms = now;
    }
}

void ObcSched_Run(void)
{
    const uint32_t now = HAL_GetTick();
    unsigned int i;

    for (i = 0U; i < (sizeof(s_tasks) / sizeof(s_tasks[0])); i++)
    {
        ObcTask *task = &s_tasks[i];

        if ((task->period_ms == 0U) || ((now - task->last_run_ms) >= task->period_ms))
        {
            task->last_run_ms = now;
            task->run();
        }
    }
}
