/*
 * main.c — the mission program: the decisions.
 *
 * This file owns a specific drone, a specific mission, and the choice to
 * fly it at 20 Hz in real time. None of that is a fact about drones — the
 * engine in drone.c knows how a drone behaves and nothing about missions.
 */
#include <stdio.h>
#include <unistd.h>
#include "drone.h"

/* The duration of each simulation tick, in seconds. */
#define TICK_S 0.05

/* How close to a waypoint counts as arrived, in metres. */
#define ARRIVAL_RADIUS_M 5

/* How many waypoints the mission has. An array does not remember its own
   length, so the count lives here and every loop is checked against it. */
#define MISSION_WAYPOINT_COUNT 3

int main(void) {
    DroneState drone = {
        .x_m = 0.0,
        .y_m = 0.0,
        .altitude_m = 0.0,
        .heading_deg = 0.0,
        .speed_mps = 0.0,
        .battery_percent = 100.0,
        .mode = MODE_TAKEOFF,
    };

    /* The mission: the targets to fly to, in order. */
    Waypoint mission[MISSION_WAYPOINT_COUNT] = {
        {.x_m = 10, .y_m = 40, .altitude_m = 12},
           {.x_m = 50, .y_m = 25, .altitude_m = 20},
           {.x_m = 95, .y_m = 75, .altitude_m = 8},

    };

    int current_wp = 0;

    printf("DRONE-01 online\n");

    /* Announce the mission before flying it. */
    printf("MISSION %d WAYPOINTS\n", MISSION_WAYPOINT_COUNT);
    for (int i = 0; i < MISSION_WAYPOINT_COUNT; i++) {
        printf("WP%d   x %5.1f   y %5.1f   alt %4.1f\n", i, mission[i].x_m, mission[i].y_m, mission[i].altitude_m);

    }

    print_state(&drone);

    while(drone.mode != MODE_COMPLETE ) {
        tick(&drone, &mission[current_wp], TICK_S);
        print_state(&drone);
        printf("   -> WP%d   %6.1f m   BRG %5.1f deg\n", current_wp, distance_to(&drone, &mission[current_wp]), bearing_to(&drone, &mission[current_wp]));

        /* Arrival: close enough to call this waypoint reached? */
        if (distance_to(&drone, &mission[current_wp]) <= ARRIVAL_RADIUS_M) {
            printf("REACHED WP%d\n", current_wp);

            current_wp++;

            if (current_wp == MISSION_WAYPOINT_COUNT) {
                printf("MISSION COMPLETE\n");
                drone.mode = MODE_COMPLETE;
            } 

        }

        usleep(TICK_S * 1000000);
    }

    return 0;
}
