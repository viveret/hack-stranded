################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ss_lib/Core.cpp \
../ss_lib/Core_Funcs.cpp \
../ss_lib/Core_Types.cpp \
../ss_lib/File_Sys.cpp \
../ss_lib/Math.cpp 

OBJS += \
./ss_lib/Core.o \
./ss_lib/Core_Funcs.o \
./ss_lib/Core_Types.o \
./ss_lib/File_Sys.o \
./ss_lib/Math.o 

CPP_DEPS += \
./ss_lib/Core.d \
./ss_lib/Core_Funcs.d \
./ss_lib/Core_Types.d \
./ss_lib/File_Sys.d \
./ss_lib/Math.d 


# Each subdirectory must supply rules for building sources it contributes
ss_lib/%.o: ../ss_lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


