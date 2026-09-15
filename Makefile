# Makefile — how to build the drone engine.
#
# Run `make` in this folder and it builds whatever is out of date.

# The engine binary. Rebuilt whenever either source file is newer than it.
drone: src/drone.c src/drone.h
	gcc -Wall -Wextra -Werror -std=gnu11 -o drone src/drone.c

run: drone
	./drone
	
clean:
	rm -f drone
	

.PHONY: run clean
