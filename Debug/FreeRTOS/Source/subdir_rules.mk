################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/%.obj: ../FreeRTOS/Source/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jack/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/jack/workspace_v9/IFFF" --include_path="/home/jack/workspace_v9/IFFF/drivers/include" --include_path="/home/jack/workspace_v9/IFFF/drivers/source" --include_path="/home/jack/workspace_v9/IFFF/FreeRTOS/Source/include" --include_path="/home/jack/workspace_v9/IFFF/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="/home/jack/workspace_v9/IFFF/FreeRTOS/Source/portable/MemMang" --include_path="/home/jack/workspace_v9/IFFF/include" --include_path="/home/jack/workspace_v9/IFFF/src" --include_path="/home/jack/ti/tivaware_c_series_2_1_4_178" --include_path="/home/jack/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


