################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.c 

OBJS += \
./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.o 

C_DEPS += \
./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/EEPROM_Emul/Porting/STM32G4/%.o Middlewares/ST/EEPROM_Emul/Porting/STM32G4/%.su Middlewares/ST/EEPROM_Emul/Porting/STM32G4/%.cyclo: ../Middlewares/ST/EEPROM_Emul/Porting/STM32G4/%.c Middlewares/ST/EEPROM_Emul/Porting/STM32G4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/EEPROM_Emul/Core -I../Middlewares/ST/EEPROM_Emul/Porting/STM32WB -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I../Drivers/CMSIS/Include -I"C:/cases_root/test/WB_BLE_SRAM_NVM/Drivers/BSP/P-NUCLEO-WB55.Nucleo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-EEPROM_Emul-2f-Porting-2f-STM32G4

clean-Middlewares-2f-ST-2f-EEPROM_Emul-2f-Porting-2f-STM32G4:
	-$(RM) ./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.cyclo ./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.d ./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.o ./Middlewares/ST/EEPROM_Emul/Porting/STM32G4/flash_interface.su

.PHONY: clean-Middlewares-2f-ST-2f-EEPROM_Emul-2f-Porting-2f-STM32G4

