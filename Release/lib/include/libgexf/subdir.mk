################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/include/libgexf/abstractparser.cpp \
../lib/include/libgexf/attributeiter.cpp \
../lib/include/libgexf/attvalueiter.cpp \
../lib/include/libgexf/conv.cpp \
../lib/include/libgexf/data.cpp \
../lib/include/libgexf/directedgraph.cpp \
../lib/include/libgexf/dynamicgraph.cpp \
../lib/include/libgexf/edgeiter.cpp \
../lib/include/libgexf/filereader.cpp \
../lib/include/libgexf/filewriter.cpp \
../lib/include/libgexf/gexf.cpp \
../lib/include/libgexf/gexfparser.cpp \
../lib/include/libgexf/graph.cpp \
../lib/include/libgexf/legacyparser.cpp \
../lib/include/libgexf/legacywriter.cpp \
../lib/include/libgexf/memoryvalidator.cpp \
../lib/include/libgexf/metadata.cpp \
../lib/include/libgexf/nodeiter.cpp \
../lib/include/libgexf/rngvalidator.cpp \
../lib/include/libgexf/schemavalidator.cpp \
../lib/include/libgexf/undirectedgraph.cpp 

OBJS += \
./lib/include/libgexf/abstractparser.o \
./lib/include/libgexf/attributeiter.o \
./lib/include/libgexf/attvalueiter.o \
./lib/include/libgexf/conv.o \
./lib/include/libgexf/data.o \
./lib/include/libgexf/directedgraph.o \
./lib/include/libgexf/dynamicgraph.o \
./lib/include/libgexf/edgeiter.o \
./lib/include/libgexf/filereader.o \
./lib/include/libgexf/filewriter.o \
./lib/include/libgexf/gexf.o \
./lib/include/libgexf/gexfparser.o \
./lib/include/libgexf/graph.o \
./lib/include/libgexf/legacyparser.o \
./lib/include/libgexf/legacywriter.o \
./lib/include/libgexf/memoryvalidator.o \
./lib/include/libgexf/metadata.o \
./lib/include/libgexf/nodeiter.o \
./lib/include/libgexf/rngvalidator.o \
./lib/include/libgexf/schemavalidator.o \
./lib/include/libgexf/undirectedgraph.o 

CPP_DEPS += \
./lib/include/libgexf/abstractparser.d \
./lib/include/libgexf/attributeiter.d \
./lib/include/libgexf/attvalueiter.d \
./lib/include/libgexf/conv.d \
./lib/include/libgexf/data.d \
./lib/include/libgexf/directedgraph.d \
./lib/include/libgexf/dynamicgraph.d \
./lib/include/libgexf/edgeiter.d \
./lib/include/libgexf/filereader.d \
./lib/include/libgexf/filewriter.d \
./lib/include/libgexf/gexf.d \
./lib/include/libgexf/gexfparser.d \
./lib/include/libgexf/graph.d \
./lib/include/libgexf/legacyparser.d \
./lib/include/libgexf/legacywriter.d \
./lib/include/libgexf/memoryvalidator.d \
./lib/include/libgexf/metadata.d \
./lib/include/libgexf/nodeiter.d \
./lib/include/libgexf/rngvalidator.d \
./lib/include/libgexf/schemavalidator.d \
./lib/include/libgexf/undirectedgraph.d 


# Each subdirectory must supply rules for building sources it contributes
lib/include/libgexf/%.o: ../lib/include/libgexf/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/home/katsumata/workspace/General_SPD/lib/include/libxml2 -I/home/katsumata/workspace/General_SPD/lib/include -I/usr/include/c++/4.7 -I/usr/include/c++/4.7/backward -I/usr/include/c++/4.7/x86_64-suse-linux -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include -I/usr/lib64/gcc/x86_64-suse-linux/4.7/include-fixed -I/usr/local/include -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


