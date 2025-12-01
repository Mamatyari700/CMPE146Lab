################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
freertos/croutine.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/croutine.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/croutine.d_raw" -MT"freertos/croutine.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/event_groups.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/event_groups.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/event_groups.d_raw" -MT"freertos/event_groups.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/heap_4.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/MemMang/heap_4.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/heap_4.d_raw" -MT"freertos/heap_4.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/list.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/list.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/list.d_raw" -MT"freertos/list.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/queue.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/queue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/queue.d_raw" -MT"freertos/queue.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/stream_buffer.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/stream_buffer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/stream_buffer.d_raw" -MT"freertos/stream_buffer.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/tasks.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/tasks.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/tasks.d_raw" -MT"freertos/tasks.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/timers.o: C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/timers.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/panda/Documents/CMPE 146/CMPE146Lab/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"freertos/timers.d_raw" -MT"freertos/timers.o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


