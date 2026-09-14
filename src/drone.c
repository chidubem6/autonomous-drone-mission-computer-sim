/*
 * drone.c — the simulation engine: the drone's onboard computer.
 */
#include <stdio.h>
#include "drone.h"

int main(void) {
    DroneState drone = {
        .x_m = 0.0,
        .y_m = 0.0,
        .altitude_m = 0.0,
        .heading_deg = 0.0,
        .speed_mps = 0.0,
        .battery_percent = 100.0
    };

    printf("DRONE-01 online\n");

    printf("%.1f\n", drone.battery_percent);

    return 0;
}
