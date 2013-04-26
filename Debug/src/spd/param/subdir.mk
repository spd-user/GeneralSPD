################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/param/GenerateOutput.cpp \
../src/spd/param/GenerateSpdRule.cpp \
../src/spd/param/GenerateTopology.cpp \
../src/spd/param/InitParameter.cpp \
../src/spd/param/NeighborhoodParameter.cpp \
../src/spd/param/OutputParameter.cpp \
../src/spd/param/Parameter.cpp \
../src/spd/param/RandomParameter.cpp \
../src/spd/param/RuntimeParameter.cpp 

OBJS += \
./src/spd/param/GenerateOutput.o \
./src/spd/param/GenerateSpdRule.o \
./src/spd/param/GenerateTopology.o \
./src/spd/param/InitParameter.o \
./src/spd/param/NeighborhoodParameter.o \
./src/spd/param/OutputParameter.o \
./src/spd/param/Parameter.o \
./src/spd/param/RandomParameter.o \
./src/spd/param/RuntimeParameter.o 

CPP_DEPS += \
./src/spd/param/GenerateOutput.d \
./src/spd/param/GenerateSpdRule.d \
./src/spd/param/GenerateTopology.d \
./src/spd/param/InitParameter.d \
./src/spd/param/NeighborhoodParameter.d \
./src/spd/param/OutputParameter.d \
./src/spd/param/Parameter.d \
./src/spd/param/RandomParameter.d \
./src/spd/param/RuntimeParameter.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/param/%.o: ../src/spd/param/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


