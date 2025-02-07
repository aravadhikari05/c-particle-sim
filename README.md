# Particle Simulator

A high-performance 2D particle simulator built in C using OpenGL. Handles particle collisions with quadtrees and uses Separating Axis Theorem (SAT) for collision resolution. Users can interact with the simulation via different states to control particle behavior dynamically.

## Features
- **Real-time 2D particle simulation** with OpenGL for rendering and GLFW for window/input handling
- **Quadtree-based collision detection** for optimized performance
- **Separating Axis Theorem (SAT)** for accurate collision handling
- **Multiple simulation states**:
  - **Spawning State**: Spawns particles on cursor position
  - **Magnet State**: Attracts/Repulses particles based on cursor position
  - **Default State**: Normal particle behavior
- **Verlet Integration** (Velocity Verlet)

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/aravadhikari05/c-particle-sim.git
   cd c-particle-sim
   ```
2. Build the following libraries:
   - GLAD
   - GLFW
   
    All libraries can be built with ```make libs```. Make sure submodules have been cloned.
3. Compile the project:

   `make`
4. Run the program:

   `./bin/game`

## Controls
- **`S`** - Toggle spawning state
- **`M`** - Toggle magnet state
- **`S` or `M` again** - Return to default state

## Future Improvements
- UDP server integration


