################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/msgpack-0.5.7/test/msgpack_test.cpp \
../lib/msgpack-0.5.7/test/msgpackc_test.cpp 

CC_SRCS += \
../lib/msgpack-0.5.7/test/buffer.cc \
../lib/msgpack-0.5.7/test/cases.cc \
../lib/msgpack-0.5.7/test/convert.cc \
../lib/msgpack-0.5.7/test/fixint.cc \
../lib/msgpack-0.5.7/test/fixint_c.cc \
../lib/msgpack-0.5.7/test/object.cc \
../lib/msgpack-0.5.7/test/pack_unpack.cc \
../lib/msgpack-0.5.7/test/pack_unpack_c.cc \
../lib/msgpack-0.5.7/test/streaming.cc \
../lib/msgpack-0.5.7/test/streaming_c.cc \
../lib/msgpack-0.5.7/test/version.cc \
../lib/msgpack-0.5.7/test/zone.cc 

OBJS += \
./lib/msgpack-0.5.7/test/buffer.o \
./lib/msgpack-0.5.7/test/cases.o \
./lib/msgpack-0.5.7/test/convert.o \
./lib/msgpack-0.5.7/test/fixint.o \
./lib/msgpack-0.5.7/test/fixint_c.o \
./lib/msgpack-0.5.7/test/msgpack_test.o \
./lib/msgpack-0.5.7/test/msgpackc_test.o \
./lib/msgpack-0.5.7/test/object.o \
./lib/msgpack-0.5.7/test/pack_unpack.o \
./lib/msgpack-0.5.7/test/pack_unpack_c.o \
./lib/msgpack-0.5.7/test/streaming.o \
./lib/msgpack-0.5.7/test/streaming_c.o \
./lib/msgpack-0.5.7/test/version.o \
./lib/msgpack-0.5.7/test/zone.o 

CC_DEPS += \
./lib/msgpack-0.5.7/test/buffer.d \
./lib/msgpack-0.5.7/test/cases.d \
./lib/msgpack-0.5.7/test/convert.d \
./lib/msgpack-0.5.7/test/fixint.d \
./lib/msgpack-0.5.7/test/fixint_c.d \
./lib/msgpack-0.5.7/test/object.d \
./lib/msgpack-0.5.7/test/pack_unpack.d \
./lib/msgpack-0.5.7/test/pack_unpack_c.d \
./lib/msgpack-0.5.7/test/streaming.d \
./lib/msgpack-0.5.7/test/streaming_c.d \
./lib/msgpack-0.5.7/test/version.d \
./lib/msgpack-0.5.7/test/zone.d 

CPP_DEPS += \
./lib/msgpack-0.5.7/test/msgpack_test.d \
./lib/msgpack-0.5.7/test/msgpackc_test.d 


# Each subdirectory must supply rules for building sources it contributes
lib/msgpack-0.5.7/test/%.o: ../lib/msgpack-0.5.7/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/msgpack-0.5.7/test/%.o: ../lib/msgpack-0.5.7/test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -DDEBUG -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


