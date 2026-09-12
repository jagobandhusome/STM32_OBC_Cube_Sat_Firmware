/**
 * @file    ext_memory.c
 * @brief   External SRAM tests over the STM32H7 Flexible Memory Controller.
 */
#include "ext_memory.h"
#include "board_pins.h"
#include "console.h"

#include <stddef.h>

static uint16_t *ChipBase(unsigned int chip_index)
{
    return (uint16_t *)(EXT_SRAM_BASE_ADDRESS + ((uint32_t)chip_index * EXT_SRAM_CHIP_SIZE_BYTES));
}

void ExtMemory_ConfigureFmc(void)
{
    *FMC_BTR1_REG = FMC_BTR1_SRAM_ASYNC;
    *FMC_BCR1_REG = FMC_BCR1_SRAM_ASYNC;
}

void ExtMemory_ChipSelectTest(void)
{
    unsigned int chip;
    unsigned int word;

    Console_Print("Configuring FMC for external SRAM\r\n");
    ExtMemory_ConfigureFmc();

    for (chip = 0U; chip < EXT_SRAM_CHIP_COUNT; chip++)
    {
        uint16_t *location = ChipBase(chip);
        unsigned int failed = 0U;

        Console_Printf("Writing chip-select region %u\r\n", chip);

        for (word = 0U; word < 10U; word++)
        {
            location[word] = (uint16_t)word;
        }

        for (word = 0U; word < 10U; word++)
        {
            if (location[word] != (uint16_t)word)
            {
                Console_Printf("Error reading chip-select region %u at word %u\r\n", chip, word);
                failed = 1U;
                break;
            }
        }

        if (failed == 0U)
        {
            Console_Printf("Chip-select region %u passed\r\n", chip);
        }
    }
}

void ExtMemory_WriteTest(void)
{
    volatile uint16_t *memory = (volatile uint16_t *)EXT_SRAM_BASE_ADDRESS;
    const uint32_t word_count = EXT_SRAM_SIZE_BYTES / sizeof(uint16_t);
    uint32_t index;
    uint16_t value = 0U;

    Console_Print("Configuring FMC and writing SRAM pattern\r\n");
    ExtMemory_ConfigureFmc();

    for (index = 0U; index < word_count; index++)
    {
        memory[index] = value;
        value++;

        if ((index > 0U) && ((index % 32768U) == 0U))
        {
            const uint32_t percent = (index * 100U) / word_count;
            Console_Printf("Write progress: %lu%%\r\n", (unsigned long)percent);
        }
    }

    Console_Print("SRAM write test complete\r\n");
}

void ExtMemory_ReadTest(void)
{
    volatile uint16_t *memory = (volatile uint16_t *)EXT_SRAM_BASE_ADDRESS;
    const uint32_t word_count = EXT_SRAM_SIZE_BYTES / sizeof(uint16_t);
    uint32_t index;
    uint16_t expected = 0U;
    unsigned int failed = 0U;
    uint32_t first_error = 0U;

    Console_Print("Configuring FMC and verifying SRAM pattern\r\n");
    ExtMemory_ConfigureFmc();

    for (index = 0U; index < word_count; index++)
    {
        const uint16_t actual = memory[index];

        if (actual != expected)
        {
            if (failed == 0U)
            {
                first_error = index;
                Console_Printf("First mismatch at word %lu: expected 0x%04X, read 0x%04X\r\n",
                               (unsigned long)index, expected, actual);
            }
            failed = 1U;
        }

        expected++;

        if ((index > 0U) && ((index % 32768U) == 0U))
        {
            const uint32_t percent = (index * 100U) / word_count;
            Console_Printf("Read progress: %lu%%\r\n", (unsigned long)percent);
        }
    }

    if (failed != 0U)
    {
        Console_Printf("SRAM read test FAILED (first error at word %lu)\r\n",
                       (unsigned long)first_error);
    }
    else
    {
        Console_Print("SRAM read test PASSED\r\n");
    }
}
