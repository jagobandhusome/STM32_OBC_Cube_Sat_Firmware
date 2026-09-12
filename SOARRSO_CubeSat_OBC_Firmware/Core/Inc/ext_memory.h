/**
 * @file    ext_memory.h
 * @brief   External SRAM / FMC memory tests.
 */
#ifndef EXT_MEMORY_H
#define EXT_MEMORY_H

void ExtMemory_ConfigureFmc(void);
void ExtMemory_ChipSelectTest(void);
void ExtMemory_WriteTest(void);
void ExtMemory_ReadTest(void);

#endif /* EXT_MEMORY_H */
