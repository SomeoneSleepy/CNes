BUILD_DIR = build
INCLUDE_DIR = ./include
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)

SRC := $(wildcard src/*.c) $(wildcard src/components/*.c)
OBJ := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))

all: $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
