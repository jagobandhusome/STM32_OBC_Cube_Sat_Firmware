################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/STM32CAN_ContollerAreaNetwork.c \
../Core/Src/board.c \
../Core/Src/can_app.c \
../Core/Src/console.c \
../Core/Src/ext_memory.c \
../Core/Src/hw_tests.c \
../Core/Src/obc_cmd.c \
../Core/Src/obc_crc16.c \
../Core/Src/obc_hk.c \
../Core/Src/obc_sched.c \
../Core/Src/main.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c 

OBJS += \
./Core/Src/STM32CAN_ContollerAreaNetwork.o \
./Core/Src/board.o \
./Core/Src/can_app.o \
./Core/Src/console.o \
./Core/Src/ext_memory.o \
./Core/Src/hw_tests.o \
./Core/Src/obc_cmd.o \
./Core/Src/obc_crc16.o \
./Core/Src/obc_hk.o \
./Core/Src/obc_sched.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o 

C_DEPS += \
./Core/Src/STM32CAN_ContollerAreaNetwork.d \
./Core/Src/board.d \
./Core/Src/can_app.d \
./Core/Src/console.d \
./Core/Src/ext_memory.d \
./Core/Src/hw_tests.d \
./Core/Src/obc_cmd.d \
./Core/Src/obc_crc16.d \
./Core/Src/obc_hk.d \
./Core/Src/obc_sched.d \
./Core/Src/main.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H753xx -DSOARRSO_CUBESAT_OBC -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/STM32CAN_ContollerAreaNetwork.d ./Core/Src/STM32CAN_ContollerAreaNetwork.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o

.PHONY: clean-Core-2f-Src

