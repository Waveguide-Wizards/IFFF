################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/CCS/ARM_CM4F/%.obj: ../FreeRTOS/Source/portable/CCS/ARM_CM4F/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/amabo/Documents/GitHub/IFFF" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/littlefs" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/drivers/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/drivers/source" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/portable/MemMang" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/src" --include_path="/home/jack/ti/tivaware_c_series_2_1_4_178" --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS/Source/portable/CCS/ARM_CM4F/%.obj: ../FreeRTOS/Source/portable/CCS/ARM_CM4F/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/amabo/Documents/GitHub/IFFF" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/littlefs" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/drivers/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/drivers/source" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/FreeRTOS/Source/portable/MemMang" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/include" --include_path="C:/Users/amabo/Documents/GitHub/IFFF/src" --include_path="/home/jack/ti/tivaware_c_series_2_1_4_178" --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


