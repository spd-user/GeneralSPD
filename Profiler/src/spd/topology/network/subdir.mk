################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/topology/network/Network.cpp \
../src/spd/topology/network/Random.cpp \
../src/spd/topology/network/Regular.cpp 

OBJS += \
./src/spd/topology/network/Network.o \
./src/spd/topology/network/Random.o \
./src/spd/topology/network/Regular.o 

CPP_DEPS += \
./src/spd/topology/network/Network.d \
./src/spd/topology/network/Random.d \
./src/spd/topology/network/Regular.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/topology/network/%.o: ../src/spd/topology/network/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -I/usr/include -I/usr/x86_64-suse-linux/include -O2 -pg -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


