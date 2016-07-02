# Define Unity paths for testing
UNITY_PATH := $(UNITY_SOURCE)
UNITY_CORE_PATH := $(UNITY_PATH)/src
UNITY_EXTRA_PATH := $(UNITY_PATH)/extras/fixture/src

# Define library paths
SRC_PATH := src
TEST_PATH := tests

# Define files that will get compiled
SRC := $(SRC_PATH)/gpio/stm32f0_gpio.c
TEST_SRC := $(TEST_PATH)/gpio/test_stm32f0_gpio.c
TEST_SRC += $(TEST_PATH)/test_runner.c

UNITY_SRC := $(UNITY_CORE_PATH)/unity.c $(UNITY_EXTRA_PATH)/unity_fixture.c

TEST_TARGET := run_test.out

# Define compiler options
CFLAGS := -std=c99 \
		  -Wall \
		  -I. \
		  -I$(UNITY_CORE_PATH) \
		  -I$(UNITY_EXTRA_PATH) \
		  -I$(SRC_PATH) \
		  -DUNITY_FIXTURES

# Commands
COMPILE:=gcc

# Make commands
.PHONY: all
all: $(TEST_TARGET)

.PHONY: test
test: $(TEST_TARGET)
	./$(TEST_TARGET) -v

.PHONY: clean
clean:
	rm -rf $(TEST_TARGET)

# Rules for Make

# $^ is shorthand for all of the dependencies
# $@ is shorthand for the target
$(TEST_TARGET): $(SRC) $(TEST_SRC) $(UNITY_SRC)
	echo $(TEST_SRC)
	$(COMPILE) $(CFLAGS) -o $@ $^
