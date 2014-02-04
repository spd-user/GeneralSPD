################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/output/color/AffectColor.cpp \
../src/spd/output/color/MembraneColor.cpp \
../src/spd/output/color/StandardColor.cpp 

OBJS += \
./src/spd/output/color/AffectColor.o \
./src/spd/output/color/MembraneColor.o \
./src/spd/output/color/StandardColor.o 

CPP_DEPS += \
./src/spd/output/color/AffectColor.d \
./src/spd/output/color/MembraneColor.d \
./src/spd/output/color/StandardColor.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/output/color/%.o: ../src/spd/output/color/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -I/usr/include -I/usr/x86_64-suse-linux/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


