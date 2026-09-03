# Makefile for Assignment 4 (K-Means Clustering + FastMap)
# Assumes this file sits in the assignment root, alongside src/ and driver/,
# with ../common_wrapper and ../utils as sibling directories (same layout
# used in Assignment 3).

CC      := gcc
CSTD    := -std=c11
WARN    := -Wall -Wextra
OPT     := -O2
CFLAGS  := $(CSTD) $(WARN) $(OPT)
LDLIBS  := -lm    # kmeans.c and fastmap.c use sqrt()/pow() from <math.h>

SRC_DIR     := src
DRIVER_DIR  := driver
WRAPPER_DIR := ../common_wrapper
BUILD_DIR   := build

# Neither K-Means nor FastMap needs CSR conversion (Section 4.1), so
# ../utils/csr_conversion.c is intentionally NOT linked in. If a later
# part of this assignment adds a graph-based algorithm, add it here:
#   UTILS_DIR := ../utils
#   INCLUDES  += -I$(UTILS_DIR)
#   SRCS      += $(UTILS_DIR)/csr_conversion.c

INCLUDES := -I$(SRC_DIR) -I$(DRIVER_DIR) -I$(WRAPPER_DIR)

SRCS := \
	$(SRC_DIR)/kmeans.c \
	$(SRC_DIR)/fastmap.c \
	$(DRIVER_DIR)/kmeans_runner.c \
	$(DRIVER_DIR)/fastmap_runner.c \
	$(DRIVER_DIR)/driver.c \
	$(WRAPPER_DIR)/wrapper.c

# All source files are uniquely named, so objects can live flat in build/.
OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
DEPS := $(OBJS:.o=.d)

vpath %.c $(SRC_DIR) $(DRIVER_DIR) $(WRAPPER_DIR)

TARGET := run

.PHONY: all clean exec

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEPS)

# Usage: make exec ARGS="tests/km_01.txt kmeans"
# (named "exec", not "run" - the built binary is itself called "run",
# and a same-named phony target would collide with it)
exec: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)