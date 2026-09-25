/*
 * test_drone.c — claims about the engine, checked by a machine.
 *
 * Each check prints PASS or FAIL and the run carries on, so every claim
 * reports on every run. main() counts the failures and returns nonzero if
 * there were any, which is what makes `make test` fail.
 */
#include <stdio.h>
#include <math.h>
#include "../src/drone.h"

/* Cap on floating-point drift: decimals have no absolute binary form, so arithmetic approximates. Above the drift (~1e-16 m), below any error that matters. */
#define TOLERANCE_M 1e-9

int check(int ok, const char *description) {
    if (!ok) {
        printf("%-90s FAIL\n", description);
        return 1;
    } else {
        printf("%-90s PASS\n", description);
        return 0;
    }

}

/* ---- CLAIM 1 ----
One tick of powered flight costs battery. */
int test_tick_drains_battery (void) {
    DroneState d = {.battery_percent = 100, .mode = MODE_TAKEOFF};

    Waypoint w = {0};

    /* "Went down" is a claim about two moments, so the first one is kept. */
    double battery_before = d.battery_percent;


    tick(&d, &w, 1.0);

    int failed = 0;

    /* CLAIM: one tick of powered flight costs battery. */
    failed += check(battery_before > d.battery_percent, "One tick of powered flight costs battery.");

    return failed;
}

int test_drone_lands_at_x10 (void) {
    /* ---- CLAIM 2 ----
   Flying east at 10 m/s for one second lands the drone at x = 10. */

    DroneState d = {.battery_percent = 100, .mode = MODE_NAVIGATE};

    Waypoint w = {.x_m = 100};

    tick(&d, &w, 1.0);

    int failed = 0;

    failed += check(fabs(d.x_m - 10) < TOLERANCE_M, "Flying east at 10 m/s for one second lands the drone at x = 10.");

    /* CLAIM: flying due east changes nothing about the north position. */
    failed += check(fabs(d.y_m - 0) < TOLERANCE_M, "Flying due east changes nothing about the north position.");

    return failed;
}

/* ---- CLAIM 3 ----
   A dead drone in the air, with battery exactly 0, sinks */
int test_dead_drone_sinks(void) {
    DroneState d = {.battery_percent = 0, .altitude_m = 10, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 500};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_before > alt_after, "A dead drone in the air, with battery exactly 0, sinks.");

    return failed;
}

/* ---- CLAIM 4 ----
   A healthy drone below its target altitude climbs. */
int test_drone_below_target_climbs(void) {
    DroneState d = {.battery_percent = 100, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 100};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_after > alt_before, "A healthy drone below its target altitude climbs.");

    return failed;
}

/* ---- CLAIM 5 ----
   A healthy drone above its target altitude descends. */
int test_drone_above_target_descends(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 50};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_after < alt_before, "A healthy drone above its target altitude descends.");

    return failed;
}

/* ---- CLAIM 6 ----
   A healthy drone at exactly its target altitude stays exactly there. */
int test_drone_at_target_holds_altitude(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 100};

    double alt_before = d.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_after == alt_before, "A healthy drone at exactly its target altitude stays exactly there.");

    return failed;
}

/* ---- CLAIM 7 ----
   A healthy drone at less than one step below its target lands on it exactly. */
int test_climb_lands_exactly_on_target(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 99, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 100};

    double target_waypoint_alt = w.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_after == target_waypoint_alt, "A healthy drone at less than one step below its target lands on it exactly." );
    failed += check(d.mode == MODE_NAVIGATE, "A climb that lands on target transitions to MODE_NAVIGATE.");

    return failed;


}

/* ---- CLAIM 8 ----
   A healthy drone at less than one step above its target lands on it exactly. */
int test_descent_lands_exactly_on_target(void) {
    DroneState d = {.battery_percent = 100, .altitude_m = 100, .mode = MODE_TAKEOFF};
    Waypoint w = {.altitude_m = 99};

    double target_waypoint_alt = w.altitude_m;

    tick(&d, &w, 1.0);

    double alt_after = d.altitude_m;

    int failed = 0;

    failed += check(alt_after == target_waypoint_alt, "A healthy drone at less than one step above its target lands on it exactly.");
    failed += check(d.mode == MODE_NAVIGATE, "A descent that lands on target transitions to MODE_NAVIGATE.");

    return failed;
}

/* ---- CLAIM 9 ----
   A drone that is navigating when its battery runs out ends the tick in MODE_FAILSAFE */
int test_dead_navigate_drone_transitions_to_mode_failsafe(void) {
    DroneState d = {.battery_percent = 0, .mode = MODE_NAVIGATE, .altitude_m = 100};
    Waypoint w = {0};

    tick(&d, &w, 1.0);

    int failed = 0;

    failed += check(d.mode == MODE_FAILSAFE, "A drone that is navigating when its battery runs out ends the tick in MODE_FAILSAFE.");

    return failed;
}

/* ---- CLAIM 10 ----
   A drone that is in takeoff when its battery runs out ends the tick in MODE_FAILSAFE */
int test_dead_takeoff_drone_transitions_to_mode_failsafe(void) {
    DroneState d = {.battery_percent = 0, .mode = MODE_TAKEOFF, .altitude_m = 1};
    Waypoint w = {.altitude_m = 100};

    tick(&d, &w, 1.0);

    int failed = 0;

    failed += check(d.mode == MODE_FAILSAFE, "A drone that is in takeoff when its battery runs out ends the tick in MODE_FAILSAFE.");

    return failed;
}

int main(void) {
    int failures = 0;
    failures += test_tick_drains_battery();
    failures += test_drone_lands_at_x10();
    failures += test_dead_drone_sinks();
    failures += test_drone_below_target_climbs();
    failures += test_drone_above_target_descends();
    failures += test_drone_at_target_holds_altitude();
    failures += test_climb_lands_exactly_on_target();
    failures += test_descent_lands_exactly_on_target();
    failures += test_dead_navigate_drone_transitions_to_mode_failsafe();
    failures += test_dead_takeoff_drone_transitions_to_mode_failsafe();

    if (failures) {
        return 1;
    } else {
        printf("all tests passed\n");
        return 0;
    }
}
