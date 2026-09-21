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
    /* Initialise a drone with 100% battery and set to 0 on every other field */
    DroneState d = {.battery_percent = 100,};

    Waypoint w = {0};

    /* "Went down" is a claim about two moments, so the first one is kept. */
    double battery_before = d.battery_percent;


    tick(&d, &w, 1.0);

    /* CLAIM: one tick of powered flight costs battery. */
    assert(battery_before > d.battery_percent);


    printf("all tests passed\n");
    return 0;
}
