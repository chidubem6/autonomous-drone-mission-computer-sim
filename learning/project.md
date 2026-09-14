# Project: Drone Mission Computer

## About me
- Computer science student, on summer break, able to give this ~4 hours a day
  (~28 hours/week) and self-described as obsessive about it.
- Comfortable with C, `gcc`, the terminal, and git (uses git frequently). Most
  recent build: a CLI "number toolkit" (fibonacci, factorial, is-prime) driven by
  user interaction. Currently working through pointers.
- No plan when sitting down to code — opens the editor and picks up where the
  last session left off.
- The one repeated routine: gym sessions, logged in the Notes app. Planned
  exercises with sets/reps on top, actual weight and reps underneath. Same shape
  every session.
- Off the clock: Ghost Recon Breakpoint and milsim games, plus watching coding
  content. Solo — nobody else needs to see the gym log, the games, or the code.
- Why now: wants a software engineering job, and wants one project he is proud of
  AND deployed. Carries a real fear that in the AI era he can't actually code,
  alongside a sense that some evidence says otherwise.
- Starting point on the idea: an AI proposed the drone mission computer sim. He
  liked the live-updating state block (position/altitude/velocity/heading/battery)
  and said plainly that he does not understand the project through and through.
  Object tracking is the part that feels most like magic.

## The idea
A simulation of an autonomous drone's onboard computer, written in C: it holds
the drone's state and advances it on a fixed tick, flying itself between
waypoints while battery drains. The state is published over a connection to a
browser dashboard — the mission-control screen — which shows it live, updating as
the drone flies. The whole thing runs deployed on a real server at a real URL, so
anyone can open the link and watch the mission happen.

## MVP
### In
- **Drone state** — one struct: position, altitude, heading, velocity, battery.
- **The tick loop** — the engine advances state on a fixed clock. The heartbeat.
- **Waypoint navigation** — a list of coordinates; steer toward the current one,
  arrive, advance to the next. Where the vector math lives.
- **Battery drain** — ticks down while flying; zero ends the mission. Gives the
  sim stakes.
- **Mission state machine** — IDLE -> FLYING -> ARRIVED -> COMPLETE.
- **State published over a connection** — the engine makes what it knows readable
  from outside itself.
- **Event log** — timestamped mission events (waypoint reached, battery low,
  mission complete) travelling on the same pipe as state.
- **Warnings derived from existing state** — battery below threshold, altitude
  below floor, off-course drift. Only ever computed from data that already exists.
- **Browser dashboard** — the mock-up's state block, live, redrawing on update.
- **Deployed** — running on a real server with a real URL, not a laptop.

### Parking lot (v2)
- Simulated sensors (GPS, IMU, camera, altimeter) with realistic noise
- Sensor failure injection ("GPS died mid-flight, now what")
- The subsystem health panel (GPS OK / IMU OK / CAMERA OK) — deliberately parked
  until the sensors it reports on actually exist; a green tick next to a sensor
  that was never written is a lie on screen
- Object detection, tracking, and search pattern — the hardest thing on the
  screen, and meaningless until a drone is flying first
- A map view instead of raw numbers
- Command uplink — sending new waypoints from the browser back to the drone
- Writing flight logs and replaying them (this merges with the "flight recorder"
  idea: the sim writes its own black box, then an analyzer reads it)
- Wind, drag, realistic physics
- More than one drone

## The trunk — core components

### 1. Source control — git and GitHub
The professional save-and-undo system. It records every version of the code so
there is always a way back to a working one, and GitHub is the public copy an
employer can actually look at. In from day one, before there is anything to lose.

### 2. The simulation engine (the C program)
The drone's onboard computer. It owns the state — position, speed, battery — and
a tick loop that advances that state a small step at a time, continuously.
Everything else in the project either feeds this or watches it.

### 3. The build step — compiler and Makefile
C doesn't run; it gets compiled into a program first. Once the project is more
than one file, a Makefile is the recipe that rebuilds the right pieces with one
command instead of retyping the whole compile line.

### 4. The server
The program that sits between the engine and the outside world: it listens for
people opening the link and hands them the drone's current state. The C engine
knows how to fly a drone and nothing about the internet. This is the piece that
knows about the internet.

### 5. The contract — how the two sides agree
Both sides must agree on what a state update looks like: which fields, what
they're named, what units they're in. Without a written-down shape, the engine
sends one thing, the dashboard reads another, and nothing on screen moves.

### 6. The dashboard
The browser page — the mission control screen. It receives state updates and
redraws. No physics, no decisions; it only shows what the engine already decided.
It has no idea a drone exists.

### 7. Two places the code runs
Today: the laptop, where he is the only person who can see it. Deployed: a rented
computer that never sleeps and has an address anyone can reach. Same code, two
environments — and the gap between "works on my machine" and "works there" is
where most real engineering pain lives.

### 8. Deployment
Getting it onto that rented computer and keeping it running. The step that
converts a repo into a link that can go on a CV.

### 9. Tests
Small programs that check the code does what it claims — "a drone pointed east and
told to fly for ten seconds should end up here." What makes it safe to change the
engine in week six without silently breaking week two.
