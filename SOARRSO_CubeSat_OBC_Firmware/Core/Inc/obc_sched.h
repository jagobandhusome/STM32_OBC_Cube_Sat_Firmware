/**
 * @file    obc_sched.h
 * @brief   Cooperative flight-task scheduler for the SOARRSO CubeSat OBC.
 *
 * Each entry is a standalone activity (housekeeping, command, board).
 * When FreeRTOS is added later, create one task per function and retire
 * ObcSched_Run() — do not mix this dispatcher with the FreeRTOS kernel.
 */
#ifndef OBC_SCHED_H
#define OBC_SCHED_H

#include <stdint.h>

void ObcSched_Init(void);
void ObcSched_Run(void);

#endif /* OBC_SCHED_H */
