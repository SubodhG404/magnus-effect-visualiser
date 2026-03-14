# Magnus Effect Visualizer

A 3D interactive simulation of the Magnus effect in C++ using OpenGL. Visualize how spin affects a ball's trajectory through the air.

![Magnus Effect](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e5/Magnus_effect.svg/640px-Magnus_effect.svg.png)

## Overview

The Magnus effect is the observable phenomenon where a spinning object moving through a fluid departs from its straight path. This is commonly seen in ball sports - when a soccer player curves the ball, or a tennis player hits a topspin shot.

This simulator shows:
- **Realistic Magnus force** calculation using physics equations
- **Reference path** (gray) - trajectory without Magnus effect
- **Actual path** (yellow) - trajectory with Magnus effect
- Support for backspin, topspin, and sidespin

## Physics

The Magnus force is calculated using:
```
F = 0.5 * ρ * A * Cl * v²
```

Where:
- ρ = air density (1.225 kg/m³)
- A = cross-sectional area
- Cl = lift coefficient (based on spin parameter)
- v = velocity

The force direction is perpendicular to both velocity and spin axis: **v × ω**

## Controls

### Mouse
- **Left drag**: Orbit camera
- **Scroll**: Zoom in/out

### Keyboard
- **Arrow keys**: Pan camera

### UI Panel
- **Velocity X/Y/Z**: Set initial velocity
- **Spin Rate**: Adjust angular velocity (rad/s)
- **Spin Type**: Select backspin, topspin, sidespin, or no spin
- **Visibility**: Scale Magnus effect (1x = realistic, higher = more visible)
- **LAUNCH**: Start simulation
- **RESET**: Reset ball position

## Building

### Prerequisites
- C++ compiler (g++ or clang)
- CMake
- OpenGL
- GLUT
- GLM (included)
- ImGui (included)

### Build on Linux

```bash
cd magnus_visualizer/build
cmake ..
make
./magnus_visualizer
```

### Build on macOS

```bash
cd magnus_visualizer/build
cmake .. -G "Xcode"
# Open in Xcode and build
```

### Build on Windows

```bash
cd magnus_visualizer/build
cmake .. -G "Visual Studio"
# Open solution and build
```

## Expected Results

### Backspin
- Ball curves upward (lift)
- Goes higher than reference path

### Topspin
- Ball curves downward (drop)
- Falls faster than reference path

### Sidespin
- Ball curves left or right
- Deflects sideways from reference path

## Project Structure

```
magnus_visualizer/
├── src/
│   ├── main.cpp        - Entry point & UI
│   ├── simulation.cpp  - Physics calculations
│   ├── ball.cpp        - Ball state
│   ├── camera.cpp      - Camera controls
│   ├── renderer.cpp    - OpenGL rendering
│   ├── trail.cpp       - Path visualization
│   └── vectors.cpp     - Force vector display
├── include/
│   ├── glm/            - Math library
│   └── imgui/          - UI library
└── CMakeLists.txt      - Build configuration
```

## License

MIT License
