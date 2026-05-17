CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
ARFLAGS = rcs

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
LIB_DIR = lib

SRCS = $(SRC_DIR)/laba12.c $(SRC_DIR)/main.c
OBJS = $(BUILD_DIR)/laba12.o $(BUILD_DIR)/main.o

LIB_NAME = liblaba12.a
TARGET = program

all: $(LIB_DIR)/$(LIB_NAME) $(TARGET)

$(LIB_DIR)/$(LIB_NAME): $(BUILD_DIR)/laba12.o    папка с библиотекой и имя библиотеки(lib/liblaba12.a: build/laba12.o)
	mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(BUILD_DIR)/main.o $(LIB_DIR)/$(LIB_NAME)
	$(CC) $(BUILD_DIR)/main.o -L$(LIB_DIR) -llaba12 -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(TARGET)

.PHONY: all clean