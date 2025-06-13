################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/actions.c \
../app/button.c \
../app/display.c \
../app/gameoflife.c \
../app/main.c \
../app/menu.c \
../app/test.c 

OBJS += \
./app/actions.o \
./app/button.o \
./app/display.o \
./app/gameoflife.o \
./app/main.o \
./app/menu.o \
./app/test.o 

C_DEPS += \
./app/actions.d \
./app/button.d \
./app/display.d \
./app/gameoflife.d \
./app/main.d \
./app/menu.d \
./app/test.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../app -I../../common/core/Inc -I../../common/drivers/stm32g4xx_hal/Inc -I../../common/drivers/stm32g4xx_hal/Inc/Legacy -I../../common/drivers/cmsis/Device/ST/STM32G4xx/Include -I../../common/drivers/cmsis/Include -I../../common/drivers/bsp -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../drivers/bsp -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/actions.cyclo ./app/actions.d ./app/actions.o ./app/actions.su ./app/button.cyclo ./app/button.d ./app/button.o ./app/button.su ./app/display.cyclo ./app/display.d ./app/display.o ./app/display.su ./app/gameoflife.cyclo ./app/gameoflife.d ./app/gameoflife.o ./app/gameoflife.su ./app/main.cyclo ./app/main.d ./app/main.o ./app/main.su ./app/menu.cyclo ./app/menu.d ./app/menu.o ./app/menu.su ./app/test.cyclo ./app/test.d ./app/test.o ./app/test.su

.PHONY: clean-app

