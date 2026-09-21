/*
 * drone.h — the drone's state: everything the onboard computer knows about
 * the vehicle right now. One struct, updated every tick, read by everything.
 */
#ifndef DRONE_H
#define DRONE_H

/*
 * The three things the drone can be doing. A FlightMode variable holds
 * exactly one of these names, never an unlabelled number.
 */
typedef enum {
    MODE_TAKEOFF,
    MODE_NAVIGATE,
    MODE_COMPLETE,
} FlightMode;

typedef struct {
    double x_m;   /* position east of the launch point, in metres */
    double y_m;   /* position north of the launch point, in metres */
    double altitude_m; /* altitude above the launch point, in metres */
    double heading_deg; /* heading in degrees, 0 = north, 90 = east, 180 = south, 270 = west */
    double speed_mps; /* speed in metres per second */
    double battery_percent; /* battery charge remaining, in percent */
    FlightMode mode; /* the current state a drone is in */
} DroneState;

/*
 * A single target the drone flies to. Same frame as DroneState:
 * metres east, north and up, measured from the launch point.
 */
typedef struct {
    double x_m; /* target position east of the launch point, in metres */
    double y_m; /* target position north of the launch point, in metres */
    double altitude_m; /* target altitude above the launch point, in metres */
} Waypoint;

/*
 * The engine's functions, declared for anyone who includes this header.
 * These are declarations only — the code itself stays in drone.c, and the
 * linker is what connects the two.
 */

void tick(DroneState *d, const Waypoint *target, double dt);
double bearing_to(const DroneState *d, const Waypoint *w);
double distance_to(const DroneState *d, const Waypoint *w);
void print_state(const DroneState *d);

#endif /* DRONE_H */
