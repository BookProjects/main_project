#Verbosity flags (Q=quiet, E=echo)
ifdef VERBOSE
	Q =
	E = @true
else
	Q = @
	E = @echo 
endif

# Define Unity paths for testing
UNITY_PATH := $(UNITY_SOURCE)
UNITY_EXTRA_PATH := $(UNITY_PATH)/extras/fixture/src

# Define library paths
SRC_PATH := src
TEST_PATH := tests

# Define result paths
BUILD_PATH := bin
OBJ_PATH := $(BUILD_PATH)/obj

# Define files that will get compiled
_SRC := gpio/stm32f0_gpio.c utils/system_memory.c
SRC := $(patsubst %,$(SRC_PATH)/%,$(_SRC))
_TEST_SRC := gpio/test_stm32f0_gpio.c helpers/mock_system_memory.c
_TEST_SRC += test_runner.c
TEST_SRC := $(patsubst %,$(TEST_PATH)/%,$(_TEST_SRC))

_OBJ := $(_SRC:.c=.o)
OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_OBJ))
_TEST_OBJ := $(_TEST_SRC:.c=.o)
TEST_OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_TEST_OBJ))

_UNITY_SRC := src/unity.c extras/fixture/src/unity_fixture.c
SRC := $(patsubst %,$(UNITY_PATH)/%,$(_UNITY_SRC))
_UNITY_OBJ := $(_UNITY_SRC:.c=.o)
UNITY_OBJ := $(patsubst %,$(OBJ_PATH)/%,$(_UNITY_OBJ))

PROJECT_LIB := main_project.a
TEST_TARGET := $(BUILD_PATH)/run_test.out

# Define compiler options
CFLAGS := -std=c99 \
		  -Wall \
		  -I. \
		  -I$(UNITY_PATH)/src \
		  -I$(UNITY_PATH)/extras/fixture/src \
		  -I$(SRC_PATH) \
		  -DUNITY_FIXTURES

# Commands
COMPILE:=gcc -c
LINK:=gcc

# Make commands
.PHONY: all
all: $(TEST_TARGET)

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
	$(Q)$(COMPILE) -o $@ $< $(CFLAGS) -DTESTING

$(OBJ_PATH)/%.o: $(UNITY_PATH)/%.c
	$(E)C Compiling $< to $@
	$(Q)mkdir -p `dirname $@`
	$(Q)$(COMPILE) -o $@ $< $(CFLAGS)

$(PROJECT_LIB): $(OBJ)
	$(Q)$(AR) $(ARFLAGS) $@ $^
	ranlib $@

# $^ is shorthand for all of the dependencies
# $@ is shorthand for the target
$(TEST_TARGET): $(TEST_OBJ) $(UNITY_OBJ) $(PROJECT_LIB)
	$(E)$(UNITY_OBJ)
	$(Q)$(LINK) -o $@ $^
