/**
 * @file    can_app.c
 * @brief   FDCAN bring-up and interrupt handlers for the CubeSat OBC.
 */
#include "can_app.h"
#include "console.h"
#include "STM32CAN_ContollerAreaNetwork.h"

#include <stddef.h>

static volatile unsigned int s_can_test_running[2] = {0U, 0U};

static void EchoReceivedFrame(teSTM32CAN_PERIPHERAL peripheral,
                              tUINT32 fifo_address,
                              tUINT32 frame_count)
{
    unsigned int i;
    unsigned int frame;
    tsSTM32CAN_RXFifoElement *element = (tsSTM32CAN_RXFifoElement *)fifo_address;

    for (frame = 0U; frame < frame_count; frame++)
    {
        tsSTM32CAN_Frame tx_frame;
        const unsigned int length = (element->iDLC > STM32CAN_FRAME_DATA_MAX_LENGTH)
                                        ? STM32CAN_FRAME_DATA_MAX_LENGTH
                                        : element->iDLC;

        tx_frame.iIdentifier = element->iID;
        tx_frame.iDataLength = length;
        for (i = 0U; i < length; i++)
        {
            tx_frame.aData[i] = element->aDataBytes[i];
        }

        (void)STM32CAN_Transmit(peripheral, &tx_frame, 1U);
        element++;
    }
}

static void HandleCanIsr(teSTM32CAN_PERIPHERAL peripheral, unsigned int bus_index)
{
    tUINT32 fifo_address = 0U;
    tUINT32 first_index = 0U;
    tUINT32 frame_count = 0U;
    tUINT32 fifo_capacity = 0U;

    if (STM32CAN_Receive(peripheral, &fifo_address, &first_index, &frame_count, &fifo_capacity) != BT_SUCCESS)
    {
        (void)STM32CAN_Flush(peripheral);
        return;
    }

    if ((s_can_test_running[bus_index] != 0U) && (frame_count > 0U))
    {
        EchoReceivedFrame(peripheral, fifo_address, frame_count);
    }

    (void)STM32CAN_Flush(peripheral);
}

void CanApp_Init(void)
{
    tsSTM32CAN_Frame startup_frame;

    (void)STM32CAN_Initialise(CAN_PERIPHERAL_FDCAN1,
                              (tUINT32 *)STM32CAN_FDCAN1_MESSAGE_RAM_ADDRESS,
                              CAN_BAUD_1MBPS, 0U, 1U, 1U, 1U);
    (void)STM32CAN_Initialise(CAN_PERIPHERAL_FDCAN2,
                              (tUINT32 *)STM32CAN_FDCAN2_MESSAGE_RAM_ADDRESS,
                              CAN_BAUD_1MBPS, 0U, 1U, 1U, 1U);

    (void)STM32CAN_RegisterCallbacks(CAN_PERIPHERAL_FDCAN1, CAN1_ISR, 0);
    (void)STM32CAN_RegisterCallbacks(CAN_PERIPHERAL_FDCAN2, CAN2_ISR, 0);
    (void)STM32CAN_Connect(CAN_PERIPHERAL_FDCAN1);
    (void)STM32CAN_Connect(CAN_PERIPHERAL_FDCAN2);

    startup_frame.iDataLength = 2U;
    startup_frame.iIdentifier = 0x55U;
    startup_frame.aData[0] = 0x55U;
    startup_frame.aData[1] = 0x55U;

    (void)STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN1, &startup_frame, 1U);
    (void)STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN2, &startup_frame, 1U);
}

void CanApp_LoopbackTest(unsigned int bus_index)
{
    if (bus_index > 1U)
    {
        return;
    }

    Console_Printf("Starting CAN%u loopback for 10 seconds\r\n", bus_index + 1U);
    s_can_test_running[bus_index] = 1U;
    HAL_Delay(10000);
    s_can_test_running[bus_index] = 0U;
    Console_Printf("CAN%u loopback complete\r\n", bus_index + 1U);
}

void CAN1_ISR(void)
{
    HandleCanIsr(CAN_PERIPHERAL_FDCAN1, 0U);
}

void CAN2_ISR(void)
{
    HandleCanIsr(CAN_PERIPHERAL_FDCAN2, 1U);
}
