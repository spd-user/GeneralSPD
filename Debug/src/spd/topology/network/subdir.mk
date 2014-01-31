################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/topology/network/Network.cpp \
../src/spd/topology/network/Random.cpp \
../src/spd/topology/network/Regular.cpp \
../src/spd/topology/network/Ring.cpp 

OBJS += \
./src/spd/topology/network/Network.o \
./src/spd/topology/network/Random.o \
./src/spd/topology/network/Regular.o \
./src/spd/topology/network/Ring.o 

CPP_DEPS += \
./src/spd/topology/network/Network.d \
./src/spd/topology/network/Random.d \
./src/spd/topology/network/Regular.d \
./src/spd/topology/network/Ring.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/topology/network/%.o: ../src/spd/topology/network/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/usr/include -I/usr/include/c++/4.8 -I/usr/include/c++/4.8/backward -I/usr/include/c++/4.8/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include-fixed -I/usr/local/include -I/usr/x86_64-suse-linux/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


