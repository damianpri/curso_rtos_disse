################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="D:/Curso RTOS/workspace_windows" --include_path="D:/Curso RTOS/workspace_windows/driverlib" --include_path="D:/Curso RTOS/workspace_windows/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/Curso RTOS/workspace_windows/FreeRTOS/Source/include" --include_path="D:/Curso RTOS/workspace_windows/freertos_proj_1Tarea_enviado/" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="D:/Curso RTOS/workspace_windows" --include_path="D:/Curso RTOS/workspace_windows/driverlib" --include_path="D:/Curso RTOS/workspace_windows/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/Curso RTOS/workspace_windows/FreeRTOS/Source/include" --include_path="D:/Curso RTOS/workspace_windows/freertos_proj_1Tarea_enviado/" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


