# Telemetry contract

The agreement between the engine (`build/drone`, written in C) and anything
that reads its output (the server, from section 6, written in JavaScript).
Neither program can see the other's code. This document is the only thing
they share, so if the output and this file disagree, the output is wrong.

## One tick = one line

Every tick, the mission program prints one JSON object on one line to stdout.

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "tick" | the type of event |
| mission_time_s | number | seconds | mission time since start; goes up by 0.05 every tick |
| flight_mode | string | `"TAKEOFF"` `"NAVIGATE"` `"COMPLETE"` `"FAILSAFE"` | what the drone is doing |
| position_x_m | number | metres | position east of the launch point |
| position_y_m | number | metres | position north of the launch point |
| altitude_m | number | metres | altitude above the launch point |
| heading_deg | number | degrees | heading; 0 = north, 90 = east, 180 = south, 270 = west |
| horizontal_speed_mps | number | metres per second. horizontal only; 0 while climbing straight up | speed |
| battery_pct | number | percent | battery charge remaining, 0 to 100 |
| current_waypoint | number | index | which waypoint the drone is flying to; the first is 0 |
| distance_from_waypoint_m | number | metres | horizontal straight-line distance to that waypoint, ignoring altitude |
| bearing_to_waypoint_deg | number | degrees | direction from the drone to that waypoint; same convention as heading_deg |

## Events

### drone_activated

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "drone_activated" | the type of event |
| mission_time_s | number | seconds | mission time since start |
| drone_name | string | text | the drone's name |

Shown in the event log as: `0.00s DRONE-01 online`

```json
{"event": "drone_activated", "mission_time_s": 0.00, "drone_name": "DRONE-01"}
```

### mission_init

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "mission_init" | the type of event |
| mission_time_s | number | seconds | mission time since start |
| waypoints | array of objects | `[{x_m, y_m, altitude_m}]`, all metres | the mission's waypoints in flying order; index 0 is the first |

Shown in the event log as: `0.00s mission loaded 3 waypoints`

```json
{"event": "mission_init", "mission_time_s": 0.00, "waypoints": [{"x_m": 10.0, "y_m": 40.0, "altitude_m": 12.0}, {"x_m": 50.0, "y_m": 25.0, "altitude_m": 20.0}, {"x_m": 95.0, "y_m": 75.0, "altitude_m": 8.0}]}
```

### mode_change

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "mode_change" | the type of event |
| mission_time_s | number | seconds | mission time since start |
| previous_mode | string | `"TAKEOFF"` `"NAVIGATE"` `"COMPLETE"` `"FAILSAFE"` | the mode the drone is leaving |
| new_mode | string | `"TAKEOFF"` `"NAVIGATE"` `"COMPLETE"` `"FAILSAFE"` | the mode the drone is entering |

Shown in the event log as: `6.00s TAKEOFF -> NAVIGATE`

```json
{"event": "mode_change", "mission_time_s": 6.00, "previous_mode": "TAKEOFF", "new_mode": "NAVIGATE"}
```

### waypoint_reached

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "waypoint_reached" | the type of event |
| mission_time_s | number | seconds | mission time since start |
| waypoint | number | index | which waypoint was reached; the first is 0 |

Shown in the event log as: `12.35s Reached waypoint 0`

```json
{"event": "waypoint_reached", "mission_time_s": 12.35, "waypoint": 0}
```

### mission_complete

| field | type | unit / allowed values | meaning |
|---|---|---|---|
| event | string | "mission_complete" | the type of event |
| mission_time_s | number | seconds | mission time since start |

Shown in the event log as: `33.40s Mission complete`

```json
{"event": "mission_complete", "mission_time_s": 33.40}
```

## What a reader can rely on

A reader is any program consuming these lines: the server, or the tests.

- Every line ends with a newline. The newline is what marks the end of a message.
- Every line has an event field and the fields for that event

## Example

```json
{"event": "tick", "mission_time_s": 3.00, "flight_mode": "TAKEOFF", "position_x_m": 0.0, "position_y_m": 0.0, "altitude_m": 6.0, "heading_deg": 0.0, "horizontal_speed_mps": 0.0, "battery_pct": 99.75, "current_waypoint": 0, "distance_from_waypoint_m": 41.23, "bearing_to_waypoint_deg": 14.0}
```
