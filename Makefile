# This file is based on the Makefile automagically generated by mbed.org.

BUILD_DIR = ./build

PROJ = tp5

###############################################################################
#
# source code
#
SRC += ./main4-3-1.cpp
SRC += ./util.cpp
FREERTOS_SRC += $(wildcard ./freertos/*.c)
FREERTOS_SRC += ./freertos/portable/MemMang/heap_1.c
FREERTOS_SRC += ./freertos/portable/GCC/ARM_CM3/port.c
TRACEALYZER_SRC += $(wildcard ./tracealyzer/*.c)

###############################################################################
#
# object files required (one per source code file)
#
OBJECTS += $(SRC:.cpp=.o)
FREERTOS_OBJECTS += $(FREERTOS_SRC:.c=.o)
TRACEALYZER_OBJECTS += $(TRACEALYZER_SRC:.c=.o)
SYS_OBJECTS += $(wildcard ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/*.o)

###############################################################################
#
# where to find the required headers
#
MBED_INCLUDE_PATHS += -I./mbed
MBED_INCLUDE_PATHS += -I./mbed/drivers
MBED_INCLUDE_PATHS += -I./mbed/hal
MBED_INCLUDE_PATHS += -I./mbed/platform
MBED_INCLUDE_PATHS += -I./mbed/TARGET_LPC1768
MBED_INCLUDE_PATHS += -I./mbed/TARGET_LPC1768/TARGET_NXP
MBED_INCLUDE_PATHS += -I./mbed/TARGET_LPC1768/TARGET_NXP/TARGET_LPC176X/
MBED_INCLUDE_PATHS += -I./mbed/TARGET_LPC1768/TARGET_NXP/TARGET_LPC176X/device
MBED_INCLUDE_PATHS += -I./mbed/TARGET_LPC1768/TARGET_NXP/TARGET_LPC176X/TARGET_MBED_LPC1768

FREERTOS_INCLUDE_PATHS += -I./freertos/include
FREERTOS_INCLUDE_PATHS += -I./freertos/portable/GCC/ARM_CM3

TRACEALYZER_INCLUDE_PATHS += -I./tracealyzer/config
TRACEALYZER_INCLUDE_PATHS += -I./tracealyzer/include

INCLUDE_PATHS += -I.
INCLUDE_PATHS += $(MBED_INCLUDE_PATHS)
INCLUDE_PATHS += $(FREERTOS_INCLUDE_PATHS)
INCLUDE_PATHS += $(TRACEALYZER_INCLUDE_PATHS)

###############################################################################
#
# where to find the required libraries (*.a files)
#
LIBRARY_PATHS += -L./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM
LIBRARIES = -lmbed

###############################################################################
#
# linker script used to build the binary
#
LINKER_SCRIPT = ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/LPC1768.ld

###############################################################################
#
# flags and symbols required by the compiler
#
CPU += -mcpu=cortex-m3 
CPU += -mthumb

C_FLAGS += $(CPU) 
C_FLAGS += -c
C_FLAGS += -g
C_FLAGS += -fno-common
C_FLAGS += -fmessage-length=0
C_FLAGS += -Wall
C_FLAGS += -fno-exceptions
C_FLAGS += -ffunction-sections
C_FLAGS += -fdata-sections
C_FLAGS += -fomit-frame-pointer
C_FLAGS += -include mbed_config.h
C_FLAGS += -MMD
C_FLAGS += -MP

CC_FLAGS = $(C_FLAGS)
CC_FLAGS += -fno-rtti

CC_SYMBOLS += -DTARGET_LPC1768
CC_SYMBOLS += -DTARGET_M3
CC_SYMBOLS += -DTARGET_NXP
CC_SYMBOLS += -DTARGET_LPC176X
CC_SYMBOLS += -DTARGET_MBED_LPC1768
CC_SYMBOLS += -DTOOLCHAIN_GCC_ARM
CC_SYMBOLS += -DTOOLCHAIN_GCC
CC_SYMBOLS += -D__CORTEX_M3
CC_SYMBOLS += -DARM_MATH_CM3
CC_SYMBOLS += -DMBED_BUILD_TIMESTAMP=1414254042.69
CC_SYMBOLS += -D__MBED__=1

LD_FLAGS += $(CPU)
LD_FLAGS += -Wl,--gc-sections
LD_FLAGS += -u _printf_float
LD_FLAGS += -u _scanf_float

LD_SYS_LIBS += -lstdc++
LD_SYS_LIBS += -lsupc++
LD_SYS_LIBS += -lm
LD_SYS_LIBS += -lc
LD_SYS_LIBS += -lgcc
LD_SYS_LIBS += -lnosys

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -g
else
  CC_FLAGS += -DNDEBUG -Os
endif

############################################################################### 
#
# compiler executables paths
#
GCC_BIN =
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
SIZE    = $(GCC_BIN)arm-none-eabi-size

###############################################################################
#
# rules used to build the program
#
all: $(BUILD_DIR)/$(PROJ).bin size

clean:
	+@echo "Cleaning files..."
	@rm -f $(BUILD_DIR)/$(PROJ).bin $(BUILD_DIR)/$(PROJ).elf $(OBJECTS) $(FREERTOS_OBJECTS) $(TRACEALYZER_OBJECTS) $(DEPS)
	
.c.o:
	+@echo "Compile: $<"
	@$(CC) $(C_FLAGS) $(CC_SYMBOLS) -std=gnu99  $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@echo "Compile: $<"
	@$(CPP) $(CC_FLAGS) -fno-rtti $(CC_SYMBOLS) -std=gnu++98 $(INCLUDE_PATHS) -o $@ $<	

$(BUILD_DIR)/$(PROJ).elf: $(OBJECTS) $(FREERTOS_OBJECTS) $(TRACEALYZER_OBJECTS) $(SYS_OBJECTS)
	+@echo "Linking: $@"
	@$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES) $(LD_SYS_LIBS)

$(BUILD_DIR)/$(PROJ).bin: $(BUILD_DIR)/$(PROJ).elf
	+@echo "Binary: $@"
	@$(OBJCOPY) -O binary $< $@
	
size: $(BUILD_DIR)/$(PROJ).elf
	$(SIZE) $<

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d) $(FREERTOS_OBJECTS:.o=.d) $(TRACEALYZER_OBJECTS:.o=.d)
-include $(DEPS)
