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
#define DESCENT_RATE_MPS 5.0

/* How fast the drone descends on purpose, to reach a waypoint lower than
   where it currently is, in metres per second. Commanded, so it is slower
   and gentler than the powerless sink above. */
#define CONTROLLED_DESCENT_RATE_MPS 2.5

/* How fast the drone flies toward a waypoint, in metres per second. */
#define CRUISE_SPEED_MPS 10.0

/* How close to a waypoint counts as arrived, in metres. */
#define ARRIVAL_RADIUS_M 5


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
 * Compass heading from the drone to a waypoint, in degrees.
 * 0 = north, 90 = east — the same convention as DroneState.heading_deg.
 */
double bearing_to(const DroneState *d, const Waypoint *w) {
    double dx = w->x_m - d->x_m;   /* metres east  */
    double dy = w->y_m - d->y_m;   /* metres north */

    /* East first, north second: measures clockwise from north. Answer
       is in radians, somewhere between -pi and +pi. */
    double radians = atan2(dx, dy);

    double degrees = radians * (180.0 / M_PI);

    if (degrees < 0) {
        degrees += 360;
    }

    return degrees;
}

/*
 * Advance the drone by one tick: dt seconds of simulated time have passed,
 * so edit the state into how it will be at the end of that slice.
 */
void tick(DroneState *d, const Waypoint *target, double dt) {
    
    d->battery_percent -= DRAIN_RATE_PCT_PER_S * dt;

    if (d->battery_percent <= 0.0) {
        d->battery_percent = 0.0;
    }

    /* Navigation: point at the target and fly — but only while NAVIGATING,
       and only while there is power left. */
    if (d->battery_percent > 0.0 && d->mode == MODE_NAVIGATE) {
        double target_bearing = bearing_to(d,target);
        d->heading_deg = target_bearing;
        d->speed_mps = CRUISE_SPEED_MPS;

        /* Convert d->heading_deg into radians. This is the exact
           inverse of the conversion you wrote inside bearing_to(). */
        double heading_radians = d->heading_deg * (M_PI / 180.0);


        /* Move east. Add (speed * dt) times the east share
           to d->x_m. */
        d->x_m += (d->speed_mps * dt) * sin(heading_radians) ;

        /* Move north, the same way, into d->y_m. */
        d->y_m += d->speed_mps * dt * cos(heading_radians);

    } else {
        d->speed_mps = 0.0;
    }

    /* If battery is flat but still in the air */
    if (d->battery_percent <= 0.0 && d->altitude_m > 0.0) {
        d->altitude_m -= DESCENT_RATE_MPS * dt;

        /* Altitude cannot be less than 0 */
        if (d->altitude_m <= 0) {
            d->altitude_m = 0.0;
        }

    /* Climb at the climb rate */
    } else if (d->battery_percent > 0.0 && d->altitude_m < target->altitude_m) {
        double remaining_alt = target->altitude_m - d->altitude_m;
        double step = CLIMB_RATE_MPS * dt;

        /* Climb step cannot be larger than the remaining altitude */
        if (step > remaining_alt) {
            step = remaining_alt;
        }
        d->altitude_m += step;

    } else if (d->battery_percent > 0.0 && d->altitude_m > target->altitude_m) {
        double remaining_alt = d->altitude_m - target->altitude_m;
        double step = CONTROLLED_DESCENT_RATE_MPS * dt;

        if (step > remaining_alt) {
            step = remaining_alt;
        }
        d->altitude_m -= step;

    } else {
        /*Altitude is already where it should be - either holding at the target altitude
        or on the ground */
    }

    /* The transition rule. This is the entire state machine: read the current
       mode, look at an event, write a new mode. */
    if (d->mode == MODE_TAKEOFF && d->altitude_m >= target->altitude_m) {
        d->mode = MODE_NAVIGATE;
        printf("MODE NAVIGATE\n");
    }
}

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
