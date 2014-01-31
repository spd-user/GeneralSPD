################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/rule/game/AverageGameRule.cpp \
../src/spd/rule/game/SimpleSumGameRule.cpp 

OBJS += \
./src/spd/rule/game/AverageGameRule.o \
./src/spd/rule/game/SimpleSumGameRule.o 

CPP_DEPS += \
./src/spd/rule/game/AverageGameRule.d \
./src/spd/rule/game/SimpleSumGameRule.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/rule/game/%.o: ../src/spd/rule/game/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -I/usr/include -I/usr/x86_64-suse-linux/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


