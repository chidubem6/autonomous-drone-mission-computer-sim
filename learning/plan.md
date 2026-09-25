# Learning plan: Drone Mission Computer

## Locked decisions
- **Engine language**: C — reinforces the pointers/memory work already in progress, and a hand-written simulation engine is the specific evidence this project exists to produce. C/C++ is also the filter for robotics and defence work (PX4, ArduPilot, ROS 2); Python is table stakes and can be picked up later.
- **Architecture**: two separate programs — a C engine that prints state, and a server that reads it. The contract between components becomes visible text, each piece is debuggable alone, and it mirrors how real flight software and ground stations are actually split.
- **Server language**: JavaScript on Node.js — browsers only run JavaScript, so the entire ground station (server + dashboard) stays in one language. C on the vehicle, JavaScript on the ground.
- **Server → browser transport**: WebSockets — the data is a stream, not an answer, and the channel for v2's command uplink comes free.
- **Dashboard**: plain HTML, CSS and JavaScript — the dashboard is a dozen numbers written into a dozen slots; a build toolchain would be the only mystery box in the repo. React is a deliberate v2 exercise, done once the map view and controls make the pain real.
- **Database**: none in the MVP — nothing in the MVP needs remembering. It arrives with the v2 flight recorder, which is a real reason to persist data.
- **Deployment**: Docker, deployed to Render from GitHub — the host has to compile C, not just run JavaScript, so the build must be explicit. Push a commit, it rebuilds. (Free tier sleeps when idle; first visitor waits ~30s and the mission restarts.)
- **Testing**: hand-written `assert` test programs run by `make test` — zero dependencies, and it teaches the underlying idea: a test is a claim about behaviour, written down so a machine can check it.

## Sections

### 1. A drone that exists  [x] complete
**Deliverable:** `make` builds the project, and running it prints one line of drone state to the terminal.
**Concepts:** git-repository, git-commit, makefile, compiling-c, struct, header-files, printf-format
**Tasks:**
- [x] 1.1 Initialize the git repository and commit the learning files
- [x] 1.2 A first C file that compiles and prints, built by hand with gcc
- [x] 1.3 The drone state struct, living in its own header file
- [x] 1.4 Print one line of real drone state with formatted output
- [x] 1.5 A Makefile, so one command builds the whole thing

### 2. A drone that moves  [x] complete
**Deliverable:** the engine runs continuously — position climbs, battery drops, state lines scroll past in real time.
**Concepts:** simulation-tick, delta-time, fixed-timestep, main-loop, sleep-and-timing, battery-model
**Tasks:**
- [x] 2.1 A tick function: one slice of time moves the drone forward
- [x] 2.2 The main loop: ticking forever instead of by hand
- [x] 2.3 Real time: sleeping between ticks so the sim runs at 20 Hz
- [x] 2.4 A battery that drains with use
- [x] 2.5 Takeoff, level off, and stop when the battery is flat

### 3. A drone that flies a mission  [x] complete
**Deliverable:** given three waypoints, it steers to each in turn, reports arrival, and finishes with COMPLETE.
**Concepts:** arrays-of-structs, vectors-and-distance, heading-and-direction, arrival-threshold, finite-state-machine, enums
**Tasks:**
- [x] 3.1 A Waypoint struct and the mission: an array of three of them, printed at startup
- [x] 3.2 How far to go: straight-line distance from the drone to the current waypoint
- [x] 3.3 Which way to go: the direction to the waypoint, and the heading in degrees
- [x] 3.4 Flying it: move along that direction every tick at cruise speed
- [x] 3.5 Arrival: a "close enough" threshold, advancing to the next waypoint, announcing each one
      - the 3.4 descent gap is fixed: a fourth altitude branch descends at CONTROLLED_DESCENT_RATE_MPS when the battery is healthy and the drone is above its target, clamped so one tick cannot overshoot downward.
      - known gap found in 3.5: arrival is horizontal only, and the climb rate cannot keep up with the leg lengths — at 2 m/s over a 41 m leg flown at 10 m/s the drone gains 8 m, so the original 200/400/310 m waypoints were unreachable by two orders of magnitude. Altitudes were reduced to 12/20/8 m so the mission is coherent and the descent branch is observable. Decide in section 4 whether to keep short legs with low altitudes, or spread waypoints to realistic kilometre-scale distances and speed simulated time up.
- [x] 3.6 Flight modes: an enum and the state machine that runs TAKEOFF -> NAVIGATE -> COMPLETE
      - takeoff is now a real phase: the drone climbs to WP0's altitude with no horizontal movement, then transitions. There is no
        cruise-altitude constant - the climb target is the first waypoint's own altitude. A real vehicle would climb to a fixed safe
        altitude first, for obstacle clearance; revisit in section 4 if the waypoint altitudes ever get spread out.
      - gap found and deliberately deferred: the battery-flat emergency descent is still implicit state. A drone sinking at 5 m/s with
        a dead battery still reports MODE_NAVIGATE. Add MODE_LANDING (or a failsafe mode) - the learner spotted this unprompted while
        asking whether a MODE_OFF was needed. Due in section 4, where edge cases get tested.
      - the transition rule compares two doubles with ==, which works only because the climb clamp assigns the exact target value.
        Fragile if the altitude logic ever changes. Tie it to isnan()/tolerance comparisons in section 4.
      - the climb branch now caps its step at the remaining gap (correct by construction, cannot overshoot) while both descent
        branches still move-then-clamp. Two idioms for one idea. Convert the descents to match once section 4 has tests that can
        prove the behaviour did not change.
      - the printf inside tick() is a known compromise: tick() now both advances the simulation and writes to the terminal, which will
        block section 4's test program from calling it quietly. Section 5's data contract is where it gets separated.

### 4. Proving it's right  [x] complete
**Deliverable:** `make test` runs the assertions and prints all-pass — including "east at 10 m/s for 1 second lands at x=10".
**Concepts:** assert, test-program, make-targets, edge-cases, regression-testing, deterministic-simulation
**Format trial (section 4 only, agreed 2026-09-20):** learner proposes the approach FIRST, before any guidance on how.
Each task opens with what it must achieve and nothing about how; the learner says how they would do it; only then does the
lesson say what it would have done, and the two get compared. Gaps in a proposal are surfaced by QUESTIONS that walk the
learner into them, never by naming them outright — and never by letting a known-broken design ship (learner's amendment,
same day: "can you not let me miss a test suite but rather prompt my thinking instead"). Questions point at the territory,
not the answer; the learner must be able to get them wrong. Precondition: a proposal is on the table before any prompting.
Reason: the learner named the real gap — "youre telling me what to do and im doing it but how do i know i have to do those
things" — judgment is built by consequences, not by instruction. Review at the end of section 4 and decide whether to keep it.
**Tasks:** (stated as outcomes only — the how is the learner's to propose)
- [x] 4.1 `make test` exists: it produces a program, separate from the engine, that checks one claim about the engine's
      behaviour and says out loud whether the claim held.
      - the real work turned out to be structural, not about testing: `src/drone.c` held both the engine and `main()`, so
        nothing could use the engine without being the drone. Split into `src/drone.c` (capabilities) and `src/main.c`
        (decisions); `drone.h` now declares the four engine functions. Binaries moved to `build/` on the learner's own
        argument that a per-binary `clean` goes stale — it already had.
      - the section-3 prediction came true on the first run: `tick()`'s `printf("MODE NAVIGATE")` fires inside the test,
        so `make test` prints engine noise that has nothing to do with the claim. Still due in section 5.
      - the test asserts `battery_before > d.battery_percent` and was verified by deliberately commenting out the drain
        line and watching it fail, then restoring it.
- [x] 4.2 The section's headline claim is checked: "flying east at 10 m/s for 1 second lands the drone at x = 10" —
      and the test is fair, meaning it would actually fail if the engine were wrong.
      - the proposal was right in shape on the first pass. The two ways this test fails silently on a CORRECT engine were
        both found by the learner reading tick(): heading_deg and speed_mps are overwritten inside tick(), so the waypoint
        is the steering wheel; and the TAKEOFF -> NAVIGATE transition sits at the END of tick(), so the first tick of a
        zero-initialised drone never moves. Fixed by setting .mode = MODE_NAVIGATE in the test's own drone.
      - fairness was defined as "would fail if the engine were wrong" and then earned twice: the waypoint was moved from
        x = 10 to x = 100 so a target-snapping engine cannot pass, and CRUISE_SPEED_MPS was dropped to 5.0 to watch the
        assertion abort before being restored.
      - the == trap landed for real: assert(y_m == 0) failed on a correct engine because cos of a double approximation of
        pi/2 leaves 6.1e-16 m of drift. Both asserts now use fabs(actual - expected) < TOLERANCE_M, 1e-9. The x assert had
        been passing on a geometric accident (sin is flat at its peak), which is exactly the green-today-red-Tuesday test.
        This closes the section-3 note about == comparisons — but note the engine's own transition rule at drone.c:141
        still compares two doubles with >=, and is still only safe because the climb clamp assigns the exact target value.
      - the engine noise predicted in section 3 is now visible on every run: `make test` prints MODE NAVIGATE from claim 1's
        drone before any test output. Still due in section 5.
      - deferred: the tolerance reasoning was supplied, not produced. Worth re-asking cold in a later lesson.
      - decided 2026-09-22: the printf inside tick() stays until section 5. The learner was offered a short task to move
        the transition announcement out to main.c before 4.3, weighed it against section 5's contract work, and chose to keep
        the fix where the reason for it lives. Tasks 4.3-4.5 run with MODE NAVIGATE printing into the test output.
- [x] 4.3 The boundaries are checked: the places where the engine is most likely to be wrong are named and asserted,
      not the places where it is obviously right.
      - all seven forks in tick() were listed, then ranked by consequence. Claims 3-8 sit at the forks: dead drone at
        battery exactly 0 sinks; healthy drone climbs / descends / holds exactly at target; climb and descent caps land
        exactly on target from inside one step, plus the takeoff -> navigate transition asserted at that pivot. Every
        claim was break-checked with a prediction first.
      - claim 3 was green but unfair on its first version (it read claim 1's drone). The break check caught it. This is
        the reason the break check is not optional.
      - claim 6 exposed double protection: <= on the climb fork alone does not change behaviour, because the cap trims
        the step to 0. Only fork AND cap broken together fail it.
      - the cap tests use == on purpose: exactness is the claim, and the transition relies on it. A throwaway check found
        0 of 10M near-target pairs where alt + (target - alt) != target. This settles the section-3 worry about the
        transition's exact comparison, at least for the current engine.
      - each claim is now its own named function, and main() is the list of calls. The learner first chose bare { }
        blocks, then asked to switch the same session: named failures and a wall in both directions. He converted
        claims 1-2; claims 3-8 were converted for him on request (structure only, bodies unchanged).
      - for 4.4: whether a battery of 0.000001 % should count as powered (a reserve / failsafe threshold) - learner's question.
      - for 4.5: a failed assert aborts, hiding every claim after it.
      - noted: the printf inside tick() made the learner misread which line had run. Still due in section 5.
- [x] 4.4 Every mode the drone can be in has a name. A drone sinking with a dead battery no longer reports
      MODE_NAVIGATE, and the change is proven safe by tests that were passing before it and still pass after.
      - built red-first: baseline green -> MODE_FAILSAFE appended to the enum (still green) -> claims 9 (navigating) and
        10 (taking off) written and seen red, each on its own -> one line in the battery clamp at the top of tick()
        -> all green. The failsafe latches: nothing in tick() leaves it.
      - claim 9 was first green because it was never called from main(). Expecting red is what caught it.
      - claims 9 and 10 first put the drone on the ground (altitude 0), which did not match their headers. Both now fly.
      - for 4.5: claims 3-8 depend on MODE_TAKEOFF being enum value 0 without saying so (moving MODE_FAILSAFE first
        broke claim 7). Decide whether every claim states its mode.
      - for 4.5: claim 9's abort hid claim 10 for real. It had to be seen red by commenting claim 9's call out.
      - still open: is a grounded, unpowered drone 'failsafe' or 'off'? This is the same question as the 0.000001 % reserve.
- [x] 4.5 One command, whole suite, all-pass: every claim from 4.1–4.4 runs on every `make test`, and a broken engine
      is caught by it rather than by reading the terminal.
      - assert is gone. A check(ok, description) helper prints `<description>  PASS|FAIL` in one aligned column and
        returns 1/0; each claim returns the sum of its checks; main() adds up the claims and returns 1 if anything failed,
        0 otherwise. Returning the raw count was rejected: only 8 bits of exit status survive, so 256 failures would read
        as success.
      - the design was the learner's, piece by piece (PASS/FAIL instead of abort, a failure count, the helper's inputs and
        output, conditions passed as what SHOULD be true). The exit-status half was walked to, not proposed.
      - two silent-lie bugs were written and caught by break checks, both the same shape: a return value dropped (main()
        ignoring claim 7, then `failed =` overwriting instead of `+=`). -Werror cannot catch the second one.
      - final proof: commenting out the TAKEOFF->NAVIGATE assignment failed exactly claims 7 and 8, as predicted, while
        the engine's printf still said MODE NAVIGATE. The terminal lied; the suite did not.
      - every claim now sets .mode explicitly; none relies on MODE_TAKEOFF being enum value 0. Learner caught that
        claim 1 relied on it too.
      - still open, carried forward: is a grounded, unpowered drone 'failsafe' or 'off' (same question as the 0.000001 %
        reserve). The printf inside tick() is still due in section 5.
      - reviewed 2026-09-25: the propose-first format is KEPT for section 5 and beyond. Learner's reason: "i feel like i
        can explain my code more".

### 5. The contract  [ ] in progress
**Deliverable:** the engine's output becomes structured data — one JSON object per tick, readable by a human and by a machine.
**Concepts:** data-contract, json, serialization, line-based-protocol, stdout-as-stream
**Format:** propose-first, carried over from section 4 (kept at the 2026-09-25 review).
**Tasks:** (stated as outcomes only — the how is the learner's to propose)
- [x] 5.1 The engine stops talking: nothing in src/drone.c writes to the terminal, the mode change is still announced
      when the mission runs, and `make test` output holds nothing but the suite's own lines.
      - the printf left tick(); print_state moved to main.c with its declaration out of drone.h. main.c saves the mode
        before tick() and announces ANY change after it (learner's choice: failsafe must be visible too), through a
        mode_to_string() switch in main.c. First `make test` since 4.1 with no engine noise in it.
      - mode_to_string has no default on purpose: with no default, -Wswitch fails the build if a new FlightMode gets no
        case. The fallback return after the switch only covers out-of-range values.
      - for 5.4: MODE_COMPLETE is never announced by the comparison, because main.c sets it itself, outside the
        before/after window around tick(). "MISSION COMPLETE" prints separately. Two paths for one kind of event.
      - parked (pre-existing): the main loop only stops on MODE_COMPLETE. A drone in MODE_FAILSAFE lands and then ticks
        forever. Decide what the mission program does with a failsafe drone before section 6 depends on the stream ending.
- [x] 5.2 The contract is written down before any code: every field one tick carries, its type and unit, and what a
      reader can rely on — in a place the engine and the future server both point at.
      - docs/telemetry-contract.md: ten fields, each with its unit in its name (mission_time_s, flight_mode,
        position_x_m, position_y_m, altitude_m, heading_deg, horizontal_speed_mps, battery_pct, current_waypoint,
        distance_from_waypoint_m); two promises (a newline ends every line, every line carries every field); one example.
      - mode travels as a string: an enum number shifts meaning when a mode is inserted, and the server would show the
        wrong thing without complaining. Mission time rather than wall-clock time, so 5.5's test has a known expected value.
      - Markdown doc over a .js file (a JS file would belong to the server, and C cannot read it) and over JSON Schema
        (a tool before the first mismatch). main.c points at the doc from above print_state.
      - learner's question exposed that speed and distance are both horizontal only -> renamed horizontal_speed_mps,
        distance meaning made explicit. Climb rate left out of the MVP by choice; adding a field later does not break a reader.
      - for 5.3: the pointer comment above print_state is not true until print_state prints the contract.
      - for 5.4: the "Not covered yet" section of the doc lists what still prints as plain text.
- [ ] 5.3 One JSON object per tick: running the mission prints the drone's state as one JSON line per tick, matching
      the written contract.
- [ ] 5.4 The whole stream is machine-readable: waypoint arrivals, mode changes and mission end are data too, and every
      single line the program prints parses — proven by a machine reading it, not by eye.
- [ ] 5.5 The contract is held by the suite: a test fails if the engine's output stops matching the contract.

### 6. The server  [ ] not started
**Deliverable:** `node server.js` launches the C engine, reads its state, and serves a blank page at localhost:3000.
**Concepts:** nodejs, npm-and-package-json, child-process, stdin-stdout-pipes, http-server, ports, static-file-serving

### 7. The dashboard  [ ] not started
**Deliverable:** open that page and watch the mission-control state block update live as the drone flies.
**Concepts:** websocket, event-driven-javascript, dom-manipulation, rendering-from-state, css-layout, stale-vs-wrong

### 8. Going live  [ ] not started
**Deliverable:** a URL. Send it to someone and they watch the drone fly from their phone.
**Concepts:** docker, dockerfile, container-image, environment-variables, deploy-from-git, production-vs-local, reading-production-logs

### 9. Finishing the MVP  [ ] not started
**Deliverable:** event log and warnings added, pushed, and live in production — completing the loop of shipping a change to a real deployed app.
**Concepts:** event-log, derived-warnings, thresholds, deploy-loop, reading-a-diff, readme

## Sequencing note
Sections 1–5 are built alone in a terminal with nothing pretty to look at — roughly half the project with no visuals. That is deliberate: the engine is the source of truth, and a dashboard drawing numbers from a drone that cannot fly properly is a far worse place to be stuck.
