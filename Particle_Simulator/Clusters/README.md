# Particle Cluster Simulator

A high-performance, real-time simulator of interacting particles on a 2D toroidal grid, written in C with Allegro 5 visualization and OpenMP parallelization.
This project is an implementation of **Jeffrey Ventrella’s Artificial Life Clusters**, a classic artificial life experiment demonstrating how simple rules of attraction and repulsion between particle types can lead to complex emergent patterns and self-organizing behaviors.

---

## Description

- Simulates `NUM_TYPES × BALLS_PER_TYPE` particles travelling and interacting on a 700×700 window.
- Interactions are type-dependent: each pair of types has unique attraction/repulsion distance and strength.
- Adaptively handles collisions using soft repulsion and wraps particles across window boundaries for a continuous world.
- Spatially optimized via grid-based partitioning to limit force calculations to neighboring cells.
- Parallelized with OpenMP: each cell’s interactions are computed concurrently.
- Real-time rendering via Allegro, with colorful visualization of each particle type.

---

## Features

- **8 distinct types** (Blue, Yellow, Green, Purple, Red, Cyan, Orange, Pink)  
- **Gaussian-based forces** centered at "ideal" distances  
- **Overlap avoidance & friction** (`MIN_COL_DIST`, `FRICTION`, `MAX_SPEED`)  
- **Grid partitioning**: 25×25 pixel cells speeding up interactions  
- **Dynamic threading**: cells spawn parallel tasks when particle density exceeds threshold  
- **Controls**:  
  - `R`: Randomize inter-type forces/distances  
  - `I`: Reinitialize positions/velocities  
  - Window close or ESC: Exit cleanly

---

## Simulation Loop
### Initialization

#### init_balls()
- positions and moves particles randomly.
- Forces/distances between types optionally randomized at startup or via R.

### Per Frame

#### apply_forces_parallel()
rebuilds grid, applies neighbor forces per cell in parallel.

#### update_positions() 
moves particles, applies wrapping.

#### draw_balls()
renders particles each frame.

---

## Interaction Physics
- Gaussian-based attractive/repulsive forces
- Short-range repulsion on overlap
- Friction slows particles (FRICTION ≤ 1)
- Max speed enforced (MAX_SPEED)

---

## Configuration Parameters
Adjustable via macros in clusters.c:

#### GRID_SIZE
grid cell width/height (must exceed MAX_COL_DIST)

#### BALLS_PER_TYPE & NUM_TYPES
population settings

#### FRICTION
velocity decay factor (1 = no friction)

#### MAX_SPEED
velocity cap

---

## Visuals
Each particle type is rendered in a unique color. For custom color schemes, modify draw_balls() switch-case section.

---

## Troubleshooting
If Allegro initialization fails, check dev libraries and use must_init() for diagnostics.
If the performance is low, 
- Reduce **BALLS_PER_TYPE** or **NUM_TYPES**
- Increase **WINDOW_WIDTH** or **WINDOW_HEIGHT**
- Reduce **FRAME_RATE**
- Reduce **MAX_COL_DIST**

---

## To Do & Enhancements
- Add command-line options for parameter configs
- Implement GUI controls for runtime tuning
- Record video or snapshots for pattern study
- Export simulation data (CSV or JSON)
- Add logging for performance metrics or cluster statistics
