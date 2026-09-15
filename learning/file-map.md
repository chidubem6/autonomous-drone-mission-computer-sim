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
- Makefile — known (2026-09-15) — the build recipe: how to turn src/ into the drone executable, plus run and clean → [[makefile]], [[make-targets]]
- .git/ — generated — git's own storage: every commit and every past version of every file lives here, never edited by hand → [[git-repository]]
- src/ — known (2026-09-14) — the C engine's source: the drone's onboard computer → [[project-structure]]
- src/drone.c — known (2026-09-14) — the engine itself; holds one DroneState and renders it as one telemetry line → [[pointers]], [[printf-format]]
- src/drone.h — known (2026-09-14) — the shape of the drone's state, shared with every file that needs it → [[struct]], [[include-guards]]
- drone — generated — the compiled engine, rebuilt from src/ by gcc and never committed → [[compiling-c]]

<!-- Nothing else exists yet. Section 1 adds the git repository, the Makefile,
     and the first engine source files; every one of them gets a line here the
     day it appears. -->
