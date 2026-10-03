# Telemetry contract

The agreement between the engine (`build/drone`, written in C) and anything
that reads its output (the server, from section 6, written in JavaScript).
Neither program can see the other's code. This document is the only thing
they share, so if the output and this file disagree, the output is wrong.

## One tick = one line

Every tick, the mission program prints one JSON object on one line to stdout.

| field | type | unit / allowed values | meaning |
|---|---|---|---|
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

## What a reader can rely on

A reader is any program consuming these lines: the server, or the tests.

- Every line ends with a newline. The newline is what marks the end of a message.
- Every line carries every field in the table.

## Example

```json
{"mission_time_s": 3.00, "flight_mode": "TAKEOFF", "position_x_m": 0.0, "position_y_m": 0.0, "altitude_m": 6.0, "heading_deg": 0.0, "horizontal_speed_mps": 0.0, "battery_pct": 99.75, "current_waypoint": 0, "distance_from_waypoint_m": 41.23, "bearing_to_waypoint_deg": 14.0}
```

## Not covered yet

Waypoint arrivals, mode changes and mission end still print as plain text.
Task 5.4 brings them into this contract.
