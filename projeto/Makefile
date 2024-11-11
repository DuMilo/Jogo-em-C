CC = gcc
CFLAGS = -Wall -g -I./include

SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/screen.c $(SRC_DIR)/timer.c $(SRC_DIR)/keyboard.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/Bichinho

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
