/**
 * @file    can_app.h
 * @brief   FDCAN application layer for the CubeSat OBC.
 */
#ifndef CAN_APP_H
#define CAN_APP_H

void CanApp_Init(void);
void CanApp_LoopbackTest(unsigned int bus_index);

void CAN1_ISR(void);
void CAN2_ISR(void);

#endif /* CAN_APP_H */
