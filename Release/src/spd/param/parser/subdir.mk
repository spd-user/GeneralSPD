################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/param/parser/InitParser.cpp \
../src/spd/param/parser/NeighborParser.cpp \
../src/spd/param/parser/OutputParser.cpp \
../src/spd/param/parser/ParseParam.cpp \
../src/spd/param/parser/RandomParser.cpp \
../src/spd/param/parser/RuntimeParser.cpp 

OBJS += \
./src/spd/param/parser/InitParser.o \
./src/spd/param/parser/NeighborParser.o \
./src/spd/param/parser/OutputParser.o \
./src/spd/param/parser/ParseParam.o \
./src/spd/param/parser/RandomParser.o \
./src/spd/param/parser/RuntimeParser.o 

CPP_DEPS += \
./src/spd/param/parser/InitParser.d \
./src/spd/param/parser/NeighborParser.d \
./src/spd/param/parser/OutputParser.d \
./src/spd/param/parser/ParseParam.d \
./src/spd/param/parser/RandomParser.d \
./src/spd/param/parser/RuntimeParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/param/parser/%.o: ../src/spd/param/parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


