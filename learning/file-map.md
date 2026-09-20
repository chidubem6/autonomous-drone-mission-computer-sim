# File map

<!-- Every file/folder is either explained or parked — no mystery boxes. -->
<!-- known: explained in the learner's own words | parked: honest one-liner for now, deep dive scheduled | generated: machine-made, never edit, always rebuildable -->

## /
- learning/project.md — known (2026-09-13) — the project, its MVP, and the trunk of core components
- learning/plan.md — known (2026-09-13) — the locked stack decisions and the nine build sections → [[writing-a-plan]]
- learning/knowledge-graph.md — known (2026-09-13) — the living map of what is actually understood, updated every lesson
- learning/file-map.md — known (2026-09-13) — this file: why every file in the repo exists
- learning/environment.md — known (2026-09-14) — records that builds happen in WSL (Ubuntu), so every lesson dictates commands in the right dialect
- .gitignore — known (2026-09-14) — which machine-produced files git must never track → [[gitignore]]
- .gitattributes — known (2026-09-14) — forces LF line endings repo-wide so the Linux build never meets a carriage return → [[line-endings-lf-crlf]]
- Makefile — known (2026-09-16) — the build recipe: how to turn src/ into the drone executable, plus run and clean; compiles with -std=gnu11 because the engine calls POSIX usleep, and links -lm because the engine calls sqrt → [[makefile]], [[make-targets]], [[c-standard-vs-posix]], [[linking-libraries]]
- .git/ — generated — git's own storage: every commit and every past version of every file lives here, never edited by hand → [[git-repository]]
- src/ — known (2026-09-14) — the C engine's source: the drone's onboard computer → [[project-structure]]
- src/drone.c — known (2026-09-15) — the engine itself: holds one DroneState, advances it one time-slice at a time with tick(), loops forever in main() at 20 Hz, and renders each state as one telemetry line → [[pointers]], [[printf-format]], [[simulation-tick]], [[delta-time]], [[named-constants]], [[main-loop]], [[sleep-and-timing]], [[fixed-timestep]], [[battery-model]], [[clamping]], [[conditionals]], [[implicit-state]], [[arrays-of-structs]], [[waypoint-list]], [[for-loop]], [[array-length-is-not-stored]], [[vectors-and-distance]], [[address-of-array-element]], [[heading-and-direction]], [[atan2-and-quadrants]], [[angle-normalisation]], [[bearing-vs-heading]], [[trig-components]], [[passing-dependencies-as-parameters]]; since 2026-09-16 it also holds the mission — an array of three Waypoints announced at startup — plus distance_to() and bearing_to(), which report how far and which way to the current waypoint every tick; since 2026-09-17 tick() also takes the target waypoint, so the drone steers onto the bearing, moves along it at cruise speed, and climbs to that waypoint's own altitude; since 2026-09-20 it also flies the mission to its end — an arrival radius decides when a waypoint counts as reached, current_wp advances, each arrival is announced, and the loop breaks with MISSION COMPLETE rather than indexing past the array → [[arrival-threshold]], [[loop-control-break]], [[off-by-one-errors]], [[undefined-behaviour]], [[truthiness-in-c]]
- src/drone.h — known (2026-09-16) — the two shapes the engine is built from: DroneState (where the drone IS) and Waypoint (where it is MEANT to go), shared with every file that needs them → [[struct]], [[include-guards]], [[comments-that-lie]]
- drone — generated — the compiled engine, rebuilt from src/ by gcc and never committed → [[compiling-c]]

<!-- Nothing else exists yet. Section 1 adds the git repository, the Makefile,
     and the first engine source files; every one of them gets a line here the
     day it appears. -->
