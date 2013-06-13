################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
io.obj: ../io.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="io.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mmc.obj: ../mmc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="mmc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdcard.obj: ../sdcard.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="sdcard.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

utils.obj: ../utils.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --include_path="D:/ti/ccsv5/tools/compiler/arm_5.0.4/include" --include_path="D:/StellarisWare/boards/ek-lm3s9b92" --include_path="D:/StellarisWare" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="utils.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


