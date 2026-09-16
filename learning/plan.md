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

### 3. A drone that flies a mission  [ ] not started
**Deliverable:** given three waypoints, it steers to each in turn, reports arrival, and finishes with COMPLETE.
**Concepts:** arrays-of-structs, vectors-and-distance, heading-and-direction, arrival-threshold, finite-state-machine, enums

### 4. Proving it's right  [ ] not started
**Deliverable:** `make test` runs the assertions and prints all-pass — including "east at 10 m/s for 1 second lands at x=10".
**Concepts:** assert, test-program, make-targets, edge-cases, regression-testing, deterministic-simulation

### 5. The contract  [ ] not started
**Deliverable:** the engine's output becomes structured data — one JSON object per tick, readable by a human and by a machine.
**Concepts:** data-contract, json, serialization, line-based-protocol, stdout-as-stream

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
