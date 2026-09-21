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

# TODO(you): the test program. Same shape as the drone rule above — real
#            target path, mkdir, gcc. Built from the engine and the test file.
$(BUILD)/test_drone: src/drone.c src/drone.h tests/test_drone.c
	mkdir -p $(BUILD)
	gcc -Wall -Wextra -Werror -std=gnu11 -o $(BUILD)/test_drone src/drone.c tests/test_drone.c -lm

# TODO(you): `test` stays a phony action, so it needs a prerequisite that IS a
#            real file (the rule above) and a recipe that RUNS the binary.
#            Model it on `run`.
test: $(BUILD)/test_drone
	./$(BUILD)/test_drone


# TODO(you): one line, and it can never go stale again.
clean:
	rm -rf $(BUILD)

.PHONY: run clean test
