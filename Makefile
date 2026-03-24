CC = gcc
CFLAGS = -Wall -Wextra -g

BUILD_DIR = build
ifeq ($(OS),Windows_NT)
TARGET = $(BUILD_DIR)/run.exe
else
TARGET = $(BUILD_DIR)/run
endif
INPUT = test.txt

SRCS = main.c lexer.c parser.c
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

dev: $(TARGET)
	.\$(TARGET) $(INPUT)

ifeq ($(OS),Windows_NT)
clean:
	-del /Q build\*.o
	-del /Q build\run.exe
else
clean:
	-rm -f build/*.o build/run.exe
endif