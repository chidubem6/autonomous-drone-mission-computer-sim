/*
 * test_drone.c — claims about the engine, checked by a machine.
 *
 * Each claim is written as an assert. A true claim is silent; a false one
 * prints the expression, the file and the line, and aborts with a nonzero
 * exit code, which is what makes `make test` fail.
 */
#include <stdio.h>
#include <assert.h>
#include "../src/drone.h"

int main(void) {
    /* TODO(you): a drone in the state your claim is actually about.
       {0} gives you a flat battery, which is the wrong situation. */
    DroneState d = {.battery_percent = 100,};

    Waypoint w = {0};

    /* TODO(you): the battery level before the tick. You cannot claim it
       went down without holding on to what it was. */
    double battery_before = d.battery_percent;


    tick(&d, &w, 1.0);

    /* TODO(you): the claim, as an assert. */
    assert(battery_before > d.battery_percent);


    printf("all tests passed\n");
    return 0;
}
