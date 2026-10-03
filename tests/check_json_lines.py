"""
check_json_lines.py: reads a stream on stdin and checks that every line
parses as JSON, on its own. Usage:  ./build/drone | python3 tests/check_json_lines.py
"""
import sys
import json

fail_count = 0

for number, line in enumerate(sys.stdin, start=1):
    try:
        json.loads(line)
    except json.JSONDecodeError as err:
        print(f"{number} {line} ERROR {err}")
        fail_count = fail_count + 1

if fail_count:
    sys.exit(1)
else:
    print(f"all {number} lines parse")
    sys.exit(0)
