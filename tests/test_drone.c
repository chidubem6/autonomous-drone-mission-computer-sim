/*
 * test_drone.c — claims about the engine, checked by a machine.
 *
 * Each claim is written as an assert. A true claim is silent; a false one
 * prints the expression, the file and the line, and aborts with a nonzero
 * exit code, which is what makes `make test` fail.
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/drone.h"

/* Cap on floating-point drift: decimals have no absolute binary form, so arithmetic approximates. Above the drift (~1e-16 m), below any error that matters. */
#define TOLERANCE_M 1e-9

int main(void) {
    /* Initialise a drone with 100% battery and set to 0 on every other field */
    DroneState d = {.battery_percent = 100,};

    Waypoint w = {0};

    /* "Went down" is a claim about two moments, so the first one is kept. */
    double battery_before = d.battery_percent;


    tick(&d, &w, 1.0);

    /* CLAIM: one tick of powered flight costs battery. */
    assert(battery_before > d.battery_percent);


    /* ---- CLAIM 2 ----
       Flying east at 10 m/s for one second lands the drone at x = 10. */

    DroneState d_nav_east = {.battery_percent = 100, .mode = MODE_NAVIGATE};

    Waypoint w_nav_east = {.x_m = 100};

    tick(&d_nav_east, &w_nav_east, 1.0);

    assert(fabs(d_nav_east.x_m - 10) < TOLERANCE_M);

    /* CLAIM: flying due east changes nothing about the north position. */
    assert(fabs(d_nav_east.y_m - 0) < TOLERANCE_M);

    printf("all tests passed\n");
    return 0;
}
