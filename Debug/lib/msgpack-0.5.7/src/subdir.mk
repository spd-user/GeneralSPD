################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/msgpack-0.5.7/src/gcc_atomic.cpp \
../lib/msgpack-0.5.7/src/object.cpp 

C_SRCS += \
../lib/msgpack-0.5.7/src/objectc.c \
../lib/msgpack-0.5.7/src/unpack.c \
../lib/msgpack-0.5.7/src/version.c \
../lib/msgpack-0.5.7/src/vrefbuffer.c \
../lib/msgpack-0.5.7/src/zone.c 

OBJS += \
./lib/msgpack-0.5.7/src/gcc_atomic.o \
./lib/msgpack-0.5.7/src/object.o \
./lib/msgpack-0.5.7/src/objectc.o \
./lib/msgpack-0.5.7/src/unpack.o \
./lib/msgpack-0.5.7/src/version.o \
./lib/msgpack-0.5.7/src/vrefbuffer.o \
./lib/msgpack-0.5.7/src/zone.o 

C_DEPS += \
./lib/msgpack-0.5.7/src/objectc.d \
./lib/msgpack-0.5.7/src/unpack.d \
./lib/msgpack-0.5.7/src/version.d \
./lib/msgpack-0.5.7/src/vrefbuffer.d \
./lib/msgpack-0.5.7/src/zone.d 

CPP_DEPS += \
./lib/msgpack-0.5.7/src/gcc_atomic.d \
./lib/msgpack-0.5.7/src/object.d 


# Each subdirectory must supply rules for building sources it contributes
lib/msgpack-0.5.7/src/%.o: ../lib/msgpack-0.5.7/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/msgpack-0.5.7/src/%.o: ../lib/msgpack-0.5.7/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


