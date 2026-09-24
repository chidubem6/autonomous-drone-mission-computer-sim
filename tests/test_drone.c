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

/* ---- CLAIM 1 ----
One tick of powered flight costs battery. */
void test_tick_drains_battery (void) {
    DroneState d = {.battery_percent = 100,};

    Waypoint w = {0};

    /* "Went down" is a claim about two moments, so the first one is kept. */
    double battery_before = d.battery_percent;


    tick(&d, &w, 1.0);

    /* CLAIM: one tick of powered flight costs battery. */
    assert(battery_before > d.battery_percent);
}

void test_drone_lands_at_x10 (void) {
    /* ---- CLAIM 2 ----
   Flying east at 10 m/s for one second lands the drone at x = 10. */

    DroneState d = {.battery_percent = 100, .mode = MODE_NAVIGATE};

    Waypoint w = {.x_m = 100};

    tick(&d, &w, 1.0);

    assert(fabs(d.x_m - 10) < TOLERANCE_M);

    /* CLAIM: flying due east changes nothing about the north position. */
    assert(fabs(d.y_m - 0) < TOLERANCE_M);
}

/* ---- CLAIM 3 ----
   A dead drone in the air, with battery exactly 0, sinks */
void test_dead_drone_sinks(void) {
    DroneState d = {.battery_percent = 0, .altitude_m = 10};
    Waypoint w = {.altitude_m = 500};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_before > alt_after);
}

/* ---- CLAIM 4 ----
   A healthy drone below its target altitude climbs. */
void test_drone_below_target_climbs(void) {
    DroneState d = {.battery_percent = 100};
    Waypoint w = {.altitude_m = 100};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_after > alt_before);
}

/* ---- CLAIM 5 ----
   A healthy drone above its target altitude descends. */
void test_drone_above_target_descends(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100};
    Waypoint w = {.altitude_m = 50};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_after < alt_before);
}

/* ---- CLAIM 6 ----
   A healthy drone at exactly its target altitude stays exactly there. */
void test_drone_at_target_holds_altitude(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100};
    Waypoint w = {.altitude_m = 100};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_after == alt_before);
}

/* ---- CLAIM 7 ----
   A healthy drone at less than one step below its target lands on it exactly. */
void test_climb_lands_exactly_on_target(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 99};
    Waypoint w = {.altitude_m = 100};

    double target_waypoint_alt = w.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_after == target_waypoint_alt);
    assert(d.mode == MODE_NAVIGATE);
}

/* ---- CLAIM 8 ----
   A healthy drone at less than one step above its target lands on it exactly. */
void test_descent_lands_exactly_on_target(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100};
    Waypoint w = {.altitude_m = 99};

    double target_waypoint_alt = w.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    assert(alt_after == target_waypoint_alt);
    assert(d.mode == MODE_NAVIGATE);
}

int main(void) {
    test_tick_drains_battery();
    test_drone_lands_at_x10();
    test_dead_drone_sinks();
    test_drone_below_target_climbs();
    test_drone_above_target_descends();
    test_drone_at_target_holds_altitude();
    test_climb_lands_exactly_on_target();
    test_descent_lands_exactly_on_target();

    printf("all tests passed\n");
    return 0;
}
