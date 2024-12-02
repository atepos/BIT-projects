################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/date.c \
../Sources/init.c \
../Sources/main.c \
../Sources/print.c \
../Sources/state_machine.c \
../Sources/str.c \
../Sources/utilities.c 

OBJS += \
./Sources/date.o \
./Sources/init.o \
./Sources/main.o \
./Sources/print.o \
./Sources/state_machine.o \
./Sources/str.o \
./Sources/utilities.o 

C_DEPS += \
./Sources/date.d \
./Sources/init.d \
./Sources/main.d \
./Sources/print.d \
./Sources/state_machine.d \
./Sources/str.d \
./Sources/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


