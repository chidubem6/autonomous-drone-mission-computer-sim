/*
 * drone.c — the simulation engine: the drone's onboard computer.
 */
#include <stdio.h>
#include "drone.h"

/* Print one line describing everything the drone knows about itself. */
void print_state(const DroneState *d) {
    printf("POS %6.1f,%6.1f   ALT %5.1f m   HDG %5.1f deg   SPD %6.1f m/s   BAT %5.1f %%\n", 
        d->x_m, d->y_m, d->altitude_m, d->heading_deg, d->speed_mps, d->battery_percent);
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

    printf("DRONE-01 online\n");

    print_state(&drone);

    return 0;
}
