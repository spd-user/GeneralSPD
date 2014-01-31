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
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/usr/include -I/usr/include/c++/4.8 -I/usr/include/c++/4.8/backward -I/usr/include/c++/4.8/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include-fixed -I/usr/local/include -I/usr/x86_64-suse-linux/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


