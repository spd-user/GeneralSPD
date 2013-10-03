################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/core/maker/CommandLineBasedMaker.cpp \
../src/spd/core/maker/DeserializedMaker.cpp \
../src/spd/core/maker/GEXFBasedMaker.cpp 

OBJS += \
./src/spd/core/maker/CommandLineBasedMaker.o \
./src/spd/core/maker/DeserializedMaker.o \
./src/spd/core/maker/GEXFBasedMaker.o 

CPP_DEPS += \
./src/spd/core/maker/CommandLineBasedMaker.d \
./src/spd/core/maker/DeserializedMaker.d \
./src/spd/core/maker/GEXFBasedMaker.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/core/maker/%.o: ../src/spd/core/maker/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


