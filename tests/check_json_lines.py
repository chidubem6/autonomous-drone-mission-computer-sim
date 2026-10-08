"""
check_json_lines.py: reads a stream on stdin and checks that every line
parses as JSON and matches the contract, on its own. Usage:  ./build/drone | python3 tests/check_json_lines.py
"""
import sys
import json

EVENTS = {
    "tick": {
        "mission_time_s": float,
        "flight_mode": str,
        "position_x_m": float,
        "position_y_m": float,
        "altitude_m": float,
        "heading_deg": float,
        "horizontal_speed_mps": float,
        "battery_pct": float,
        "current_waypoint": int,
        "distance_from_waypoint_m": float,
        "bearing_to_waypoint_deg": float,
    },
    "drone_activated": {"mission_time_s": float, "drone_name": str},
    "mission_init": {"mission_time_s": float, "waypoints": list},
    "mode_change": {"mission_time_s": float, "previous_mode": str, "new_mode": str},
    "waypoint_reached": {"mission_time_s": float, "waypoint": int},
    "mission_complete": {"mission_time_s": float},
}

WAYPOINT_FIELDS = {"x_m": float, "y_m": float, "altitude_m": float}



problems = []

number = 0
for number, line in enumerate(sys.stdin, start=1):
    try:
        
        message = json.loads(line)
        """
        for each message, we check for the event.

        if line.event mataches the event, then check the same fields match the contract fields
        for each item of the message.event, check if it matches the intended value of the event

        if fields mtach, then check each field has the correct type
        """
        
        if "event" not in message:
            problems.append([number, "Invalid format. No event key"])
            continue

        
        event = message["event"]

        if event not in EVENTS:
            problems.append([number, f"{event} does not exist in contract"])
            continue

        for field, expected_type in EVENTS[event].items():

            if field not in message:
                problems.append([number, f"{field} does not exist"])

                
            elif not isinstance(message[field], expected_type):
                problems.append([number, f"{field} has invalid type"])

        if message["event"] == "mission_init":
            if "waypoints" not in message:
                problems.append([number, "No waypoint field"])
                continue

            if not isinstance(message["waypoints"], list):
                problems.append([number, "Waypoints field not a list"])
                continue
                


            for waypoint in message["waypoints"]:
                for field, expected_type in WAYPOINT_FIELDS.items():
                    if field not in waypoint:
                        problems.append([number, f"{field} does not exist"])

                    elif not isinstance(waypoint[field], expected_type):
                        problems.append([number, f"{field} has invalid type"])



        


    except json.JSONDecodeError as err:
        problems.append([number, f"not JSON: {err}"])

if problems:
    for line_number, reason in problems:
        print(f"line {line_number}: {reason}")

    sys.exit(1)
elif number == 0:
    print("Empty stream")
    sys.exit(1)
else:
    print(f"all {number} lines match the contract")
    sys.exit(0)
