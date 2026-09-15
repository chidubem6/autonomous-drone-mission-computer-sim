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
- last-reviewed: 2026-09-15
- evidence: self-reported — currently attempting to implement pointers and reading up on them. 2026-09-14 asked directly about `*` in declarations vs expressions and about `struct Node *next`; was given the address/dereference distinction, the `->` shorthand, and the pass-by-value argument for why section 2's tick function must take a DroneState *. Then wrote print_state(const DroneState *d) themselves — six `d->field` reads and a `print_state(&drone)` call site, all correct first time, no prompting on the `&`. 2026-09-15 wrote the tick signature `void tick(DroneState *d, ...)` from the print_state pattern, and every call site as `tick(&drone, 0.05)` — six correct `&` uses, no prompting

## const-correctness
- status: practicing
- depends-on: pointers
- introduced: 2026-09-14
- last-reviewed: 2026-09-15
- evidence: used `const DroneState *d` on print_state after the rationale — a compiler-enforced promise not to modify, and the contrast that will make section 2's un-const `update(DroneState *)` legible. Applied as given, not yet independently reasoned about. 2026-09-15 reasoned it out independently before being told: asked for the tick signature, dropped the const and said why — "dronestate isnt const because we are chaning it". The contrast the section-1 lesson predicted would land, landed

## struct
- status: practicing
- depends-on: none
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: wrote the six-field DroneState unaided from a two-field pattern, units carried in the field names, and initialised it with designated initialisers. Caught nothing wrong with `speed_ms` until it was pointed out that `ms` reads as milliseconds; renamed to `speed_mps`

## header-files
- status: practicing
- depends-on: struct
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: wrote src/drone.h and included it from drone.c with quotes rather than angle brackets, after the "next to this file" vs "system directories" distinction

## preprocessor
- status: introduced
- depends-on: compilation-stages
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: worked with #include as literal text substitution rather than an import system — the basis for reasoning about double inclusion

## include-guards
- status: practicing
- depends-on: preprocessor
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: predicted the section 3 double-inclusion problem before being told — nav.h includes drone.h, drone.c includes both, so the struct arrives twice — then wrote the #ifndef/#define/#endif guard, closing #endif with a naming comment

## undefined-behaviour
- status: introduced
- depends-on: compiler-warnings
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: deliberate %d-on-a-double break. Predicted compile and run correctly but expected C to silently convert the double to an int; the real output was garbage (6 here, 2102178464 on their run). Asked unprompted why the value varies between machines and runs, which earned the calling-convention answer: doubles travel in floating-point registers, %d reads the integer slot, and finds leftovers

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
- last-reviewed: 2026-09-14
- evidence: self-reported — built a CLI number toolkit in C; 2026-09-14 wrote drone.c unaided but omitted the trailing newline the spec asked for, then added it after reasoning about why it matters downstream. Later met the variadic-function consequence: printf has no type information for its arguments, because the types are decided by a runtime string, so a wrong specifier cannot be converted or caught by the language itself. Task 1.4 went deep on formatting: used `%%` for a literal percent unprompted, predicted correctly that growing values would shift the columns, then worked through field widths over several passes — that a width is a MINIMUM and printf never truncates, that numbers right-align and text left-aligns, and that literal text between conversions is not counted in any field width. Asked good questions at each step rather than accepting the rule

## floating-point-numbers
- status: practicing
- depends-on: none
- introduced: 2026-09-14
- last-reviewed: 2026-09-15
- evidence: every DroneState field is a double because the drone moves 0.5 m per tick — integers would round every tick to nothing. Applied but not yet independently reasoned about. 2026-09-15 extended it from struct fields to a function parameter, choosing double for dt with the rounding argument in their own words

## compiling-c
- status: practicing
- depends-on: compiled-vs-interpreted
- introduced: —
- last-reviewed: 2026-09-15
- evidence: self-reported — compiles and runs C from the terminal with gcc; 2026-09-15 recalled the full gcc line from memory into the Makefile recipe, flags and all bar -Werror, which was restored for them; 2026-09-14 built src/drone.c in WSL with -Wall -Wextra -std=c11 and ran it. Also met the Linux/Windows difference — Linux marks a file executable with a permission bit, Windows with the .exe suffix — and asked a good unprompted follow-up about when .exe would appear

## compilation-stages
- status: introduced
- depends-on: compiling-c
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: predicted a .obj file would be left behind after compiling; corrected to the four stages (preprocess, compile, assemble, link) that gcc runs in one command, deleting the intermediate. Object files reappear deliberately in task 1.5

## compiler-warnings
- status: practicing
- depends-on: compiling-c
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: watched -Wformat fire on a deliberate %d/double mismatch and saw the program still build and print nonsense, then adopted -Werror so a warning cannot be scrolled past. Also saw the limit of the tool: a stray `!` inside a format string passed every flag, because the compiler checks well-formedness, never intent

## project-structure
- status: introduced
- depends-on: none
- introduced: 2026-09-14
- last-reviewed: —
- evidence: created src/ after the three-folder layout (src/ engine, server/ node, public/ dashboard) was explained in terms of what the Dockerfile will need to say in section 8

## makefile
- status: practicing
- depends-on: compiling-c
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: wrote the first rule themselves after a worked example in another domain. First prerequisite list named only src/drone.c; when asked about the header, reasoned correctly that #include pastes drone.h in regardless — true of gcc, but missing that gcc is never invited if make sees no change. Then predicted the conditional sharply — "make will run the gcc if drone.h was in the src file list" — and confirmed it with touch src/drone.h: 'drone' is up to date, followed by a real rebuild once the header was declared. Asked two good structural questions unprompted: where Makefile comes from (program vs file you author), and how make tracks changes (it does not — it reads filesystem mtimes fresh each run). Left the <file> placeholder and three TODO blocks in on the first cleanup pass

## file-timestamps
- status: practicing
- depends-on: makefile
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: first read "timestamp" as metadata about build reproducibility and said so plainly; after the correction to last-modified time, read the three real mtimes off ls --time-style=full-iso and saw drone sitting 42 seconds newer than drone.c. Met touch as an instrument for changing mtime without changing content

## make-targets
- status: practicing
- depends-on: makefile
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: asked what was actually wrong with a recipe that both builds and runs, which earned the concrete answers (hostage terminal, a Docker RUN make that never exits, and a target being a noun). The run: drone dependency chain was supplied after a stuck watch, not derived; clean and .PHONY: run clean were written unaided. Predicted correctly that gcc runs exactly once across make clean / make / make run, and why. Then tested the phony-vs-file distinction unprompted — three make runs in a row all fired, two bare makes in a row both said up to date

## simulation-tick
- status: practicing
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-15
- evidence: explained as freezing time and computing a small slice forward, repeatedly; worked through 10 m/s x 0.05s = 0.5 m per tick. 2026-09-15 turned it into code: wrote `d->altitude_m += CLIMB_RATE_MPS * dt;` unaided after one hint, then predicted the four output altitudes (0.0, 0.1, 0.2, 0.3) exactly before the build ran

## delta-time
- status: practicing
- depends-on: simulation-tick
- introduced: 2026-09-13
- last-reviewed: 2026-09-15
- evidence: predicted correctly that a 300 ms tick causes the dashboard to lag and position to jump; extended to "the drone being in the wrong place is the bug, the lag is the symptom". 2026-09-15 met dt as a parameter. Reasoned unprompted that it must be a double — "its a fraction of a unit and we want specifity, like we dont want to round up or down". On a deliberate `+=` to `=` break, predicted 0.1/0.1/0.1 and gave the mechanism in one sentence: "it overwrites instead of adding" — though miscounted the printed lines as three, forgetting the pre-tick print

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
- status: introduced
- depends-on: data-contract
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: asked what a missing newline would do to a line-reading server, answered that it would read the wrong number of lines and get a bad format — right direction, sharpened to the real failure: the newline IS the delimiter, so the reader waits forever for an end that never comes

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
- last-reviewed: 2026-09-15
- evidence: 2026-09-15 ran make --version in the session's Git Bash rather than WSL and got command not found; the two-toolsets-one-laptop explanation was given, not derived. Trunk component #7, revisited during the deployment decision — Docker shrinks the gap by making both environments the same machine. 2026-09-14 the gap shrank for real: builds moved to WSL (Ubuntu 24.04), the same operating system the section 8 container runs. 2026-09-15 asked unprompted what Git Bash actually is, which earned the sharper split: Git Bash is a Unix-shaped surface over Windows (bash plus a few recompiled tools, no apt, no toolchain), WSL is a real Ubuntu with its own filesystem and gitconfig. Said they have make in both places, which sits oddly against yesterday's "command not found" in Git Bash — unresolved, not chased

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
- last-reviewed: 2026-09-14
- evidence: self-reported — uses git frequently; 2026-09-14 explained unprompted that zipping the project while skipping hidden folders delivers the files but loses the history, because the repository *is* the .git folder

## git-commit
- status: practicing
- depends-on: git-repository
- introduced: —
- last-reviewed: 2026-09-14
- evidence: self-reported — uses git frequently; 2026-09-14 wrote and ran the repository's root commit, message authored themselves in the present-tense convention

## git-staging-area
- status: introduced
- depends-on: git-repository
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: correctly predicted that `git status` would list two untracked items rather than five, because git collapses an untracked directory into a single entry

## gitignore
- status: practicing
- depends-on: git-staging-area
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: filled in the `*.exe` pattern correctly by generalising from the `*.o` line; needed a second pass to remove the stale TODO block, which prompted a note about leaving finished instructions in files. Later the same day predicted correctly that the new Linux binary would show as untracked because *.exe could not match it, then replaced the dead rule with /drone

## line-endings-lf-crlf
- status: introduced
- depends-on: gitignore
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: triggered by git's CRLF warning on first `git add`; predicted correctly that Linux would try to read the carriage return and fail, though not the specific `bad interpreter` shape of the error. Fix (.gitattributes with eol=lf) was dictated, not derived

## git-identity-config
- status: introduced
- depends-on: git-commit
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: committing from WSL prompted for name and email, because WSL has its own home directory and its own ~/.gitconfig separate from Windows git. Asked unprompted whether the new identity matched earlier commits, and verified it with `git log --format`

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

## named-constants
- status: introduced
- depends-on: preprocessor
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: CLIMB_RATE_MPS entered as a #define rather than a DroneState field, on the reasoning that a climb rate is a property of how this drone flies, not of where it is right now. Used correctly in the tick body; the preprocessor-substitution link was given, not derived

## functions-over-main
- status: introduced
- depends-on: none
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: asked unprompted why code gets moved out of main, which earned the hard reason — section 4's test program has its own main and can call tick() but can never reach lines buried inside another main, so code in main is untestable forever. Answer received, not yet applied under their own steam
