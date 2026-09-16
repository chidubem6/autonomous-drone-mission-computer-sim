/*
 * drone.c — the simulation engine: the drone's onboard computer.
 */
#include <stdio.h>
#include "drone.h"
#include <unistd.h>
#include <math.h>

/* How fast the drone climbs while taking off, in metres per second. */
#define CLIMB_RATE_MPS 2.0

/* The duration of each simulation tick, in seconds. */
#define TICK_S 0.05

/* Battery consumed per second, as a percentage. A full charge
   lasts about 20 minutes (1200s). */
#define DRAIN_RATE_PCT_PER_S (100.0 / 1200)

/* Descent rate once the battery is flat, in metres per second. */
#define DESCENT_RATE_MPS 1

/* The altitude the drone climbs to and then holds, in metres. */
#define CRUISE_ALTITUDE_M 100

/* How many waypoints the mission has. An array does not remember its own
   length, so the count lives here and every loop is checked against it. */
#define MISSION_WAYPOINT_COUNT 3

/* Print one line describing everything the drone knows about itself. */
void print_state(const DroneState *d) {
    printf("POS %6.1f,%6.1f   ALT %5.1f m   HDG %5.1f deg   SPD %6.1f m/s   BAT %5.1f %%\n", 
        d->x_m, d->y_m, d->altitude_m, d->heading_deg, d->speed_mps, d->battery_percent);
}

/*
 * Straight-line horizontal distance from the drone to a waypoint, in metres.
 * Altitude is ignored on purpose: steering is a two-dimensional problem.
 */
double distance_to(const DroneState *d, const Waypoint *w) {
    double dx = w->x_m - d->x_m;   /* how far east the waypoint is from the drone */
    double dy = w->y_m - d->y_m; /* how far north the waypoint is from the drone */
        
    double distance = sqrt(dx*dx + dy*dy);

    return distance;
}

/*
 * Advance the drone by one tick: dt seconds of simulated time have passed,
 * so edit the state into how it will be at the end of that slice.
 */
void tick(DroneState *d, double dt) {
    
    d->battery_percent -= DRAIN_RATE_PCT_PER_S * dt;

    if (d->battery_percent <= 0.0) {
        d->battery_percent = 0.0;
    }

    /* If battery is flat but still in the air */
    if (d->battery_percent <= 0.0 && d->altitude_m > 0.0) {
        d->altitude_m -= DESCENT_RATE_MPS * dt;

        /* Altitude cannot be less than 0 */
        if (d->altitude_m <= 0) {
            d->altitude_m = 0.0;
        }

    /* Climb at the climb rate, then clamp so one tick cannot
        carry it past the cruise altitude. */
    } else if (d->battery_percent > 0.0 && d->altitude_m < CRUISE_ALTITUDE_M) {
        d->altitude_m += CLIMB_RATE_MPS * dt;

        /* Altitude should not be aboce cruise altitude. */
        if (d->altitude_m >  CRUISE_ALTITUDE_M) {
            d->altitude_m = CRUISE_ALTITUDE_M;
        }
    } else {
        /*Altitude does not change when landed */
    }
}

int main(void) {
    DroneState drone = {
        .x_m = 0.0,
        .y_m = 0.0,
        .altitude_m = 0.0,
        .heading_deg = 0.0,
        .speed_mps = 0.0,
        .battery_percent = 100.0
    };

    /* The mission: the targets to fly to, in order. */
    Waypoint mission[MISSION_WAYPOINT_COUNT] = {
        {.x_m = 10, .y_m = 40, .altitude_m = 200},
           {.x_m = 50, .y_m = 25, .altitude_m = 400},
           {.x_m = 95, .y_m = 75, .altitude_m = 310},

    };

    /* Which waypoint we are flying to. Advancing this is task 3.5. */
    int current_wp = 0;

    printf("DRONE-01 online\n");

    /* Announce the mission before flying it. */
    printf("MISSION %d waypoints\n", MISSION_WAYPOINT_COUNT);
    for (int i = 0; i < MISSION_WAYPOINT_COUNT; i++) {
        printf("WP%d   x %5.1f   y %5.1f   alt %4.1f\n", i, mission[i].x_m, mission[i].y_m, mission[i].altitude_m);

    }

    print_state(&drone);

    while(1) {
        tick(&drone, TICK_S);
        print_state(&drone);
        printf("   -> WP%d   %6.1f m\n", current_wp, distance_to(&drone, &mission[current_wp]));
        usleep(TICK_S * 1000000);
    }

    return 0;
}
