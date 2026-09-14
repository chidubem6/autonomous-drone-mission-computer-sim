/*
 * drone.h — the drone's state: everything the onboard computer knows about
 * the vehicle right now. One struct, updated every tick, read by everything.
 */
#ifndef DRONE_H
#define DRONE_H

typedef struct {
    double x_m;   /* position east of the launch point, in metres */
    double y_m;   /* position north of the launch point, in metres */
    double altitude_m; /* altitude above the launch point, in metres */
    double heading_deg; /* heading in degrees, 0 = north, 90 = east, 180 = south, 270 = west */
    double speed_mps; /* speed in metres per second */
    double battery_percent; /* battery charge remaining, in percent */
} DroneState;

#endif /* DRONE_H */
