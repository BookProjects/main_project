#Verbosity flags (Q=quiet, E=echo)
ifdef VERBOSE
	Q =
	E = @true
else
	Q = @
	E = @echo 
endif

# Define testing tool paths
UNITY_PATH := $(UNITY_SOURCE)
UNITY_EXTRA_PATH := $(UNITY_PATH)/extras/fixture/src
CMOCK_PATH := $(CMOCK_SOURCE)

# Define library paths
SRC_PATH := src
TEST_PATH := tests

# Define result paths
BUILD_PATH := bin
OBJ_PATH := $(BUILD_PATH)/obj
CROSS_OBJ_PATH := $(BUILD_PATH)/cross_obj

STARTUP := stm32f0/startup_stm32f0xx.s
LINKER_SCRIPT := linker/stm32f0_linker.ld

# Define files that will get compiled
_SRC := gpio/stm32f0_gpio.c \
		clock/stm32f0_clock.c \
		rcc/stm32f0_rcc.c \
		utils/system_memory.c \
		utils/system_memory_internals.c  \
		utils/common.c \
		processor/stm32f0/peripheral.c
_CROSS_SRC := stm32f0/main.c processor/stm32f0/initialization.c stm32f0/stm32f0xx_it.c
SRC := $(patsubst %,$(SRC_PATH)/%,$(_SRC))

_TEST_SRC := gpio/test_stm32f0_gpio_creation.c \
			 gpio/test_stm32f0_gpio_usage.c \
			 clock/test_stm32f0_clock.c \
			 utils/test_system_memory.c \
			 mocks/mock_system_memory.c \
			 mocks/mock_system_memory_internals.c
_TEST_SRC += test_runner.c
TEST_SRC := $(patsubst %,$(TEST_PATH)/%,$(_TEST_SRC))

_OBJ := $(_SRC:.c=.o)
OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_OBJ))

CROSS_OBJ := $(patsubst %,$(CROSS_OBJ_PATH)/%,$(_OBJ))
_CROSS_OBJ := $(_CROSS_SRC:.c=.o)
_CROSS_OBJ += $(STARTUP:.s=.o)
CROSS_OBJ += $(patsubst %,$(CROSS_OBJ_PATH)/%,$(_CROSS_OBJ))

_TEST_OBJ := $(_TEST_SRC:.c=.o)
TEST_OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_TEST_OBJ))

_UNITY_SRC := src/unity.c extras/fixture/src/unity_fixture.c
UNITY_SRC := $(patsubst %,$(UNITY_PATH)/%,$(_UNITY_SRC))
_UNITY_OBJ := $(_UNITY_SRC:.c=.o)
UNITY_OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_UNITY_OBJ))

_CMOCK_SRC := src/cmock.c
CMOCK_SRC := $(patsubst %,$(CMOCK_PATH)/,%(_CMOCK_SRC))
_CMOCK_OBJ := $(_CMOCK_SRC:.c=.o)
CMOCK_OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_CMOCK_OBJ))

PROJECT_LIB := main_project.a
TEST_TARGET := $(BUILD_PATH)/run_test.out

CROSS_TARGET := $(BUILD_PATH)/hw_binary.bin
CROSS_HEX := $(CROSS_TARGET:.bin=.hex)
CROSS_ELF := $(CROSS_TARGET:.bin=.elf)

MAP_FILE := $(BUILD_PATH)/mapfile.map

# Commands
COMPILE:=gcc -c
LINK:=gcc
AR:=ar

# Cross compile commands
CC_TYPE:=arm-none-eabi
CC_PREFIX:=$(CC_PATH)/$(CC_TYPE)
CROSS_COMPILE:=$(CC_PREFIX)-gcc
CROSS_LINK:=$(CROSS_COMPILE)
GDBTUI = $(CC_PREFIX)-gdb
ASSEMBLE:=$(CROSS_COMPILE) -x assembler-with-cpp
OBJCOPY:=$(CC_PREFIX)-objcopy
HEX:=$(OBJCOPY) -O ihex
BIN:=$(OBJCOPY) -O binary -S
MCU:=cortex-m0

# Define compiler options
BASE_CFLAGS := -std=c99 \
			   -Wall \
		  	   -I. \
		  	   -I$(SRC_PATH)
CFLAGS := $(BASE_CFLAGS) \
		  -I$(UNITY_PATH)/src \
		  -I$(UNITY_PATH)/extras/fixture/src \
		  -I$(CMOCK_PATH)/src \
		  -I$(TEST_PATH)
CFLAGS += -g
LDFLAGS :=
ARFLAGS := r

DEFS:= -DRUN_FROM_FLASH=1
MCFLAGS:= -mcpu=$(MCU)
BASE_CROSS_FLAGS:= $(MCFLAGS) -g -gdwarf-2 -mthumb
ASSEMBLER_FLAGS:= $(BASE_CROSS_FLAGS) -Wa,-amhls=$(<:.s=.lst)
CROSS_CFLAGS := -c $(BASE_CFLAGS) $(BASE_CROSS_FLAGS) -fomit-frame-pointer
CROSS_LDFLAGS := -g -nostartfiles -T$(LINKER_SCRIPT) -Wl,-Map=$(MAP_FILE),--cref,--no-warn-mismatch

# Make commands
.PHONY: all
all: $(TEST_TARGET)

# Build cross-compiled binary
.PHONY: build
build: $(CROSS_TARGET)

.PHONY: flash
flash: $(CROSS_TARGET)
	st-flash write $(CROSS_TARGET) 0x8000000

.PHONY: erase
erase:
	st-flash erase

.PHONY: debug
debug: $(CROSS_ELF)
	xterm -e st-util &
	$(GDBTUI) --eval-command="target remote localhost:4242"  $(CROSS_ELF) -ex 'load'

.PHONY: test
test: $(TEST_TARGET)
	./$(TEST_TARGET) -v

.PHONY: clean
clean:
	rm -rf $(BUILD_PATH)

# Rules for Make

$(OBJ_PATH)/%.o: */%.c
	$(E)C Compiling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(Q)$(COMPILE) -o $@ $< $(CFLAGS)

$(OBJ_PATH)/%.o: $(UNITY_PATH)/%.c
	$(E)C Compiling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(Q)$(COMPILE) -o $@ $< $(CFLAGS)

$(OBJ_PATH)/%.o: $(CMOCK_PATH)/%.c
	$(E)C Compiling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(Q)$(COMPILE) -o $@ $< $(CFLAGS)

$(CROSS_OBJ_PATH)/%.o: */%.c
	$(E)C Cross Compiling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(Q)$(CROSS_COMPILE) -o $@ $< $(CROSS_CFLAGS)

$(CROSS_OBJ_PATH)/%.o: */%.s
	$(E)Assembling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(ASSEMBLE) -c $(ASFLAGS) $< -o $@

$(PROJECT_LIB): $(OBJ)
	$(Q)$(AR) $(ARFLAGS) $@ $^
	$(Q)ranlib $@

# $^ is shorthand for all of the dependencies
# $@ is shorthand for the target
$(TEST_TARGET): $(TEST_OBJ) $(UNITY_OBJ) $(CMOCK_OBJ) $(PROJECT_LIB)
	$(E)"Linking" $@
	$(Q)$(LINK) $(LDFLAGS) -o $@ $^

$(CROSS_TARGET): $(CROSS_ELF)
	$(Q)$(BIN) $< $@

$(CROSS_HEX): $(CROSS_ELF)
	$(Q)$(HEX) $< $@

$(CROSS_ELF): $(CROSS_OBJ)
	$(E)"Linking" $@
	$(Q)$(CROSS_LINK) $(CROSS_LDFLAGS) -o $@ $^
