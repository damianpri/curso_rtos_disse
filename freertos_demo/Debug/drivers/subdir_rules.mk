################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/buttons.obj: D:/disse_rtos_workspace/workspace/drivers/buttons.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="D:/disse_rtos_workspace/workspace" --include_path="D:/disse_rtos_workspace/workspace/driverlib" --include_path="D:/disse_rtos_workspace/workspace/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/disse_rtos_workspace/workspace/FreeRTOS/Source/include" --include_path="D:/disse_rtos_workspace/workspace/freertos_demo/" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="drivers/buttons.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/rgb.obj: D:/disse_rtos_workspace/workspace/drivers/rgb.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="D:/disse_rtos_workspace/workspace" --include_path="D:/disse_rtos_workspace/workspace/driverlib" --include_path="D:/disse_rtos_workspace/workspace/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/disse_rtos_workspace/workspace/FreeRTOS/Source/include" --include_path="D:/disse_rtos_workspace/workspace/freertos_demo/" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="drivers/rgb.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


