# Knowledge graph

<!-- statuses: seed → introduced → practicing → understood -->
<!-- seed: not yet taught | introduced: explained once | practicing: used it with help | understood: explained in own words + passed a quiz -->

## compiled-vs-interpreted
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained during the language decision — Python's interpreter runs code line by line; C is translated to machine instructions once and run directly by the CPU

## manual-memory-management
- status: introduced
- depends-on: compiled-vs-interpreted
- introduced: 2026-09-13
- last-reviewed: —
- evidence: explained as the thing C does not do for you, and why garbage collection pauses break real-time flight control

## pointers
- status: practicing
- depends-on: manual-memory-management
- introduced: —
- last-reviewed: 2026-09-13
- evidence: self-reported — currently attempting to implement pointers and reading up on them

## struct
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## header-files
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## arrays-of-structs
- status: seed
- depends-on: struct
- introduced: —
- last-reviewed: —
- evidence: —

## enums
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## printf-format
- status: practicing
- depends-on: none
- introduced: —
- last-reviewed: 2026-09-13
- evidence: self-reported — built a CLI number toolkit in C

## floating-point-numbers
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## compiling-c
- status: practicing
- depends-on: compiled-vs-interpreted
- introduced: —
- last-reviewed: 2026-09-13
- evidence: self-reported — compiles and runs C from the terminal with gcc

## makefile
- status: seed
- depends-on: compiling-c
- introduced: —
- last-reviewed: —
- evidence: —

## make-targets
- status: seed
- depends-on: makefile
- introduced: —
- last-reviewed: —
- evidence: —

## simulation-tick
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained as freezing time and computing a small slice forward, repeatedly; worked through 10 m/s x 0.05s = 0.5 m per tick

## delta-time
- status: introduced
- depends-on: simulation-tick
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: predicted correctly that a 300 ms tick causes the dashboard to lag and position to jump; extended to "the drone being in the wrong place is the bug, the lag is the symptom"

## fixed-timestep
- status: seed
- depends-on: delta-time
- introduced: —
- last-reviewed: —
- evidence: —

## main-loop
- status: seed
- depends-on: simulation-tick
- introduced: —
- last-reviewed: —
- evidence: —

## sleep-and-timing
- status: seed
- depends-on: main-loop
- introduced: —
- last-reviewed: —
- evidence: —

## battery-model
- status: seed
- depends-on: delta-time
- introduced: —
- last-reviewed: —
- evidence: —

## deterministic-simulation
- status: seed
- depends-on: fixed-timestep
- introduced: —
- last-reviewed: —
- evidence: —

## tick-rate-vs-update-rate
- status: introduced
- depends-on: simulation-tick
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: correctly said a 5 Hz dashboard reading a 20 Hz engine is not wrong, it just misses in-between states — reached "sampling" unprompted

## vectors-and-distance
- status: seed
- depends-on: floating-point-numbers
- introduced: —
- last-reviewed: —
- evidence: —

## heading-and-direction
- status: seed
- depends-on: vectors-and-distance
- introduced: —
- last-reviewed: —
- evidence: —

## waypoint-list
- status: seed
- depends-on: arrays-of-structs
- introduced: —
- last-reviewed: —
- evidence: —

## arrival-threshold
- status: seed
- depends-on: vectors-and-distance
- introduced: —
- last-reviewed: —
- evidence: —

## finite-state-machine
- status: seed
- depends-on: enums
- introduced: —
- last-reviewed: —
- evidence: —

## test-is-a-claim
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained during the testing decision — a test is a claim about behaviour written down so a machine can check it

## assert
- status: seed
- depends-on: test-is-a-claim
- introduced: —
- last-reviewed: —
- evidence: —

## test-program
- status: seed
- depends-on: test-is-a-claim
- introduced: —
- last-reviewed: —
- evidence: —

## edge-cases
- status: seed
- depends-on: test-is-a-claim
- introduced: —
- last-reviewed: —
- evidence: —

## regression-testing
- status: seed
- depends-on: test-is-a-claim
- introduced: —
- last-reviewed: —
- evidence: —

## test-failure-ambiguity
- status: introduced
- depends-on: regression-testing
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: correctly identified the two meanings of a newly failing test — the code broke, or the test is outdated — and was warned about the convenience of always choosing the second

## process-architecture-two-programs
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: chose the two-program split over a single program after seeing the tradeoff; understood the engine flies the drone and knows nothing about the internet

## stdout-as-stream
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained that a program writes to standard output without knowing where it lands; the terminal is a default, not a law

## stdin-stdout-pipes
- status: introduced
- depends-on: stdout-as-stream
- introduced: 2026-09-13
- last-reviewed: —
- evidence: explained via the shell pipe — one program's output rewired into another program's input, neither knowing

## parent-child-process
- status: introduced
- depends-on: process-architecture-two-programs
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: worked out unprompted that the server runs the engine; consequence noted — if the server dies the engine goes with it, and deployment ships one thing

## data-contract
- status: introduced
- depends-on: process-architecture-two-programs
- introduced: 2026-09-13
- last-reviewed: —
- evidence: trunk component #5 — both sides must agree on field names and units or nothing on screen moves

## json
- status: seed
- depends-on: data-contract
- introduced: —
- last-reviewed: —
- evidence: —

## serialization
- status: seed
- depends-on: json
- introduced: —
- last-reviewed: —
- evidence: —

## line-based-protocol
- status: seed
- depends-on: data-contract
- introduced: —
- last-reviewed: —
- evidence: —

## nodejs
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: —
- evidence: explained as a program that runs JavaScript outside a browser, chosen so server and dashboard share one language

## npm-and-package-json
- status: seed
- depends-on: nodejs
- introduced: —
- last-reviewed: —
- evidence: —

## http-server
- status: seed
- depends-on: nodejs
- introduced: —
- last-reviewed: —
- evidence: —

## ports
- status: introduced
- depends-on: http-server
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: correctly placed the port between browser and server, and correctly ruled it out between server and engine

## static-file-serving
- status: seed
- depends-on: http-server
- introduced: —
- last-reviewed: —
- evidence: —

## websocket
- status: introduced
- depends-on: ports
- introduced: 2026-09-13
- last-reviewed: —
- evidence: explained as a connection that opens once and stays open, either side sending at will; chosen over polling and server-sent events

## polling-vs-push
- status: introduced
- depends-on: websocket
- introduced: 2026-09-13
- last-reviewed: —
- evidence: explained during the transport decision — asking repeatedly versus being pushed to

## client-server-separation
- status: introduced
- depends-on: process-architecture-two-programs
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: "the dashboard is a window, not a cockpit" — understood the engine is in charge and the dashboard has no idea a drone exists

## html-structure
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## css-layout
- status: seed
- depends-on: html-structure
- introduced: —
- last-reviewed: —
- evidence: —

## dom-manipulation
- status: seed
- depends-on: html-structure
- introduced: —
- last-reviewed: —
- evidence: —

## event-driven-javascript
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## rendering-from-state
- status: seed
- depends-on: dom-manipulation, client-server-separation
- introduced: —
- last-reviewed: —
- evidence: —

## stale-vs-wrong
- status: introduced
- depends-on: tick-rate-vs-update-rate
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: a dashboard may be stale (showing something up to 200 ms old) but should never be wrong (showing something that never happened); stale is a tuning knob, wrong is a bug

## event-log
- status: seed
- depends-on: data-contract
- introduced: —
- last-reviewed: —
- evidence: —

## derived-warnings
- status: seed
- depends-on: event-log
- introduced: —
- last-reviewed: —
- evidence: pulled into the MVP by the learner, scoped to warnings computed from state that already exists

## thresholds
- status: seed
- depends-on: derived-warnings
- introduced: —
- last-reviewed: —
- evidence: —

## when-you-need-a-database
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: —
- evidence: decided against a database for the MVP — nothing in it needs remembering; queued for the v2 flight recorder

## two-environments-local-and-production
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: trunk component #7, revisited during the deployment decision — Docker shrinks the gap by making both environments the same machine

## docker
- status: introduced
- depends-on: two-environments-local-and-production
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained as shipping the machine rather than hoping the host matches; correctly predicted deployment becomes less scary

## dockerfile
- status: seed
- depends-on: docker
- introduced: —
- last-reviewed: —
- evidence: —

## container-image
- status: seed
- depends-on: docker
- introduced: —
- last-reviewed: —
- evidence: —

## environment-variables
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## deploy-from-git
- status: seed
- depends-on: git-commit, dockerfile
- introduced: —
- last-reviewed: —
- evidence: —

## production-vs-local
- status: seed
- depends-on: two-environments-local-and-production
- introduced: —
- last-reviewed: —
- evidence: —

## reading-production-logs
- status: seed
- depends-on: production-vs-local
- introduced: —
- last-reviewed: —
- evidence: —

## deploy-loop
- status: seed
- depends-on: deploy-from-git
- introduced: —
- last-reviewed: —
- evidence: —

## git-repository
- status: practicing
- depends-on: none
- introduced: —
- last-reviewed: 2026-09-13
- evidence: self-reported — uses git frequently

## git-commit
- status: practicing
- depends-on: git-repository
- introduced: —
- last-reviewed: 2026-09-13
- evidence: self-reported — uses git frequently

## readme
- status: seed
- depends-on: none
- introduced: —
- last-reviewed: —
- evidence: —

## debugging-by-bisecting-the-pipeline
- status: introduced
- depends-on: process-architecture-two-programs
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: given a stuck battery reading, said to check the engine first because it is the source of truth — reasoning was correct before being told

## scoping-an-mvp
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: cut object tracking deliberately, argued the event log and warnings back in, and accepted the narrow scoping that separates a feature from creep

## writing-a-plan
- status: introduced
- depends-on: scoping-an-mvp
- introduced: 2026-09-13
- last-reviewed: —
- evidence: walked all eight stack decisions and approved the nine-section sequence

## reading-a-diff
- status: seed
- depends-on: git-commit
- introduced: —
- last-reviewed: —
- evidence: —

## agent-memory-claude-md
- status: seed
- depends-on: writing-a-plan
- introduced: —
- last-reviewed: —
- evidence: —

## never-ship-what-you-cant-explain
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: the reason React was left out of the MVP — a build toolchain would be the one mystery box in a project whose premise is that there are none
