CC       := gcc
STD      := -std=c11

INCLUDE  := -Iinclude

WARNINGS := \
	-Werror \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wconversion \
	-Wsign-conversion \
	-Wcast-qual \
	-Wwrite-strings \
	-Wformat=2 \
	-Wundef \
	-Wstrict-prototypes \
	-Wold-style-definition \
	-Wimplicit-fallthrough \
	-Wlogical-op \
	-Wcast-align \
	-Wvla \
	-Wnull-dereference \
	-Wdouble-promotion \
	-Wformat-overflow=2 \
	-Wformat-truncation=2 \
	-Walloc-zero \
	-Warray-bounds=2 \
	-Wstringop-overflow=4 \
	-Wstrict-overflow=5 \
	-Wswitch-enum \
	-Wpointer-arith \
	-Winit-self

CFLAGS   := $(STD) $(INCLUDE) $(WARNINGS) -MMD -MP

SRC_DIR  := src
BUILD_DIR := build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/mathmania

SRCS     := $(shell find $(SRC_DIR) -name '*.c')
OBJS     := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)
