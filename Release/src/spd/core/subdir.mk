################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/core/Player.cpp \
../src/spd/core/Space.cpp \
../src/spd/core/Strategy.cpp 

OBJS += \
./src/spd/core/Player.o \
./src/spd/core/Space.o \
./src/spd/core/Strategy.o 

CPP_DEPS += \
./src/spd/core/Player.d \
./src/spd/core/Space.d \
./src/spd/core/Strategy.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/core/%.o: ../src/spd/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


