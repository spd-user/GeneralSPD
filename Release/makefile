################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/spd/topology/network/subdir.mk
-include src/spd/topology/lattice/subdir.mk
-include src/spd/topology/cube/subdir.mk
-include src/spd/topology/subdir.mk
-include src/spd/rule/strategy/subdir.mk
-include src/spd/rule/property/subdir.mk
-include src/spd/rule/game/subdir.mk
-include src/spd/rule/action/subdir.mk
-include src/spd/rule/subdir.mk
-include src/spd/param/parser/subdir.mk
-include src/spd/param/subdir.mk
-include src/spd/output/subdir.mk
-include src/spd/core/maker/subdir.mk
-include src/spd/core/subdir.mk
-include src/spd/subdir.mk
-include lib/msgpack-0.5.7/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: General_SPD

# Tool invocations
General_SPD: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "General_SPD" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) General_SPD
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
