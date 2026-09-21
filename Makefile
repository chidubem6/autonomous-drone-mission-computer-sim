# Makefile — how to build the drone engine.
#
# Run `make` in this folder and it builds whatever is out of date.

# A variable. Everywhere $(BUILD) appears below, make substitutes "build".
# Written once, so changing the directory is a one-line edit.
BUILD := build

# The drone program: the engine plus the mission that drives it.
# gcc will not create a missing directory, so mkdir -p comes first.
$(BUILD)/drone: src/drone.c src/main.c src/drone.h
	mkdir -p $(BUILD)
	gcc -Wall -Wextra -Werror -std=gnu11 -o $(BUILD)/drone src/drone.c src/main.c -lm

run: $(BUILD)/drone
	./$(BUILD)/drone


$(BUILD)/test_drone: src/drone.c src/drone.h tests/test_drone.c
	mkdir -p $(BUILD)
	gcc -Wall -Wextra -Werror -std=gnu11 -o $(BUILD)/test_drone src/drone.c tests/test_drone.c -lm


test: $(BUILD)/test_drone
	./$(BUILD)/test_drone


clean:
	rm -rf $(BUILD)

.PHONY: run clean test
