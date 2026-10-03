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
- last-reviewed: 2026-10-01
- evidence: self-reported — currently attempting to implement pointers and reading up on them. 2026-09-14 asked directly about `*` in declarations vs expressions and about `struct Node *next`; was given the address/dereference distinction, the `->` shorthand, and the pass-by-value argument for why section 2's tick function must take a DroneState *. Then wrote print_state(const DroneState *d) themselves — six `d->field` reads and a `print_state(&drone)` call site, all correct first time, no prompting on the `&`. 2026-09-15 wrote the tick signature `void tick(DroneState *d, ...)` from the print_state pattern, and every call site as `tick(&drone, 0.05)` — six correct `&` uses, no prompting. 2026-09-15 wrote the clamp as `if (battery < 0)` — a bare name with nothing in scope — and corrected to `d->battery_percent` once reminded that only d and dt exist inside tick(). Struct fields are not loose variables; the pointer is the only route in. 2026-09-16 wrote `D->altitude_m` with a capital D; C is case-sensitive and there is no such name 2026-10-01 spaced review (15 days): passed in part — said by-value makes a copy inside the function and that `.` replaces `->` (guessed correctly that -> means go to the address, then take the field); missed that the call site drops the `&`. Then challenged why print_state takes a pointer at all when it only reads — a fair point; told by-value would be fine for a 56-byte struct and the pointer is convention + consistency. 2026-10-02 did not follow array-to-pointer conversion at first ('i dont get what you mean'); needed the memory-address picture, then asked why &mission[current_wp] needs & when mission does not — the right contrast. Chose on his own to pass &mission[current_wp] rather than the whole array, and renamed the parameter target.

## const-correctness
- status: understood
- depends-on: pointers
- introduced: 2026-09-14
- last-reviewed: 2026-09-25
- evidence: 2026-09-17 retrieved it cold, three days later, with no prompting: asked what type tick's new third parameter should be, answered `Waypoint * w`, then when asked whether it should be const, answered "no it should be const" — reasoning from the fact that tick writes the drone and only reads the waypoint. Earlier: used `const DroneState *d` on print_state after the rationale — a compiler-enforced promise not to modify, and the contrast that will make section 2's un-const `update(DroneState *)` legible. Applied as given, not yet independently reasoned about. 2026-09-15 reasoned it out independently before being told: asked for the tick signature, dropped the const and said why — "dronestate isnt const because we are chaning it". The contrast the section-1 lesson predicted would land, landed 2026-09-25 spaced review (8 days): passed — said without const a serialiser that wrote d->speed_mps would alter the real flying drone, and const prevents it. Did not say WHEN const catches it (at compile time, the build refuses), so that half was supplied.

## struct
- status: understood
- depends-on: none
- introduced: 2026-09-14
- last-reviewed: 2026-09-20
- evidence: 2026-09-16 wrote the Waypoint struct in drone.h from the DroneState pattern — three double fields with units in the names — but left the type off the third (`altitude_m;` alone), and fixed it after being told that two of the three lines had something the third was missing. Earlier: wrote the six-field DroneState unaided from a two-field pattern, units carried in the field names, and initialised it with designated initialisers. Caught nothing wrong with `speed_ms` until it was pointed out that `ms` reads as milliseconds; renamed to `speed_mps` 2026-09-20 retrieved it cold after six days: asked why DroneState and Waypoint both carry an altitude field rather than being merged, answered that one is data about the drone and the other is where it is heading — the identity distinction, in their own words, unprompted.

## header-files
- status: practicing
- depends-on: struct
- introduced: 2026-09-14
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 asked three times, in three different framings, why headers exist when drone.c can be linked from anywhere - the question that had to be answered from the compiler's side (it compiles one .c file alone and can never look at another) rather than the linker's. wrote src/drone.h and included it from drone.c with quotes rather than angle brackets, after the "next to this file" vs "system directories" distinction. 2026-09-20 review: correctly placed a new type in drone.h rather than drone.c, but could not say why - "it seems like it belongs there, like its just good practice". The rule (a header holds what more than one .c file must agree on; the .c holds the private machinery) was given again, grounded in section 4's test program needing DroneState. Right answer, no rule behind it yet - re-review

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
- status: practicing
- depends-on: manual-memory-management
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 pushed hard on uninitialized locals rather than accepting the rule - asked for an example, then asked outright "how can anything be in DroneState a;". That is the real crux, and it took the reframe that declaring a variable claims existing memory rather than creating fresh memory. Ran the experiment: DroneState ghost; + print_state(&ghost), hit -Werror=maybe-uninitialized, rebuilt without the warning flags, and saw POS -nan, 0.0 ALT 0.0 - mostly zeros from the OS's wiped pages, with one field holding scribble left by the C runtime's startup code. Also reasoned correctly about the array-bounds half: asked what stops &mission[3] once break is removed, answered "the while condition being false"

## arrays-of-structs
- status: practicing
- depends-on: struct
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.1. First attempt put the array's own name inside its initializer — `{mission.x_m = 10, ...}` — held to across three corrections, which surfaced the real question they were asking: how do you say WHICH slot a value goes in. Asked unprompted how other languages initialise arrays, and separately why C uses `{ }` rather than `[ ]`; both were answered (position is the index; `[ ]` is already taken by declaration and subscript, and `[0] =` is C's slot designator). Wrote the three-waypoint literal correctly once the prefix was removed. Then wrote the full read side — `mission[i].x_m` and its two siblings inside a four-slot printf — after being shown one of the three arguments

## enums
- status: practicing
- depends-on: none
- introduced: 2026-09-20
- last-reviewed: 2026-09-25
- evidence: 2026-09-20 wrote the FlightMode enum (MODE_TAKEOFF, MODE_NAVIGATE, MODE_COMPLETE), added the FlightMode mode field to DroneState, and set it explicitly in main()'s initializer. Predicted correctly, before writing any of it, that an unmentioned field would come out 0 and that 0 would mean MODE_TAKEOFF because it is listed first - and accepted that being right by accident is fragile under reordering. Asked unprompted whether a MODE_OFF was needed; was given the test (is there a situation no name describes?) and shown the real hole it exposes - a battery-flat drone still reporting MODE_NAVIGATE. Typo'd NODE_COMPLETE and fixed it once told, after being shown that the compiler would only complain later, at the use site. 2026-09-24 appended MODE_FAILSAFE at the END of the enum and dropped the count from the comment above it so it cannot go stale. Moved it to the top unprompted to test the question, saw a test fail and didn't know why. When pointed at claim 7's drone, which sets no .mode, answered correctly that it would start as 0, now MODE_FAILSAFE. Was shown that claims 3-8 depend on MODE_TAKEOFF being 0 without saying so 2026-09-25 wrote printf("%s", drone.mode) expecting the mode's name; -Wformat refused it. Was told an enum's names exist only in source and compile to 0..3. Then said unprompted that a raw number is readable by a server but confusing to a person; the reorder risk to a separate reader was supplied, tied to his own 4.4 claim-7 break.

## printf-format
- status: practicing
- depends-on: none
- introduced: —
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 proposed his own way to put PASS/FAIL in one column: left-align the description with a large width. Wrote printf("%-90s PASS
", description) himself. 2026-09-16 built the mission-announcement line in task 3.1 incrementally — label first, then widths, then arguments — and landed a correct four-slot line (`%d` for the int index, three `%5.1f`/`%4.1f` for doubles) with matched arguments in order. En route left a `%.1f` with no argument behind it, which is the variadic hazard they already knew in theory meeting their own code. Earlier: self-reported — built a CLI number toolkit in C; 2026-09-14 wrote drone.c unaided but omitted the trailing newline the spec asked for, then added it after reasoning about why it matters downstream. Later met the variadic-function consequence: printf has no type information for its arguments, because the types are decided by a runtime string, so a wrong specifier cannot be converted or caught by the language itself. Task 1.4 went deep on formatting: used `%%` for a literal percent unprompted, predicted correctly that growing values would shift the columns, then worked through field widths over several passes — that a width is a MINIMUM and printf never truncates, that numbers right-align and text left-aligns, and that literal text between conversions is not counted in any field width. Asked good questions at each step rather than accepting the rule

## floating-point-numbers
- status: practicing
- depends-on: none
- introduced: 2026-09-15
- last-reviewed: 2026-10-02
- evidence: met doubles as the type for every physical quantity in DroneState, and the reason integers were wrong for a 0.05 s tick. 2026-09-20 met NaN for the first time via the ghost experiment, then answered the follow-up correctly: asked what a clamp written as if (d->altitude_m > target->altitude_m) does when altitude is NaN, said the branch does not fire because the comparison is false. Was shown the sharper version - every branch in the chain declines and the empty hold else catches it, so the drone reports holding steady at an altitude of NaN. The general rule (NaN makes every comparison false, so guards fail open) landed; isnan() parked for section 4. 2026-09-22 saw the storage approximation produce a visibly wrong answer in his own test and asked for it in plain terms - got the kitchen-scale framing: a stored decimal is the nearest value the hardware can represent, so every arithmetic step can nudge the answer by a sliver 2026-10-02 predicted that adding 0.05 sixty times gives exactly 3.0 ('i would expect it to, but i also remember something weird happens'), asked how to test it directly, wrote a throwaway /tmp/drift.c himself and saw 2.99999999999999733546 / FALSE. Given why (0.05 repeats in binary, each add rounds). Answered 'int' for a type that never rounds on +1 and 'multiply by TICK_S' to get seconds — one rounding instead of sixty.

## compiling-c
- status: practicing
- depends-on: compiled-vs-interpreted
- introduced: —
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 review passed cold: predicted an unused variable under -Werror becomes a compile failure and that no claim runs, because there is no binary; could not recall what -Wall and -Wextra switch on, refreshed. Earlier: self-reported — compiles and runs C from the terminal with gcc; 2026-09-15 recalled the full gcc line from memory into the Makefile recipe, flags and all bar -Werror, which was restored for them; 2026-09-14 built src/drone.c in WSL with -Wall -Wextra -std=c11 and ran it. Also met the Linux/Windows difference — Linux marks a file executable with a permission bit, Windows with the .exe suffix — and asked a good unprompted follow-up about when .exe would appear

## compilation-stages
- status: practicing
- depends-on: compiling-c
- introduced: 2026-09-14
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 built the model unprompted and in his own words - "we compile one .c file. in that c file we make it object with holes like i expect tick to be filled in with this shape. linker will fill these in with the other file that defines it." Predicted correctly that `gcc -c` on a file calling an undeclared tick would still succeed, then predicted the exact nm symbols before running - U for tick, T for main - and got both right. Also predicted that adding the declaration to drone.h would silence the warning while leaving U tick untouched. Needed one correction: said main.o's holes are filled by drone.c's holes rather than its definitions. 2026-09-16 the link stage stopped being a name on a list and became an error they had predicted: asked whether `make` would succeed with math.h included and sqrt called, answered "no it wont because sqrt doesnt have a defintion" before running it. Then read the failure themselves and saw it came from /usr/bin/ld, a different program from gcc, after stages 1-3 had all succeeded. Earlier: predicted a .obj file would be left behind after compiling; corrected to the four stages (preprocess, compile, assemble, link) that gcc runs in one command, deleting the intermediate. Object files reappear deliberately in task 1.5

## compiler-warnings
- status: practicing
- depends-on: compiling-c
- introduced: 2026-09-16
- last-reviewed: 2026-09-25
- evidence: 2026-09-24 commenting out the climb cap left remaining_alt unused; -Werror stopped the build and he fixed it himself by commenting out the declaration too, without asking. Earlier: 2026-09-16 met -Wall -Wextra -Werror in the Makefile as a deliberate choice rather than noise. 2026-09-20 had two of them fire for real. First -Werror=maybe-uninitialized on the ghost experiment; read it together, including that "may" means the compiler could not prove it, that the bracketed name identifies the specific check, and that the net has holes once a pointer crosses a function boundary. Then hit -Werror=unused-value on his own d->mode == MODE_NAVIGATE; and self-corrected from the code rather than the message - recognised he had written a comparison where he meant an assignment, without reading the error 2026-09-25 hit -Wformat (%s given an enum) and -Wreturn-type (switch with no fallback return). Was told -Wswitch only checks enum coverage on a switch WITHOUT default, which is why the fallback return goes after the switch rather than in a default case.

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
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 rewrote the whole file for the two-program split: both .c files on the drone rule, a real-file target for the test binary, a phony test action that runs it, and rm -rf for a directory. Copy-paste left src/main.c on the test's gcc line at one point - a test that would have silently built and run the drone instead. wrote the first rule themselves after a worked example in another domain. First prerequisite list named only src/drone.c; when asked about the header, reasoned correctly that #include pastes drone.h in regardless — true of gcc, but missing that gcc is never invited if make sees no change. Then predicted the conditional sharply — "make will run the gcc if drone.h was in the src file list" — and confirmed it with touch src/drone.h: 'drone' is up to date, followed by a real rebuild once the header was declared. Asked two good structural questions unprompted: where Makefile comes from (program vs file you author), and how make tracks changes (it does not — it reads filesystem mtimes fresh each run). Left the <file> placeholder and three TODO blocks in on the first cleanup pass

## file-timestamps
- status: practicing
- depends-on: makefile
- introduced: 2026-09-15
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 predicted that `touch src/drone.c` would make gcc re-run and named the mechanism - "it cchecks timestamps?" - for a file whose contents had not changed by a single byte. first read "timestamp" as metadata about build reproducibility and said so plainly; after the correction to last-modified time, read the three real mtimes off ls --time-style=full-iso and saw drone sitting 42 seconds newer than drone.c. Met touch as an instrument for changing mtime without changing content

## make-targets
- status: practicing
- depends-on: makefile
- introduced: 2026-09-15
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 resolved his own opening conditional: after naming the binary test_drone he said `test` belongs in .PHONY because "we do not create a binary called test" - the rule applied to a design decision he had made himself hours later. 2026-09-20 review passed cold: asked whether a new `test` target belongs in `.PHONY`, stated the general rule rather than the instance — "if test is a file we are creating" it stays out, "if test is not a file in my directory but rather an action it should go in .phony" — and left the answer conditional on a design decision not yet made. Earlier: asked what was actually wrong with a recipe that both builds and runs, which earned the concrete answers (hostage terminal, a Docker RUN make that never exits, and a target being a noun). The run: drone dependency chain was supplied after a stuck watch, not derived; clean and .PHONY: run clean were written unaided. Predicted correctly that gcc runs exactly once across make clean / make / make run, and why. Then tested the phony-vs-file distinction unprompted — three make runs in a row all fired, two bare makes in a row both said up to date

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
- last-reviewed: 2026-09-24
- evidence: derived the drain rate from a hardware spec rather than inventing it — given "a small quadcopter flies about 20 minutes", answered 0.083 %/s and showed the working (100 / 1200) unaided. Wrote `d->battery_percent -= DRAIN_RATE_PCT_PER_S * dt;` from the altitude line. Predicted correctly and unprompted that an unclamped battery would go negative, at about -25 % after 25 minutes, and later that a flat battery would not stop the climb because nothing connects the two. 2026-09-24 review (cold): time to empty from 50 % - answered "50 % / drain rate", the right shape, but did not recall the rate's value (100/1200 %/s) or turn it into a number (600 s)

## deterministic-simulation
- status: introduced
- depends-on: fixed-timestep
- introduced: 2026-09-22
- last-reviewed: 2026-09-25
- evidence: named while choosing the test tolerance - the engine has no randomness and no sensors, so 10 m/s for 1 s is exactly 10 m by hand, which is why any difference above binary rounding means the engine is wrong rather than imprecise. Explained to him; not yet demonstrated by him 2026-09-25 picked mission time over wall-clock time for the contract but said "not sure what goes wrong"; given the reason (a test needs a known expected value, and wall clock changes every run). Picked the right option but could not give the reason yet.

## tick-rate-vs-update-rate
- status: introduced
- depends-on: simulation-tick
- introduced: 2026-09-13
- last-reviewed: 2026-09-15
- evidence: correctly said a 5 Hz dashboard reading a 20 Hz engine is not wrong, it just misses in-between states — reached "sampling" unprompted. 2026-09-15 saw the extreme case for real. 2026-09-15 asked unprompted whether the 50 ms was modelling telemetry relay rate — close enough to earn the real split: 50 ms is the control-loop rate and the physics step size fused into one number, while telemetry downlink is a separate, slower clock (1-10 Hz). Also met the point that a real flight computer samples reality while this program has to manufacture it

## vectors-and-distance
- status: practicing
- depends-on: floating-point-numbers
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.2. Given dx as the pattern, wrote dy and `return sqrt(dx*dx + dy*dy)` themselves. En route wrote `dy = w->y_m - d->x_m` — a copy-paste slip that is type-correct and meaning-wrong, invisible to the compiler; fixed after being asked to read the two lines side by side. Result checked against hand arithmetic (10,40 from origin -> 41.2 m) by the agent, not by them. Asked afterwards why the printed distance never changes, answered "because the current wp is stuck at 0" — right about the waypoint end, but missed that nothing in tick() has ever modified x_m or y_m, so the drone end is frozen too

## heading-and-direction
- status: practicing
- depends-on: vectors-and-distance
- introduced: 2026-09-16
- last-reviewed: 2026-09-25
- evidence: 2026-09-17 task 3.4 — wrote the sin/cos movement lines and the degrees-to-radians conversion (the inverse of 3.3's, which had been given to them), all three correct. Predicted the overshoot before running: "it arrives but keeps going then turns back around", which is exactly what happened — the bearing flipping 14.0 -> 194.0 and oscillating forever. Earlier, task 3.3. Wrote the negative-angle fold themselves (`if (degrees < 0) degrees += 360;`) and the return, after the -90-is-also-270 framing; the degrees conversion line itself was given as a hint after a long stall. Predicted the heading to WP0 (10 east, 40 north) as "around 90 degrees, top right quadrant" — quadrant right, scale wrong; when asked which axis dominates, corrected unprompted to "below 45". Real answer 14.0 deg. Also extended the telemetry printf to a five-argument line with a second function call in it, unaided 2026-09-25 review passed: heading 180 -> y_m changes and goes negative, x_m untouched, from memory.

## waypoint-list
- status: practicing
- depends-on: arrays-of-structs
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.1. Chose their own three-waypoint route (10,40 / 50,25 / 95,75) and wrote it as a Waypoint array in main(), announced at startup before the flight loop. Picked waypoint altitudes of 200-400 m against a CRUISE_ALTITUDE_M of 100 — flagged for them as something task 3.4 has to reconcile, not yet resolved

## arrival-threshold
- status: practicing
- depends-on: vectors-and-distance
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 computed the per-tick step unaided (0.5 m from 10 m/s x 0.05 s) and predicted that an equality test would sometimes land on zero but usually step over. First pick for the radius was 0.10 m — smaller than the step — and when walked through the 0.4 m case answered correctly that the drone would end up 0.1 m past, i.e. never inside the circle. Corrected to 5. Wrote the arrival condition itself: distance_to(&drone, &mission[current_wp]) <= ARRIVAL_RADIUS_M.

## finite-state-machine
- status: understood
- depends-on: enums
- introduced: 2026-09-20
- last-reviewed: 2026-09-24
- evidence: motivation earned the hard way on 2026-09-16, before the concept was taught. Two separate bugs in section 2.5 came from the same cause - the drone's situation was inferred from number combinations rather than stored - and the learner diagnosed both by tracing the chain by hand. 2026-09-20 built it: asked what MODE_TAKEOFF should do, answered the whole design unprompted - rise to target altitude, do not move forward, transition on reaching it. Wrote the mode gate on the navigation branch (d->battery_percent > 0.0 && d->mode == MODE_NAVIGATE) and the transition rule at the end of tick(). Then wrote the main() half ahead of the scaffolding being offered: while (drone.mode != MODE_COMPLETE) replacing while(1), and drone.mode = MODE_COMPLETE replacing break. Mission now flies TAKEOFF -> NAVIGATE -> COMPLETE end to end. 2026-09-24 (4 days later) added the failsafe transition inside the existing battery clamp at the top of tick() and argued for the placement unprompted: the battery is detected there, so there is no need for a second transition in the descent block. Traced correctly that nothing in tick() ever takes a drone out of MODE_FAILSAFE (it latches)

## test-is-a-claim
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-09-13
- evidence: explained during the testing decision — a test is a claim about behaviour written down so a machine can check it

## assert
- status: practicing
- depends-on: test-is-a-claim
- introduced: 2026-09-21
- last-reviewed: 2026-09-24
- evidence: 2026-09-23 predicted "nothing else after 4 runs" when claim 4 aborted, but cited the wrong evidence (a MODE NAVIGATE line that came from claim 1); was shown that the proof is what's missing. He then read a later MODE NAVIGATE line as the mode assert having run, and was told a passing assert prints nothing, so every MODE NAVIGATE comes from the printf inside tick(). Learned that abort() hides every claim after the first failure (relevant for 4.5). Earlier: 2026-09-21 chose his own first claim in English - "after a tick, battery should go down" - then wrote it unaided as assert(battery_before > d.battery_percent), having first set up a drone with .battery_percent = 100 because {0} was the wrong situation to test. Predicted the failure shape before breaking the engine: "maybe an error. printtf all test passed doest get printed" - both correct. 2026-09-22 wrote his second and third asserts, and read a real failure message line by line: asked what "core dumped" meant, and was walked through assert expanding to __assert_fail, which prints expression/file/line from inside his own process, then abort() raising SIGABRT, the OS reporting it, and make propagating the nonzero status

## test-program
- status: practicing
- depends-on: test-is-a-claim
- introduced: 2026-09-21
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 (task 4.5) named the goal gap himself: of his three parked notes, 1 and 3 (abort hides later claims) block a whole-picture run. Proposed PASS/FAIL per check instead of aborting, then a failure count, then a reusable check() taking the condition and a description and returning 1/0. Chose to pass the condition as what SHOULD be true, reasoning 'easier to think for the happy path'. Asked for PASS lines too; given the 4.4 claim-9-never-called case as the reason they earn their noise. Claims 1-6 and 8-10 were converted for him on request, and he reviewed and approved the diff. 2026-09-21 proposed the shape before any guidance: a tests/ directory, make test compiling and running a program, pass/fail output. Chose a single test file over per-aspect files once told the split pays off only when running a subset saves time. Reasoned out unprompted that the test must link the real engine rather than a copy - when asked what a duplicated tick would report after a later bugfix, saw that the test goes stale. —

## test-fairness
- status: practicing
- depends-on: test-is-a-claim, assert
- introduced: 2026-09-22
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 break-checked the new suite twice with predictions. Removing the climb cap exposed that `failed = check(...)` twice keeps only the last verdict (FAIL printed, then 'all tests passed'); he named the cause, 'second check overrides it', and switched to +=. Then predicted exactly that commenting out the TAKEOFF->NAVIGATE assignment fails claims 7 and 8 and nothing else, and it did, while the engine's printf still said MODE NAVIGATE. 2026-09-23 claim 3 was green but UNFAIR: alt_after read claim 1's grounded drone, so the assert was really 10 > 0. He predicted it would fail with the descent line commented out, watched it still pass, then traced the cause himself ("alt_after is reading d instead of d_dead_drone"), fixed it, and confirmed red-then-green. Break-checked claims 4, 5, 6, 7 and 8 with a correct prediction each time (including numbers: 102 m, 97.5 m). On claim 6, his <= break still passed and he explained why unprompted (the cap trims the step to remaining = 0). He was given the principle that tests check behaviour, not code, and needed both guards broken to see red. Earlier: 2026-09-22 asked what "fair" even means, and first guessed it meant checking that no other field changed. Given the definition - a test that would fail if the engine were wrong - he applied it himself: asked whether any wrong engine could still pass while the waypoint sat at x = 10 and the expected answer was also 10, answered "a faulty engine that keeps drones at x = 10", and moved the waypoint out to 100 so the expected value cannot be copied from the target. Then proved the finished test fair by dropping CRUISE_SPEED_MPS to 5.0, watching the assertion abort, and restoring it

## test-isolation
- status: practicing
- depends-on: test-program
- introduced: 2026-09-22
- last-reviewed: 2026-09-22
- evidence: 2026-09-22 began claim 2 by typing "d." - reaching for the drone claim 1 had already ticked. Asked what state that drone was actually in, and what would happen to claim 2 if claim 1 were later edited, he answered both correctly ("it is navigation mode. it will be in a different position if a waypoint was provided in claim 1") and declared a fresh d_nav_east / w_nav_east pair instead. Also met gcc's "redefinition of d" when the new names collided with the old, and read the accompanying note line pointing at the first definition

## choosing-a-tolerance
- status: practicing
- depends-on: nan-and-float-comparison
- introduced: 2026-09-22
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 asked cold whether the cap claim should use == or a tolerance: chose == "because we would be using the cap that makes it exact" - right call, mechanism-level reason. The stronger reason (exactness IS the claim, because the transition needs the exact value) was supplied. Challenged "did you actually run 10 million" - verified: a throwaway program found 0 of 10M near-target pairs where alt + (target - alt) != target. Also chose == for claim 6 (the else does no arithmetic, so nothing can drift). 2026-09-22 pushed back on the number three times - "should tolerance be like 0.01?", "why not just within 1 metre", "so why 1e-9" - which is the right question to ask about a magic number. Was given the two walls (far above the rounding noise floor, far below the smallest error worth catching) and the honest answer that 1e-9 is a round place to stand in a very wide safe band, not a derived value. Wrote TOLERANCE_M 1e-9 into the test himself. The reasoning was supplied rather than produced

## edge-cases
- status: practicing
- depends-on: test-is-a-claim
- introduced: 2026-09-23
- last-reviewed: 2026-09-24
- evidence: 2026-09-23 needed three reframings before the goal clicked ("im not sure what were asking"); it landed on "every if is a fork, and bugs live at the value where it switches". Then listed all seven forks in tick() accurately (one misread, < for <=, corrected on a prompt to reread character by character), and ranked them by consequence unprompted: dead-battery descent first ("drone would not descend on a dead bat"). Missed the step caps and the transition on the first pass. Asked what happens if the climb cap were deleted 0.03 m below target, he traced it past the first tick to an up/down oscillation that never settles (arithmetic slip: said 0.7 overshoot for 0.07). Located the 115/125 pivot as the target altitude and said the else holds it there. Designed and wrote claims 3-8 at the forks: battery exactly 0, drone at exactly target, drone inside one step of target for both caps. Unprompted, he raised whether a battery of 0.000001 % should really count as powered, which is a design question (a reserve threshold), not a boundary bug. Parked for 4.4

## regression-testing
- status: practicing
- depends-on: test-is-a-claim
- introduced: 2026-09-24
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 when asked what a green suite proves after a change, answered that it only shows the existing claims still hold, and 'it doesnt say if our battery mode trans worked' - so a new behaviour needs its own new claim. Did not come up with the baseline run (run make test BEFORE the change); that was supplied. In 4.4 the old claims were green before and after the change

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
- last-reviewed: 2026-09-25
- evidence: chose the two-program split over a single program after seeing the tradeoff; understood the engine flies the drone and knows nothing about the internet 2026-09-25 asked "the server runs the c program? ... are we making a web server?" — the picture had faded since planning day. Given the three-box diagram (C prints JSON lines -> Node child process reads stdout -> WebSocket -> page). Due for review before section 6.

## stdout-as-stream
- status: introduced
- depends-on: none
- introduced: 2026-09-13
- last-reviewed: 2026-10-02
- evidence: explained that a program writes to standard output without knowing where it lands; the terminal is a default, not a law 2026-09-25 was told the reader of the output becomes the section-6 server, a machine reading line by line. 2026-10-02 met 'Broken pipe' from `make run | head -12`; told it is head closing the reading end so the writer is stopped — a preview of section 6, where the server is the reader.

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
- status: practicing
- depends-on: process-architecture-two-programs
- introduced: 2026-09-25
- last-reviewed: 2026-10-02
- evidence: trunk component #5 — both sides must agree on field names and units or nothing on screen moves 2026-09-25 first real contact: asked unprompted "who are we announcing to when we transition" - the question the contract answers. Agreed the mode must travel as text, not as an enum number whose meaning lives in drone.h. 2026-09-25 task 5.2 — proposed the first fields themselves; accepted the mode travels as a string after reasoning that an enum number would be read wrongly at runtime (missed that insertion shifts every later value — shown the v1/v2 table). Chose mission time over wall clock but could not say why until given the determinism argument for 5.5. Wrote "every line carries every field" as the second promise after being led to what a missing key does in JS. Raised speed-vs-velocity unprompted, which exposed that speed and distance are both horizontal-only; renamed to horizontal_speed_mps and fixed the distance meaning. Asked for the architecture mid-task — the engine -> server -> page pipeline was not in their head before today. 2026-10-02 noticed unprompted that heading 0.0 and bearing 14.0 disagree during takeoff, asked the difference, then chose to add a bearing field to the contract rather than lose it ('it locates where a target is relative to the drone, not where the drone is heading'). Changed the contract BEFORE the code and updated the example line himself so the doc keeps its own every-field promise. Rename to bearing_to_waypoint_deg was suggested by the tutor and applied to the doc by the tutor at his request.

## json
- status: practicing
- depends-on: data-contract
- introduced: 2026-09-25
- last-reviewed: 2026-10-02
- evidence: 2026-09-25 task 5.2 — given the syntax rules (quoted keys, bare numbers, no trailing comma). Wrote seven entries of the example line with correct syntax; left a stray "altitude": 0 from the old name (different key, not a duplicate — the tutor first framed it wrongly as a duplicate and corrected itself). The last three entries and the closing brace were written for them, as agreed. Told JSON.parse keeps the last of two duplicate keys silently; not yet checked. 2026-10-02 task 5.3 wrote the eleven-field format string in print_state himself, escaping every inner quote as \" unprompted, keys in contract order, trailing newline. Missed one key's unit suffix (bearing_to_waypoint vs _deg) and fixed it on a 'read it letter by letter against row 24' prompt.

## serialization
- status: practicing
- depends-on: json
- introduced: 2026-10-01
- last-reviewed: 2026-10-02
- evidence: 2026-10-01 task 5.3 first contact: proposed renaming every field to the contract, splitting position, and taking flight_mode from mode. When asked which fields print_state could reach through d alone, named current_waypoint and distance as needing the drone and mission; missed mission time until asked where it lives ('mission time doesnt exist'). Designed the new print_state signature (d, current_wp, target, ticks) and wired both call sites; the first argument list carried a call to a non-existent mission_time_s(), d->x, and an undeclared w, all found by reading compiler errors one at a time.

## line-based-protocol
- status: practicing
- depends-on: data-contract
- introduced: 2026-09-14
- last-reviewed: 2026-10-02
- evidence: asked what a missing newline would do to a line-reading server, answered that it would read the wrong number of lines and get a bad format — right direction, sharpened to the real failure: the newline IS the delimiter, so the reader waits forever for an end that never comes 2026-10-02 read the running output against 'one tick = one line' and named the `-> WP` line as the thing breaking it ('the loop outputs the wp info'). Decided to remove it once he had worked out it carried one value (bearing) the JSON did not.

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
- last-reviewed: 2026-09-25
- evidence: a dashboard may be stale (showing something up to 200 ms old) but should never be wrong (showing something that never happened); stale is a tuning knob, wrong is a bug 2026-09-25 proposed "a timestamp" for telling fresh from stale; then offered "that it is current" as a promise a line can make — corrected: the line cannot vouch for itself, the reader judges freshness from mission_time_s.

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
- status: understood
- depends-on: git-repository
- introduced: —
- last-reviewed: 2026-09-21
- evidence: self-reported — uses git frequently; 2026-09-14 wrote and ran the repository's root commit, message authored themselves in the present-tense convention. 2026-09-21 spaced review after a week away, passed unprompted: a commit "gives a snapshot, something to fall back to, also gives a record of changes" — both halves, the restore point and the history

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
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 replaced two per-binary lines with one directory line, on his own argument that the per-binary list goes stale - which it had, silently, minutes earlier when test_drone escaped both .gitignore and clean. filled in the `*.exe` pattern correctly by generalising from the `*.o` line; needed a second pass to remove the stale TODO block, which prompted a note about leaving finished instructions in files. Later the same day predicted correctly that the new Linux binary would show as untracked because *.exe could not match it, then replaced the dead rule with /drone

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
- status: practicing
- depends-on: git-commit
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 asked outright how to read a diff, then located the hunk where main() left drone.c and read its header - @@ -151,59 +143,3 @@ - correctly identifying it as 56 lines removed. Was stuck inside the pager without knowing it. —

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
- last-reviewed: 2026-09-20
- evidence: CLIMB_RATE_MPS entered as a #define rather than a DroneState field, on the reasoning that a climb rate is a property of how this drone flies, not of where it is right now. Used correctly in the tick body; the preprocessor-substitution link was given, not derived. defined TICK_S themselves and used it at the call site rather than a bare 0.05, after the magic-number argument. Wrote it as `#define TICK_S = 0.05` — an assignment, not a substitution — and fixed it once the expansion `tick(&drone, = 0.05);` was spelled out. wrote DRAIN_RATE_PCT_PER_S as the expression (100.0 / 1200.0) so the model stays readable, after the parenthesis rule was given (a macro body is text and can be torn apart by what surrounds it). Took three passes: an empty `#define BATTERY`, then a DARIN typo, then correct. Also renamed TICK_S to TICK_PER_S without moving its two call sites, and reverted it 2026-09-20 added CONTROLLED_DESCENT_RATE_MPS as its own named constant for commanded descent, distinct from the powerless sink rate.

## functions-over-main
- status: practicing
- depends-on: none
- introduced: 2026-09-15
- last-reviewed: 2026-09-15
- evidence: asked unprompted why code gets moved out of main, which earned the hard reason — section 4's test program has its own main and can call tick() but can never reach lines buried inside another main, so code in main is untestable forever. Answer received, not yet applied under their own steam

## simulated-time-vs-real-time
- status: practicing
- depends-on: delta-time, main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 review passed after 9 days: asked why the test needs no usleep, answered "we are not replicating the 20hz feature of main.c in test but the actual ending behaviour of the drone", and that adding one would only cost waiting time. Was told the rest: tick() never reads a clock, so the results would be identical and only the wait grows with the suite. Earlier: predicted correctly that an unthrottled loop would "print really fast and the altitude will shoot up", then asked unprompted why it reached ~2000 m in a second — the right question. Worked the arithmetic with one correction (first said ~2000 ticks, forgetting each tick adds 0.1 not 1, then got 20,000 unaided). The punchline was delivered, not derived: TICK_S is a claim the code asserts, not a measurement, so 20,000 ticks x 0.05 s = 1000 simulated seconds per real second. Task 2.3 is the fix. 2026-09-15 closed the gap with usleep and then verified it independently, unprompted: counted 10 m in 5 s against a 2 m/s climb rate. Asked for the arithmetic to be walked through pass by pass, which landed the cancellation — tick rate controls smoothness, not speed

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
- status: practicing
- depends-on: compiler-warnings
- introduced: 2026-09-15
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 predicted the failure correctly before compiling - "undeclared or undefined" - naming both stages in one breath, then watched gcc warn and produce a valid object file anyway rather than refusing. hit it for real on usleep. Learned that pre-1999 C allowed calling an undeclared function and assumed an int return — the same family of silent-wrong-guess problem as the earlier %d-on-a-double break — and that -Werror is what turns it from a scrollable warning into a stop

## remote-tracking-branches
- status: practicing
- depends-on: git-commit
- introduced: 2026-09-22
- last-reviewed: 2026-09-24
- evidence: asked "what is head orgin" while amending - the first time HEAD, origin and origin/main had come up as distinct things. Given the pointer model: HEAD is where you are, origin is the name of the GitHub remote, origin/main is a local record of where the remote was at the last push or fetch and does not move on its own, so "ahead 1" is the gap between them. Then read (HEAD -> main) and (origin/main, origin/HEAD) off his own git log output and matched them to the model. Had not yet pushed with this model in hand. 2026-09-24 asked for a walkthrough of how git works, then commit, then amend. Asked whether .git could be hand-edited to change commits (given content addressing: the hash is the filename, and each child vouches for its parent). On amend-after-push, guessed "an error or an overwrite" - both halves right, but said "i dont get it"; was walked through the non-fast-forward rejection and --force. Asked what "ahead 1" signified and was shown it means the commit exists only locally, the green light for amend. Then amended an unpushed commit and pushed it: b5436d5..037cf9f

## git-amend
- status: practicing
- depends-on: git-commit, git-staging-area
- introduced: 2026-09-15
- last-reviewed: 2026-09-22
- evidence: amended three times in one sitting — message rewrite, trailer removal, then a content fix. Saw that amend does not edit a commit but builds a new one, so the hash changes every time, and that this is safe here only because nothing is pushed. The "amend freely before a push, think hard after" rule was given, not derived. 2026-09-22 asked for it cold - "how do i override a commit" - and after a week away still had the core: predicted "the new commit is on top", though unsure whether the old one would still show in the log. Asked what origin and HEAD were, which had not been explained before; given the pointer model (HEAD -> main is you, origin/main is the last record of GitHub, so ahead 1 is the gap) and then read it straight off his own git log output. Amended cleanly to fold in the comment cleanup

## conditionals
- status: practicing
- depends-on: main-loop
- introduced: 2026-09-15
- last-reviewed: 2026-09-20
- evidence: first if statement in the project. Wrote the shape unaided — condition, comparison operator, braces, assignment inside — needing only the correction from a bare `battery` to `d->battery_percent`. Met the comparison operators and the `==` vs `=` trap (assignment inside a condition is legal C, which is part of why -Wall is on); the trap was explained, not encountered. 2026-09-16 built a three-branch if / else if / else chain. Answered the ordering question before writing any code — a climb check placed first would win and the battery branch would be dead. Needed the chain shape scaffolded into the file after writing two separate ifs, then filled every condition and body themselves. Met the bare-truthiness trap: `else if (d->altitude_m)` compiles and silently means "altitude is not zero" 2026-09-20 wrote the missing descent branch as a mirror of the climb branch, getting all three flips right first time: the comparison (altitude above target), the sign (-=), and the direction of the clamp.

## clamping
- status: practicing
- depends-on: conditionals, battery-model
- introduced: 2026-09-15
- last-reviewed: 2026-09-20
- evidence: watched the battery reach -29 % and identified correctly that the arithmetic was fine and the model was not. Wrote the lower-bound clamp. The general idea — subtraction knows nothing about physical limits, so they have to be stated — was given. Upper bound (battery cannot exceed 100) is not yet guarded; nothing adds charge yet, so it comes due if a charging or regeneration model ever appears. 2026-09-16 wrote the ceiling clamp to match their own floor clamp, and moved a misplaced clamp to sit after the move it guards once told what a clamp is for 2026-09-20 wrote the descent clamp with the comparison correctly reversed — coming down, 'past the target' is the other side.

## units-in-names
- status: practicing
- depends-on: floating-point-numbers
- introduced: 2026-09-15
- last-reviewed: 2026-09-25
- evidence: a real gap surfaced. Read TICK_S (0.05) as ticks per second rather than seconds per tick, and pushed back on the correction. Landed via dimensional analysis on their own code — metres = (metres/second) x dt means dt must be seconds — plus the observation that usleep takes a duration. The rule given: "per" means divided by, so a name with no division in it should carry no "per". Same family as the day-one speed_ms / speed_mps catch, which was also missed first time 2026-09-25 named the contract fields with no units at first; when asked what a unit in the name protects against: "ensures the right unit is inferred". Then applied it to every field, and caught position_x/position_y themselves on the "read it against the rule" prompt. Also renamed a seconds field from "tick" to mission_time_s after being asked what "tick": 1.5 would mean.

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
- status: practicing
- depends-on: conditionals
- introduced: 2026-09-16
- last-reviewed: 2026-09-20
- evidence: found two bugs from one cause on 2026-09-16, and traced both by hand. First the landing flicker (0.0, 0.1, 0.0, 0.1 forever): the descent branch required flat AND airborne, so a flat-and-landed drone fell through to a climb branch that never asked about power. Correctly identified which branch ran and proposed the guard as the fix. Then the teleport (0.0 to 100.0 in one tick) from an else that assigned rather than held, and answered correctly that a hold branch should be empty. Asked for a review of the whole implementation unprompted, which earned the naming: the situation is never written down, only inferred from two numbers every tick. 2026-09-20 cured it for takeoff and completion - mode is now stored and read rather than recomputed - and spotted the remaining instance himself by asking whether another mode was needed

## comments-that-lie
- status: practicing
- depends-on: none
- introduced: 2026-09-16
- last-reviewed: 2026-09-25
- evidence: 2026-09-24 claim 8's header was copied from claim 7 and still said "below". He fixed it on a prompt to reread it, not self-caught. The same copy-from-the-previous-claim slip hit code four times this task (claim 3's first draft, claim 5, claim 7's assert, claim 8's header), each time green or silently wrong. Earlier: 2026-09-17 caught one themselves for the first time, unprompted — asked whether `/*Altitude does not change when landed */` was correct on an else branch that the drone actually spends most of its flight in (battery fine, already at target altitude). Reasoned about which cases reach the branch rather than reading the words. That question also surfaced a real bug neither of us had noticed: nothing descends the drone while the battery is healthy, so with WP1 at 400 m and WP2 at 310 m it can never come down. Parked for 3.5. Earlier: 2026-09-16 met it a third time, on their own new code: the three Waypoint field comments were copied from DroneState and still read "position east of the launch point" inside a struct that describes a target, not the vehicle. Rewrote them as "target position ..." once it was pointed out. Still flagged rather than self-caught. Earlier: twice in one sitting — a ceiling clamp labelled "Altitude cannot be less than 0", copy-pasted from its floor twin, and a battery comment still claiming "about 20 minutes (1200s)" over a value changed to (100.0 / 10) for testing. The rule given: a wrong comment is worse than none, because the reader trusts it over the code. Flagged, not yet independently caught 2026-09-20 asked why DESCENT_RATE_MPS could not simply be reused; when told its comment scoped it to the flat-battery case, proposed a separate CONTROLLED_DESCENT_RATE_MPS rather than widening the comment — choosing two honest names over one stretched one. 2026-09-25 names lie too: named the function print_mode though it only returned text, then called it bare and saw nothing print. When asked why the bare call looked finished, answered 'the name says print so i assumed it printed'. Renamed to mode_to_string after proposing 'tostring'; the type prefix (no overloading in C) was supplied.

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
- status: practicing
- depends-on: arrays-of-structs
- introduced: 2026-09-16
- last-reviewed: 2026-09-20
- evidence: asked directly and unprompted what "the array can't tell you its own length" means — a good question at the right moment. Given the contrast with Python's len()/Java's .length (a number stored beside the elements, which is what makes IndexError possible), the compile-time-only `sizeof(a)/sizeof(a[0])` escape hatch and why it dies at a function boundary, and why every C API that takes an array also takes an `n`. Connected to MISSION_WAYPOINT_COUNT as their own `n`. Not yet independently applied 2026-09-20 reached for sizeof(mission) / sizeof(mission[0]) unprompted to bound the waypoint index — the correct idiom, though it duplicated the existing MISSION_WAYPOINT_COUNT; dropped it for the named constant once the two-sources-of-truth problem was named.

## declaration-vs-definition
- status: practicing
- depends-on: header-files
- introduced: 2026-09-16
- last-reviewed: 2026-10-02
- evidence: 2026-09-21 wrote all four engine prototypes into drone.h and found print_state and distance_to himself from the rule "does anything outside this file call it". Took three attempts on the syntax - `{`, then `{}`, then `;` - the empty-body form being the instructive miss, since `{}` is still a definition. Asked unprompted why tick could not simply be extracted into the header, which is the multiple-definition trap. task 3.2, and the strongest prediction of the journey so far. Told only that math.h holds a declaration — "sqrt takes a double and returns a double" — and asked whether make would succeed, answered unprompted: "no it wont because sqr doesnt have a defintion". Honest about the second half too ("im not sure where sqrt would have to come from"), which is what made the linker explanation land on a real gap rather than a hypothetical one 2026-10-02 read 'implicit declaration of function mode_to_string' and diagnosed it himself — 'it's below it not above it' — but believed functions outside main have 'a global view rather than a line by line view'. Corrected: C reads top to bottom and a name exists only from its declaration onward. Chose a forward declaration over moving the function.

## linking-libraries
- status: practicing
- depends-on: declaration-vs-definition
- introduced: 2026-09-16
- last-reviewed: 2026-09-21
- evidence: 2026-09-21 answered cold why sqrt needs -lm but printf does not: "both are in the c library but math isnt linked by default". task 3.2. Hit `undefined reference to sqrt` on their own build and read it as a link-stage failure. Given the rest: that sqrt's compiled code lives in libm, that gcc links libc automatically but not libm, that `-lm` means "link the library named m", and that it must come after the source file because ld resolves left to right and discards a library it has no outstanding references for. The fix and the ordering rule were both dictated, not derived — comes due again the first time a third-party library appears (section 6, npm)

## address-of-array-element
- status: practicing
- depends-on: pointers
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.2. Wrote `distance_to(&drone, &mission[current_wp])` correctly first time after one explanation of the shape — index into the array to get a Waypoint value, then & to take its address. Connects the array work from 3.1 to the pointer work from section 2; no prompting needed on either &

## radians-vs-degrees
- status: introduced
- depends-on: floating-point-numbers
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.3. Given the unit contrast (a full turn is 2*pi radians rather than 360 degrees, and C's trig functions speak only radians) and then the conversion line itself, `radians * (180.0 / M_PI)`, after a fifteen-minute stall on that blank. Applied as given, not derived — comes due again anywhere a turn rate or an angle difference is computed

## atan2-and-quadrants
- status: introduced
- depends-on: heading-and-direction
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.3, explained but not yet demonstrated. Why atan(dy/dx) fails twice — division by zero when dx is 0, and a ratio that cannot tell (1,1) from (-1,-1) — and why atan2 taking the legs separately keeps all four quadrants. The compass trick was given rather than derived: atan2(a,b) measures from the b axis toward the a axis, so atan2(east, north) is clockwise-from-north by construction. The argument order IS the conversion

## angle-normalisation
- status: practicing
- depends-on: heading-and-direction
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.3. Given only the observation that atan2 returns -180..+180 and that a compass says 270 where atan2 says -90, wrote the fix themselves: an if on `degrees < 0` adding 360. The underlying idea — that two numbers 360 apart name the same direction, so a range is a choice of representative — was not stated back in their own words

## integer-division
- status: introduced
- depends-on: floating-point-numbers
- introduced: 2026-09-16
- last-reviewed: 2026-09-16
- evidence: task 3.3, flagged as a near-miss rather than earned. Wrote `180 / M_PI`, which is safe only because M_PI is a double and promotes the 180; pointed out that `180 / 200` between two ints would be 0, not 0.9, and that writing the .0 explicitly is the habit worth having. Not yet hit as a real bug

## trig-components
- status: practicing
- depends-on: heading-and-direction
- introduced: 2026-09-17
- last-reviewed: 2026-09-17
- evidence: task 3.4, and the longest genuine struggle of the journey — five rounds of questions before it landed, every one of them a good question. Accepted the "fractions of a step" framing immediately but rejected hand-waving on where the fractions come from: "i understand the fractions part. i do not understand the sin and cos", then "is it soh cah toa?", then a step further back to "what are we doing, why are we doing d->x_m y_m". Needed the unit-circle definition (walk one metre at angle theta and sin/cos ARE the coordinates you land on) plus the bridge that a ratio over a hypotenuse of 1 is the same number as the coordinate. Arrived at it in their own words: "so we multiply sin by the hypotenuse, which be 1 metre or 0.8 metres... we are trying to find out how east or north". Then wrote all three lines correctly

## bearing-vs-heading
- status: practicing
- depends-on: heading-and-direction
- introduced: 2026-09-17
- last-reviewed: 2026-10-02
- evidence: raised the distinction themselves, unprompted, mid-task: "shouldnt we rename and/or add the name bearing to distinguish between bearing and heading?" — a domain-vocabulary correction the lesson had not taught and the plan had not scheduled. Correct: the function computed a bearing and was named heading_to. Then asked for the difference to be spelled out, so the naming instinct arrived ahead of the full concept. Renamed to bearing_to across all four sites, and separately caught that main's printf still labelled the column HDG. Follow-up question was sharp too — whether bearing_to itself needed changing, which earned the answer that a bearing is purely positional and re-derived every tick 2026-10-02 asked again what the difference is, prompted by seeing heading 0 vs bearing 14 during takeoff — not retrieved after 15 days. Re-given: heading = where the drone points, a fact about the drone; bearing = drone-to-waypoint direction, a measurement between two things; they differ in TAKEOFF because tick() does not steer until NAVIGATE.

## passing-dependencies-as-parameters
- status: practicing
- depends-on: functions-over-main
- introduced: 2026-09-17
- last-reviewed: 2026-09-17
- evidence: task 3.4 opened on the design problem rather than the maths — tick() could not see the mission, so its signature had to grow. Named the parameter type `Waypoint *` unaided and, asked separately, reasoned out const correctly. Then fixed the call site in main to match. The wider idea (a function's parameters are its whole view of the world, and widening that view is a deliberate decision) was demonstrated in one instance, not yet stated back

## loop-control-break
- status: practicing
- depends-on: main-loop, conditionals
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 used break to leave the infinite main loop once the last waypoint was reached, and understood it as the thing preventing an out-of-bounds read of mission[3]. Later the same day replaced it: the loop condition became while (drone.mode != MODE_COMPLETE) and break became an assignment. Answered correctly that the array is now protected by the while condition being false, and was shown the precise mechanism - while tests before each pass, so the dangerous expression at the top of the body is never reached - plus the contrast with do/while, which would read mission[3] once before stopping

## off-by-one-errors
- status: practicing
- depends-on: undefined-behaviour, conditionals
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 wrote a bound guard as `if (current_wp != missionLength) current_wp++`, then traced it on request and stated correctly that at the last waypoint current_wp is 2, 2 != 3 is true, and it increments anyway — proving their own guard fired one step too late. Answered that the run-out check belongs after the increment, not before.

## truthiness-in-c
- status: practicing
- depends-on: conditionals, floating-point-numbers
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 wrote `d->battery_percent` as a bare condition alongside a sibling branch asking `> 0.0`. Asked what the bare name tests, answered "does it have a value?" — close, but the rule is zero is false and anything else is true, so it means != 0. Then applied it correctly: asked which branch would think a battery reading -5.0 still had power, answered the descent branch. Fixed both branches to ask the same question.

## zero-initialization
- status: practicing
- depends-on: struct, undefined-behaviour
- introduced: 2026-09-20
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 asked why claims 3-8 depended on MODE_TAKEOFF being 0 and was re-told (unnamed fields zero, enums count from 0). Decided every claim states its mode explicitly, and spotted unprompted that claim 1 relied on zero too, which the parked note had missed. The seven edits were made for him on request; the suite stayed all-PASS. 2026-09-20 predicted correctly, unprompted, that a field added to DroneState but left out of main()'s initializer list would come out 0, and that this made MODE_TAKEOFF the accidental default. Was then given the boundary: that guarantee belongs to the = { ... } initializer, not to structs generally - a bare local gets leftovers, = {0} zeroes everything, = { .x_m = 5.0 } zeroes everything else. Set .mode explicitly anyway rather than relying on the zero

## stack-memory-reuse
- status: practicing
- depends-on: undefined-behaviour, manual-memory-management
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: 2026-09-20 refused to accept the rule without the mechanism - "BUT HOW CAN ANYTHING BE IN DRONESTATE A;" - which is exactly the right place to get stuck. Needed the reframe that a declaration claims memory that already exists rather than creating fresh memory, that every byte of RAM always holds some value, and that returning from a function erases nothing. Confirmed by running the ghost experiment and seeing C-runtime leftovers in a field he never wrote to

## nan-and-float-comparison
- status: practicing
- depends-on: floating-point-numbers
- introduced: 2026-09-20
- last-reviewed: 2026-09-22
- evidence: met NaN as a real value in his own program's output (POS -nan). Answered correctly that a NaN altitude makes a clamp's if fail, and took the point that the failure is silent rather than loud. 2026-09-20: not yet applied - the transition rule he wrote compares two doubles with ==. 2026-09-22 met the == trap himself: predicted his own assert(y_m == 0) would pass ("a pass. we did not alter y"), watched it abort, then printed the value and found 6.1e-16 m of drift that never physically happened. Asked how to compare so noise passes and real errors fail, proposed the idea unprompted - "use an approximation or like a range" - and wrote both asserts as fabs(actual - expected) < TOLERANCE_M. Was shown why x survived == on a geometric accident: sin is flat at its peak so the angle error rounds away, cos sits on a slope of -1 so the same error passes straight through. Restated the cause unprompted at the end of the lesson, in his own words: decimals have "no absolute binary representation so its more of an approximation rather than a steadfast arithmetic", and the tolerance is "the cap on how far the value can drift" - accurate, and the framing his file comment now carries. Said same-day as the teaching, so it is performance rather than retention; re-ask cold

## code-smell
- status: introduced
- depends-on: none
- introduced: 2026-09-20
- last-reviewed: 2026-09-20
- evidence: asked what "a small smell" meant after being told the printf inside tick() was one. Given the definition (not a bug; a hint the design is off) and the concrete cost here: tick() now does two jobs, so section 4's test program cannot call it without text spraying out, and section 5's JSON change has to touch simulation code that did not change

## object-files-vs-executables
- status: practicing
- depends-on: compilation-stages
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: asked twice, in different words, whether an object file is the same as an executable - first as "we get an object file called test which is executable... is that an object file tho", later as a direct question. Saw the answer in his own nm output: the .o carried unfilled U entries with no address, and `file` reports it as relocatable rather than executable.

## compile-time-vs-run-time
- status: practicing
- depends-on: compilation-stages, assert
- introduced: 2026-09-21
- last-reviewed: 2026-09-22
- evidence: after commenting out the battery drain and watching the assertion fire, asked "was it the compiler that threw" - the right question, and the one that separates today's compile-time and link-time failures from the first run-time failure of the project. gcc had accepted the broken engine without a single warning under -Werror. 2026-09-22 predicted that breaking the engine would raise "an error by the compiler"; asked at what point d.x_m actually has a value, corrected himself to run time unprompted, and asked the honest follow-up about who reports the failure.

## interface-vs-implementation
- status: practicing
- depends-on: header-files, declaration-vs-definition
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: pushed on this repeatedly until it landed - "why do we have header files if we we also allow files like drone.c to be called from different locations", "why not define enum in drone.c", "why not just extratc tick from drone.c". The resolution he was reaching for: drone.h is the engine's public description, drone.c its code, and he has been relying on exactly that split every time he writes #include <stdio.h> without ever reading printf's source.

## engine-vs-program-separation
- status: practicing
- depends-on: interface-vs-implementation
- introduced: 2026-09-21
- last-reviewed: 2026-10-01
- evidence: proposed the split himself once the multiple-definition error appeared - "the main function in drone.c should be extracted and call on the functinos that are currently in drone.c" - and worked out that main's locals move with main. Sorted the eight constants across the two files correctly, though by an "all drones vs this instance" rule rather than the decisive one (a constant lives where the code using it lives); said honestly "im not sure how to reason it. what belogns in drone.c and main.c" before getting TICK_S right anyway. 2026-09-25 proposed removing every printf from drone.c; asked why nothing in the engine should print, then proposed moving print_state to main.c himself. Detected the mode change in main.c by saving the mode before tick() and comparing after - his proposal, not prompted beyond 'what does main.c have after tick returns'. Placed mode_to_string in main.c ('naming the mode is about what gets said'), weighed drone.c, and settled it on which programs need the names today. 2026-10-01 asked why print_state was moved to main.c — did not retrieve his own 5.1 decision after 6 days. Re-given. Then placed the mission clock in main.c correctly but suggested 'a tick function' to advance it, not seeing that tick() lives in the engine; after the loop was shown, chose an int counter declared before the loop and incremented inside it.

## make-dependency-graph
- status: practicing
- depends-on: make-targets
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: named the gap himself - "i thought make test would fial because we are watching for build/test_drone but we donthave that yet" - the assumption that a prerequisite is a precondition to check rather than a goal to satisfy. His own three lines of make output were the proof: two recipes ran, bottom-up. On the second run he correctly said only ./build/test_drone would execute, but explained it by existence alone, missing both the timestamp comparison and the phony half.

## makefile-variables
- status: introduced
- depends-on: makefile
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: used BUILD := build throughout his own rules after being shown one worked example. Has not yet written one himself.

## build-artifacts-directory
- status: practicing
- depends-on: gitignore, makefile
- introduced: 2026-09-21
- last-reviewed: 2026-09-21
- evidence: argued for it against a recommendation to defer, and won on evidence rather than preference - "what if we do clean on build/drone instead of drone test_Drone?" - pointing at a staleness failure that had already happened in front of him rather than one he imagined.

## variable-scope
- status: practicing
- depends-on: functions-over-main
- introduced: 2026-09-23
- last-reviewed: 2026-09-24
- evidence: 2026-09-23 named the pain himself - "i have to name them different all the time" - after claim 3's draft hit a second battery_before. Shown that distance_to() and bearing_to() both declare dx without a clash, he answered "the scope of the variables?". He proposed test functions; was shown bare { } blocks as the lighter option and that assert prints the enclosing function's name. Then chose the braces himself, wrapped every claim in its own block, and reused plain names (d, w) in claims 7-8. 2026-09-24 asked what a bare block actually is ("are they no name functions?"); was given blocks as compound statements (the same { } that follows if/while) that see outward but not in, versus functions walled off in both directions. Applied it straight away: said claim 3's old d-for-d_dead_drone slip "wouldnt compile since d doesnt exist there". Then asked to switch to functions, and converted claims 1-2 correctly himself (defined above main)

## tests-check-behaviour-not-code
- status: practicing
- depends-on: test-fairness
- introduced: 2026-09-24
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 predicted that changing the climb fork to <= would fail claim 6; it passed, and he explained why himself (step capped to remaining = 0, so alt before == alt after). The principle was then named for him: a code change that leaves behaviour correct SHOULD pass. He needed both the fork and the cap broken to see red, and predicted 102 m exactly. The pivot is guarded twice

## test-first-red-green
- status: practicing
- depends-on: test-fairness, regression-testing
- introduced: 2026-09-24
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 compared writing the new claim before vs after the engine change, and worked out unaided that 'after' means break-checking by hand while 'before' gives the break-check for free. Then did it: wrote claim 9, saw green, found the function had never been called from main(), called it, and got the expected red. The expectation of red is what caught the uncalled test. Wrote claim 10 red-first as well, and when claim 9's abort hid it, proposed commenting out claim 9's call to see claim 10 fail on its own. Also learned that a compile error (redefinition from a copy-pasted name) is not a red. Predictions were skipped twice and had to be asked for

## failsafe
- status: introduced
- depends-on: finite-state-machine
- introduced: 2026-09-24
- last-reviewed: 2026-09-24
- evidence: 2026-09-24 reached for a 'more professional' name than MODE_NO_POWER and was given 'failsafe', plus the idea that a mode names what the drone is doing rather than why. Decided unprompted that a drone dying during takeoff should also go to failsafe. With a prompt, spotted that claims 9 and 10 put the drone on the ground (altitude 0), which is exactly the case set aside, and moved both drones into the air, including a waypoint above the claim-10 drone so it is really still taking off. Open: is a grounded drone with no power in failsafe or just 'off'? This is the same question as the 0.000001 % reserve

## exit-status
- status: practicing
- depends-on: test-program, make-targets
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 was unsure how an unattended build server learns a test failed, and was walked there: main's return value goes to the parent (make), where 0 means success and anything else means failure. Asked whether make's *** line comes from compiling or running; it comes from running. Then concluded unaided that a suite printing FAIL but returning 0 means 'no faults returned even when they technically did', proposed returning the failure count, and when told only 8 bits survive, found that 256 failures would wrap to 0. Wrote main() himself with a failures count and `if (failures) return 1; else return 0;`, then predicted before running that claim 7's verdict was being discarded because main never stored it. Also watched make report its own Error 1 -> exit 2.

## test-helper-function
- status: practicing
- depends-on: functions-over-main, exit-status
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 asked whether assert's abort could be prevented; told NDEBUG deletes the checks, and real frameworks write their own non-aborting check. When the repeated if-print-set block was put to him, specified the helper himself: the condition, a description, and a 1/0 return. Wrote the body of check() unaided and correctly. Dropped check()'s return twice (bare call, then `failed =` overwriting); both were caught by running it, not by reading.

## string-parameters
- status: introduced
- depends-on: pointers, const-correctness
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 told a C string is passed as const char * (a pointer to its first character, read-only here) and printed with %s; used it correctly in check(). Not yet explained back.

## build-server
- status: introduced
- depends-on: exit-status
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: 2026-09-25 asked what a build server is; told it is a machine that builds unattended and listens only to exit codes. Section 8's Render deploy will run make inside Docker, and a nonzero step stops the deploy.

## switch-statement
- status: practicing
- depends-on: enums, conditionals
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: proposed a switch himself for mapping a FlightMode to text ("like if branches but because its specified we use switch"). Syntax was shown; wrote mode_to_string with a return-per-case for all four modes correctly. Missed the fallback return; -Wreturn-type caught it. Worked out that an out-of-range value falls through every case ("it wont do anything and maybe wont return anything"), predicted a default would show UNKNOWN at runtime, and was told why the fallback belongs after the switch (keeps -Wswitch alive). Why-after-switch was supplied, not produced - re-ask cold later.

## string-literals
- status: introduced
- depends-on: pointers, const-correctness
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: told a string literal is fixed text stored in the program, and a function handing one back returns const char *. Wrote the const char * return type correctly in mode_to_string.

## dropped-return-value
- status: practicing
- depends-on: functions-over-main
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: third occurrence (4.5 twice, 5.1 once): called print_mode(drone.mode) with nothing receiving the result. Traced the path when asked where "NAVIGATE" went, and answered "i need to do printf". The misleading name made the bare call look complete.

## speed-vs-velocity
- status: introduced
- depends-on: vectors-and-distance, heading-and-direction
- introduced: 2026-09-25
- last-reviewed: 2026-09-25
- evidence: asked unprompted "why not velocity" and whether a drone climbing straight up has speed 0. Given: speed is a size, velocity is size + direction; heading_deg + speed_mps already are the horizontal velocity; the engine sets speed to 0 outside NAVIGATE, so a climb reads as 0. Decided to rename the field horizontal_speed_mps and leave climb rate out of the MVP.

## json-schema
- status: seed
- depends-on: json, data-contract
- introduced: —
- last-reviewed: —
- evidence: named as the machine-checkable alternative to a Markdown contract (2026-09-25); parked — not worth a tool before the first real mismatch.

## array-to-pointer-conversion
- status: introduced
- depends-on: pointers, arrays-of-structs
- introduced: 2026-10-02
- last-reviewed: 2026-10-02
- evidence: 2026-10-02 told that passing an array passes the address of its first element, so the parameter must be a pointer (or T name[], which means the same). Did not follow the first explanation; the memory-address picture (1000/1024/1048) and the 'address of the first house on a street' framing landed. Asked whether the pointer is still needed in the parameter if C converts automatically — yes; and why &mission[current_wp] needs & — because indexing yields one struct, which does not convert. Not yet applied independently: chose to pass a single waypoint instead, which sidestepped it.

## exact-counting-with-integers
- status: practicing
- depends-on: floating-point-numbers
- introduced: 2026-10-02
- last-reviewed: 2026-10-02
- evidence: 2026-10-02 replaced a planned float accumulator with an int tick_count and mission_time_s = TICK_S * ticks after seeing his own drift experiment fail. Named the int parameter `ticks` after being asked whether `mission_time_s` told the truth about what it held.
