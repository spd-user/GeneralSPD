################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/spd/output/BinaryOutput.cpp \
../src/spd/output/ConsoleOutput.cpp \
../src/spd/output/GEXFOutput.cpp \
../src/spd/output/ImageOutput.cpp \
../src/spd/output/NumberOutput.cpp \
../src/spd/output/OutputVisitor.cpp \
../src/spd/output/PayoffOutput.cpp \
../src/spd/output/PropertyOutput.cpp 

OBJS += \
./src/spd/output/BinaryOutput.o \
./src/spd/output/ConsoleOutput.o \
./src/spd/output/GEXFOutput.o \
./src/spd/output/ImageOutput.o \
./src/spd/output/NumberOutput.o \
./src/spd/output/OutputVisitor.o \
./src/spd/output/PayoffOutput.o \
./src/spd/output/PropertyOutput.o 

CPP_DEPS += \
./src/spd/output/BinaryOutput.d \
./src/spd/output/ConsoleOutput.d \
./src/spd/output/GEXFOutput.d \
./src/spd/output/ImageOutput.d \
./src/spd/output/NumberOutput.d \
./src/spd/output/OutputVisitor.d \
./src/spd/output/PayoffOutput.d \
./src/spd/output/PropertyOutput.d 


# Each subdirectory must supply rules for building sources it contributes
src/spd/output/%.o: ../src/spd/output/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/usr/include -I/usr/include/c++/4.8 -I/usr/include/c++/4.8/backward -I/usr/include/c++/4.8/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include -I/usr/lib64/gcc/x86_64-suse-linux/4.8/include-fixed -I/usr/local/include -I/usr/x86_64-suse-linux/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


