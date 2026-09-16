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
- last-reviewed: 2026-09-16
- evidence: self-reported — currently attempting to implement pointers and reading up on them. 2026-09-14 asked directly about `*` in declarations vs expressions and about `struct Node *next`; was given the address/dereference distinction, the `->` shorthand, and the pass-by-value argument for why section 2's tick function must take a DroneState *. Then wrote print_state(const DroneState *d) themselves — six `d->field` reads and a `print_state(&drone)` call site, all correct first time, no prompting on the `&`. 2026-09-15 wrote the tick signature `void tick(DroneState *d, ...)` from the print_state pattern, and every call site as `tick(&drone, 0.05)` — six correct `&` uses, no prompting. 2026-09-15 wrote the clamp as `if (battery < 0)` — a bare name with nothing in scope — and corrected to `d->battery_percent` once reminded that only d and dt exist inside tick(). Struct fields are not loose variables; the pointer is the only route in. 2026-09-16 wrote `D->altitude_m` with a capital D; C is case-sensitive and there is no such name

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
- evidence: 2026-09-16 wrote the Waypoint struct in drone.h from the DroneState pattern — three double fields with units in the names — but left the type off the third (`altitude_m;` alone), and fixed it after being told that two of the three lines had something the third was missing. Earlier: wrote the six-field DroneState unaided from a two-field pattern, units carried in the field names, and initialised it with designated initialisers. Caught nothing wrong with `speed_ms` until it was pointed out that `ms` reads as milliseconds; renamed to `speed_mps`

## header-files
- status: practicing
- depends-on: struct
- introduced: 2026-09-14
- last-reviewed: 2026-09-14
- evidence: wrote src/drone.h and included it from drone.c with quotes rather than angle brackets, after the "next to this file" vs "system directories" distinction

## preprocessor
- status: practicing
- depends-on: compilation-stages
- introduced: 2026-09-14
- last-reviewed: 2026-09-15
- evidence: worked with #include as literal text substitution rather than an import system — the basis for reasoning about double inclusion. 2026-09-15 restated the mechanism in their own words unprompted — "takes the name and pastes whatever follows it... like stdio.h is text sub where it sees this include and just replaces it with the actual file" — then met the signature trap: the mistake sits in the #define, the error arrives at the call site, because the #define line no longer exists by the time the compiler proper runs

## include-guards
- status: practicing
- depends-on: preprocessor
- introduced: 2026-09-14
- last-reviewed: 2026-09-15
- evidence: predicted the section 3 double-inclusion problem before being told — nav.h includes drone.h, drone.c includes both, so the struct arrives twice — then wrote the #ifndef/#define/#endif guard, closing #endif with a naming comment. 2026-09-15 identified the mechanism unaided when <unistd.h> appeared to declare nothing — named the #ifndef/#define machinery from drone.h as the reason a line inside a header can vanish, before being told. Generalised from include guards to conditional compilation

## undefined-behaviour
- status: introduced
- depends-on: compiler-warnings
- introduced: 2026-09-14
- last-reviewed: 2026-09-16
- evidence: 2026-09-16 met it a second way, via array bounds: predicted correctly and unprompted that `i <= COUNT` "would try and read past the list", but guessed the reason was that the access is "still a valid access". Corrected to the real one — it is not valid, it is unchecked; `mission[3]` is arithmetic (base + 3 x sizeof) that succeeds for any index, and C stores no length to compare against, so there is nothing to check WITH. Asked the question honestly rather than guessing silently. 2026-09-14: deliberate %d-on-a-double break. Predicted compile and run correctly but expected C to silently convert the double to an int; the real output was garbage (6 here, 2102178464 on their run). Asked unprompted why the value varies between machines and runs, which earned the calling-convention answer: doubles travel in floating-point registers, %d reads the integer slot, and finds leftovers

## arrays-of-structs
- status: practicing
- depends-on: struct
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.1. First attempt put the array's own name inside its initializer — `{mission.x_m = 10, ...}` — held to across three corrections, which surfaced the real question they were asking: how do you say WHICH slot a value goes in. Asked unprompted how other languages initialise arrays, and separately why C uses `{ }` rather than `[ ]`; both were answered (position is the index; `[ ]` is already taken by declaration and subscript, and `[0] =` is C's slot designator). Wrote the three-waypoint literal correctly once the prefix was removed. Then wrote the full read side — `mission[i].x_m` and its two siblings inside a four-slot printf — after being shown one of the three arguments

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
- last-reviewed: 2026-09-16
- evidence: 2026-09-16 built the mission-announcement line in task 3.1 incrementally — label first, then widths, then arguments — and landed a correct four-slot line (`%d` for the int index, three `%5.1f`/`%4.1f` for doubles) with matched arguments in order. En route left a `%.1f` with no argument behind it, which is the variadic hazard they already knew in theory meeting their own code. Earlier: self-reported — built a CLI number toolkit in C; 2026-09-14 wrote drone.c unaided but omitted the trailing newline the spec asked for, then added it after reasoning about why it matters downstream. Later met the variadic-function consequence: printf has no type information for its arguments, because the types are decided by a runtime string, so a wrong specifier cannot be converted or caught by the language itself. Task 1.4 went deep on formatting: used `%%` for a literal percent unprompted, predicted correctly that growing values would shift the columns, then worked through field widths over several passes — that a width is a MINIMUM and printf never truncates, that numbers right-align and text left-aligns, and that literal text between conversions is not counted in any field width. Asked good questions at each step rather than accepting the rule

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
- last-reviewed: 2026-09-16
- evidence: 2026-09-16 hit two new ones in task 3.1 under -Werror. `variable 'mission' set but not used` after filling the array and never reading it — understood as unfinished rather than broken, and cleared by writing the loop that reads it, not by silencing it. Also created a format/argument mismatch mid-edit (`%.1f` with no value behind it) and was shown the one-slot-one-argument rule before building. Earlier: watched -Wformat fire on a deliberate %d/double mismatch and saw the program still build and print nonsense, then adopted -Werror so a warning cannot be scrolled past. Also saw the limit of the tool: a stray `!` inside a format string passed every flag, because the compiler checks well-formedness, never intent. 2026-09-15 hit `implicit declaration of function usleep` under -Werror and read the error without alarm. The cause chain (-std=c11 sets __STRICT_ANSI__, glibc hides non-ISO declarations, the preprocessor deletes the line, the compiler then meets an unknown name) was explained across three passes at their request; the fix -std=c11 -> -std=gnu11 was dictated, not derived

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
- status: introduced
- depends-on: delta-time
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: answered the section-4 question correctly — fixed is what makes an exact-value test possible, because an asserted dt gives byte-identical runs while a measured one gives 9.97 or 10.04 depending on CPU load. Then asked for variable timestep to be explained, and got it: games need it to decouple world speed from frame rate, its costs are irreproducibility, spike-teleporting (bullet tunnelling) and integration instability, and real engines use a fixed-step accumulator with variable rendering. Also met the drift this project accepts: a pass costs 50 ms plus the cost of tick and print

## main-loop
- status: practicing
- depends-on: simulation-tick
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: wrote the loop body unaided — `while(1) { tick(&drone, TICK_S); print_state(&drone); }` — replacing six hand-written lines. First reached for `while(true)` out of habit from other languages and swapped it for `while(1)` on the hint, before compiling, so never met the undeclared-identifier error itself; the <stdbool.h> explanation was given afterwards. Met Ctrl+C as the brake for a runaway program

## sleep-and-timing
- status: practicing
- depends-on: main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: wrote `usleep(TICK_S * 1000000);` — got the seconds-to-microseconds conversion after one hint about the direction, and expressed it from TICK_S rather than a literal so the two cannot drift. Needed the semicolon pointed out. Met the framing that C has no way to wait: sleeping is an OS service, which is why it lives in <unistd.h> and not the C standard library

## battery-model
- status: practicing
- depends-on: delta-time
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: derived the drain rate from a hardware spec rather than inventing it — given "a small quadcopter flies about 20 minutes", answered 0.083 %/s and showed the working (100 / 1200) unaided. Wrote `d->battery_percent -= DRAIN_RATE_PCT_PER_S * dt;` from the altitude line. Predicted correctly and unprompted that an unclamped battery would go negative, at about -25 % after 25 minutes, and later that a flat battery would not stop the climb because nothing connects the two

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
- last-reviewed: 2026-09-15
- evidence: correctly said a 5 Hz dashboard reading a 20 Hz engine is not wrong, it just misses in-between states — reached "sampling" unprompted. 2026-09-15 saw the extreme case for real. 2026-09-15 asked unprompted whether the 50 ms was modelling telemetry relay rate — close enough to earn the real split: 50 ms is the control-loop rate and the physics step size fused into one number, while telemetry downlink is a separate, slower clock (1-10 Hz). Also met the point that a real flight computer samples reality while this program has to manufacture it

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
- status: practicing
- depends-on: arrays-of-structs
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.1. Chose their own three-waypoint route (10,40 / 50,25 / 95,75) and wrote it as a Waypoint array in main(), announced at startup before the flight loop. Picked waypoint altitudes of 200-400 m against a CRUISE_ALTITUDE_M of 100 — flagged for them as something task 3.4 has to reconcile, not yet resolved

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
- last-reviewed: 2026-09-16
- evidence: motivation earned the hard way on 2026-09-16, before the concept was taught. Two separate bugs in section 2.5 came from the same cause — the drone's situation was inferred from number combinations rather than stored — and the learner diagnosed both by tracing the chain by hand. Comes due in section 3

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
- evidence: 2026-09-15 ran make --version in the session's Git Bash rather than WSL and got command not found; the two-toolsets-one-laptop explanation was given, not derived. Trunk component #7, revisited during the deployment decision — Docker shrinks the gap by making both environments the same machine. 2026-09-14 the gap shrank for real: builds moved to WSL (Ubuntu 24.04), the same operating system the section 8 container runs. 2026-09-15 asked unprompted what Git Bash actually is, which earned the sharper split: Git Bash is a Unix-shaped surface over Windows (bash plus a few recompiled tools, no apt, no toolchain), WSL is a real Ubuntu with its own filesystem and gitconfig. Said they have make in both places, which sits oddly against yesterday's "command not found" in Git Bash — unresolved, not chased. Later the same day VS Code IntelliSense reported "#include errors detected" on <unistd.h> while make succeeded — editor on Windows, compiler in WSL, looking at two different operating systems. Checking settled the earlier open question: neither gcc nor make exists on the Windows side, so the whole toolchain is WSL-only. The "which program is complaining?" habit was named; the code . fix via the WSL extension was offered, not yet confirmed as applied

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
- status: practicing
- depends-on: git-repository
- introduced: 2026-09-14
- last-reviewed: 2026-09-15
- evidence: correctly predicted that `git status` would list two untracked items rather than five, because git collapses an untracked directory into a single entry. 2026-09-15 ran `git commit --amend --no-edit` after deleting a line but before staging it, and the commit came back unchanged. Saw that amend commits the index like any other commit, that the hash changed anyway on committer timestamp alone, and fixed it with git add -A first. Desk / envelope / posted was offered as the model

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
- status: practicing
- depends-on: preprocessor
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: CLIMB_RATE_MPS entered as a #define rather than a DroneState field, on the reasoning that a climb rate is a property of how this drone flies, not of where it is right now. Used correctly in the tick body; the preprocessor-substitution link was given, not derived. defined TICK_S themselves and used it at the call site rather than a bare 0.05, after the magic-number argument. Wrote it as `#define TICK_S = 0.05` — an assignment, not a substitution — and fixed it once the expansion `tick(&drone, = 0.05);` was spelled out. wrote DRAIN_RATE_PCT_PER_S as the expression (100.0 / 1200.0) so the model stays readable, after the parenthesis rule was given (a macro body is text and can be torn apart by what surrounds it). Took three passes: an empty `#define BATTERY`, then a DARIN typo, then correct. Also renamed TICK_S to TICK_PER_S without moving its two call sites, and reverted it

## functions-over-main
- status: introduced
- depends-on: none
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: asked unprompted why code gets moved out of main, which earned the hard reason — section 4's test program has its own main and can call tick() but can never reach lines buried inside another main, so code in main is untestable forever. Answer received, not yet applied under their own steam

## simulated-time-vs-real-time
- status: practicing
- depends-on: delta-time, main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: predicted correctly that an unthrottled loop would "print really fast and the altitude will shoot up", then asked unprompted why it reached ~2000 m in a second — the right question. Worked the arithmetic with one correction (first said ~2000 ticks, forgetting each tick adds 0.1 not 1, then got 20,000 unaided). The punchline was delivered, not derived: TICK_S is a claim the code asserts, not a measurement, so 20,000 ticks x 0.05 s = 1000 simulated seconds per real second. Task 2.3 is the fix. 2026-09-15 closed the gap with usleep and then verified it independently, unprompted: counted 10 m in 5 s against a 2 m/s climb rate. Asked for the arithmetic to be walked through pass by pass, which landed the cancellation — tick rate controls smoothness, not speed

## interrupt-signal-ctrl-c
- status: introduced
- depends-on: main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: stopped their first runaway infinite loop with it; named afterwards as an interrupt signal asking a running program to stop. Used, not yet reasoned about

## c-standard-vs-posix
- status: introduced
- depends-on: preprocessor, compiling-c
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: arrived via a real build failure. Met the two-rulebooks split — ISO C defines the language and knows nothing of clocks or an OS, POSIX defines the Unix system calls — and why -std=c11 is a promise that makes glibc hide every non-ISO declaration. Explanation was requested twice and delivered; the learner supplied the conditional-compilation half of it themselves. Not yet reasoned about independently

## implicit-function-declaration
- status: introduced
- depends-on: compiler-warnings
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: hit it for real on usleep. Learned that pre-1999 C allowed calling an undeclared function and assumed an int return — the same family of silent-wrong-guess problem as the earlier %d-on-a-double break — and that -Werror is what turns it from a scrollable warning into a stop

## git-amend
- status: introduced
- depends-on: git-commit, git-staging-area
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: amended three times in one sitting — message rewrite, trailer removal, then a content fix. Saw that amend does not edit a commit but builds a new one, so the hash changes every time, and that this is safe here only because nothing is pushed. The "amend freely before a push, think hard after" rule was given, not derived

## conditionals
- status: practicing
- depends-on: main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-16
- evidence: first if statement in the project. Wrote the shape unaided — condition, comparison operator, braces, assignment inside — needing only the correction from a bare `battery` to `d->battery_percent`. Met the comparison operators and the `==` vs `=` trap (assignment inside a condition is legal C, which is part of why -Wall is on); the trap was explained, not encountered. 2026-09-16 built a three-branch if / else if / else chain. Answered the ordering question before writing any code — a climb check placed first would win and the battery branch would be dead. Needed the chain shape scaffolded into the file after writing two separate ifs, then filled every condition and body themselves. Met the bare-truthiness trap: `else if (d->altitude_m)` compiles and silently means "altitude is not zero"

## clamping
- status: practicing
- depends-on: conditionals, battery-model
- introduced: 2026-09-15
- last-reviewed: 2026-09-16
- evidence: watched the battery reach -29 % and identified correctly that the arithmetic was fine and the model was not. Wrote the lower-bound clamp. The general idea — subtraction knows nothing about physical limits, so they have to be stated — was given. Upper bound (battery cannot exceed 100) is not yet guarded; nothing adds charge yet, so it comes due if a charging or regeneration model ever appears. 2026-09-16 wrote the ceiling clamp to match their own floor clamp, and moved a misplaced clamp to sit after the move it guards once told what a clamp is for

## units-in-names
- status: introduced
- depends-on: floating-point-numbers
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: a real gap surfaced. Read TICK_S (0.05) as ticks per second rather than seconds per tick, and pushed back on the correction. Landed via dimensional analysis on their own code — metres = (metres/second) x dt means dt must be seconds — plus the observation that usleep takes a duration. The rule given: "per" means divided by, so a name with no division in it should carry no "per". Same family as the day-one speed_ms / speed_mps catch, which was also missed first time

## accelerating-a-slow-phenomenon
- status: practicing
- depends-on: battery-model
- introduced: 2026-09-15
- last-reviewed: 2026-09-16
- evidence: a 20-minute bug cannot be watched, so the drain constant was temporarily set to (100.0 / 5) to make it appear in seconds. Technique was dictated, not derived; the learner applied it, saw -29 %, and restored the real rate afterwards. Previews section 4, where a test asserts the same edge case instantly and permanently. 2026-09-16 reached for the technique unprompted on the next task — proposed exaggerated constants for a 27-minute flight without being told to. Needed help working the timeline arithmetic, then ran it and confirmed each phase landed where predicted. Met the failure mode: badly chosen test numbers can make a whole branch never execute, so the run tests less than it appears to

## logical-operators
- status: understood
- depends-on: conditionals
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: self-reported prior knowledge — demonstrated rather than claimed. Wrote `d->battery_percent <= 0 && d->altitude_m > 0.0` unprompted, ahead of the lesson, and used it correctly to express "flat AND still airborne". Treated as exercise for the rest of the session rather than taught

## implicit-state
- status: introduced
- depends-on: conditionals
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: found two bugs from one cause, and traced both by hand. First the landing flicker (0.0, 0.1, 0.0, 0.1 forever): the descent branch required flat AND airborne, so a flat-and-landed drone fell through to a climb branch that never asked about power. Correctly identified which branch ran and proposed the guard as the fix. Then the teleport (0.0 to 100.0 in one tick) from an else that assigned rather than held, and answered correctly that a hold branch should be empty. Asked for a review of the whole implementation unprompted, which earned the naming: the situation is never written down, only inferred from two numbers every tick

## comments-that-lie
- status: introduced
- depends-on: none
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: 2026-09-16 met it a third time, on their own new code: the three Waypoint field comments were copied from DroneState and still read "position east of the launch point" inside a struct that describes a target, not the vehicle. Rewrote them as "target position ..." once it was pointed out. Still flagged rather than self-caught. Earlier: twice in one sitting — a ceiling clamp labelled "Altitude cannot be less than 0", copy-pasted from its floor twin, and a battery comment still claiming "about 20 minutes (1200s)" over a value changed to (100.0 / 10) for testing. The rule given: a wrong comment is worse than none, because the reader trusts it over the code. Flagged, not yet independently caught

## for-loop
- status: practicing
- depends-on: main-loop
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.1. Given the three-part shape (start / keep-going test / step) once, wrote `i < MISSION_WAYPOINT_COUNT` unaided and correctly — the `<` not `<=` boundary landed without prompting. Contrast with the `while(1)` flight loop, which never ends, drawn but not yet quizzed. Separately showed a good debugging instinct: temporarily flipped `while(1)` to `while(0)` so the program would exit and the mission lines could be read without scrolling

## array-decay-to-pointer
- status: introduced
- depends-on: pointers
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: met through gcc rather than explanation — nine copies of `'(Waypoint *)&mission' is a pointer; did you mean to use '->'?` on their own mistake. Shown that an array's name in an expression collapses to a pointer to its first element, which is why `.` failed there and why `d->field` is right in tick(). Also shown that gcc's suggested fix (`->`) was wrong: the compiler diagnoses the symptom, never the intent

## array-length-is-not-stored
- status: introduced
- depends-on: arrays-of-structs
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: asked directly and unprompted what "the array can't tell you its own length" means — a good question at the right moment. Given the contrast with Python's len()/Java's .length (a number stored beside the elements, which is what makes IndexError possible), the compile-time-only `sizeof(a)/sizeof(a[0])` escape hatch and why it dies at a function boundary, and why every C API that takes an array also takes an `n`. Connected to MISSION_WAYPOINT_COUNT as their own `n`. Not yet independently applied
