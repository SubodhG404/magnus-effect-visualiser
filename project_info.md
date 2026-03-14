Tech Stack
Language: C++17
Windowing & Input: GLUT / FreeGLUT
Graphics API: OpenGL (Legacy/Fixed-Function pipeline — sufficient and appropriate for this project's scope)
Math: GLM (OpenGL Mathematics) — for vectors, cross products, matrix transforms
UI Overlay (optional): GLUT's built-in bitmap font (glutBitmapCharacter) for on-screen text labels — no extra library needed
That's it. No extra dependencies. GLUT + OpenGL + GLM covers everything.



magnus_visualizer/
│
├── src/
│   ├── main.cpp              ← Entry point, GLUT init, window setup
│   ├── simulation.cpp/.h     ← Physics engine (Euler integration, Magnus force)
│   ├── ball.cpp/.h           ← Ball state: position, velocity, spin, mass, radius
│   ├── renderer.cpp/.h       ← All OpenGL draw calls (ball, ground, arrows, trail)
│   ├── vectors.cpp/.h        ← Arrow drawing, force vector visualization
│   ├── trail.cpp/.h          ← Trajectory trail buffer (stores past positions)
│   ├── camera.cpp/.h         ← 3D camera, view/projection matrix setup
│   ├── input.cpp/.h          ← Keyboard & mouse input handler
│   └── ui.cpp/.h             ← On-screen HUD labels (speed, spin, forces)
│
├── include/
│   └── glm/                  ← GLM header-only math library
│
├── CMakeLists.txt            ← Build configuration
└── README.md



Module-by-Module Plan
1. main.cpp — Entry Point

Initialize GLUT window (e.g. 1280×720, titled "Magnus Effect Visualizer")
Register GLUT callbacks: display, reshape, keyboard, specialKey, timer
Set OpenGL state: depth test enabled, smooth shading, background color
Create global instances of Ball, Simulation, Renderer, Camera
Start GLUT main loop

2. ball.h/.cpp — Ball State
Holds all state for the ball object:

glm::vec3 position — current XYZ position
glm::vec3 velocity — current linear velocity
glm::vec3 angularVelocity — spin vector (ω), direction encodes spin type
float mass — user-adjustable
float radius — for rendering and cross-section area A
float rotationAngle — visual rotation for texture/stripe spin effect
reset() method to restore default launch state

3. simulation.cpp/.h — Physics Engine
This is the core. Implements the Euler integration loop each frame.
Each frame does:

Compute Magnus force: F_magnus = S * (ω × v)
Compute gravity: F_gravity = (0, -mass * 9.81, 0)
Sum total force: F_total = F_gravity + F_magnus
Compute acceleration: a = F_total / mass
Update velocity: v = v + a * dt
Update position: p = p + v * dt
Ground collision check: if p.y <= radius, reverse Y velocity, apply friction damping
Store current position into trail buffer

Constants:

Air density ρ = 1.225 kg/m³
Magnus scaling constant S (tunable)
dt = 1/60 seconds (tied to timer callback at 60fps)


4. renderer.cpp/.h — All Drawing
Handles every visual element:

Ground plane — flat quad grid drawn with GL_LINES to give a court/field look; uses glTranslatef + glColor3f
Ball — rendered with glutSolidSphere(radius, slices, stacks); apply glRotatef using the ball's rotationAngle and angularVelocity axis so the ball visually spins
Force arrows — delegated to vectors.cpp
Trajectory trail — delegated to trail.cpp
Reference projectile — a dashed line showing the no-spin path for comparison
Applies glPushMatrix / glPopMatrix for every object to isolate transforms


5. vectors.cpp/.h — Force Vector Arrows
Draws 3 arrows attached to the ball center:
ArrowColorRepresentsGreen(0,1,0)Velocity directionRed(1,0,0)Magnus force directionBlue(0,0,1)Gravity (straight down)


Arrow drawing method:

Use GL_LINES for the shaft
Use glRotatef + glutSolidCone for the arrowhead
Scale arrow length proportional to force magnitude so bigger forces = longer arrows
This directly applies 2D/3D transformation concepts (translate to ball position, rotate to match vector direction)


6. trail.cpp/.h — Trajectory Trail

Stores a std::deque<glm::vec3> of past positions (capped at e.g. 500 points)
Draw with GL_LINE_STRIP — fades older points by reducing alpha using glColor4f
Also stores a separate reference trail (no-spin path) for side-by-side comparison
Cleared on reset


7. camera.cpp/.h — 3D View

Perspective projection using gluPerspective(fov, aspect, near, far) — this is the 3D viewing pipeline
Camera orbits the scene: mouse drag updates azimuth and elevation angles
gluLookAt(eye, center, up) sets the view transform each frame
Default view: slightly elevated, angled to see both horizontal travel and vertical arc

8. input.cpp/.h — User Controls
Keyboard controls for parameter adjustment:
KeyActionSPACELaunch / Reset ballW/SIncrease / Decrease initial speedQ/EIncrease / Decrease angular velocity (spin rate)1Top spin (ω = (1,0,0))2Back spin (ω = (-1,0,0))3Side spin (ω = (0,1,0))4No spin (ω = (0,0,0))M/NIncrease / Decrease massVToggle force vector arrowsTToggle trajectory trailRReset simulationMouse dragRotate camera

9. ui.cpp/.h — HUD Overlay
Switches to 2D orthographic projection to render text, then switches back to 3D:

Current speed, spin rate, mass, spin type displayed top-left
Current Magnus force magnitude displayed
Arrow legend (Green = Velocity, Red = Magnus, Blue = Gravity)
Uses glutBitmapCharacter — no extra font library needed

Graphics Concepts Applied
Concept from SyllabusWhere Used3D Viewing PipelinegluPerspective + gluLookAt in camera.cppTranslation & Rotation (3D Transforms)glTranslatef / glRotatef for ball and arrow placementComposite TransformationsPush/pop matrix stack for each object in renderer.cppVector RepresentationGLM cross product for Magnus force, arrows showing force vectorsAnimation LoopGLUT timer callback at 60fps driving the Euler integrationRasterizationOpenGL rasterizes all spheres, lines, conesRendering PipelineFull fixed-function pipeline: vertex → rasterize → fragmentParallel vs Perspective ProjectionPerspective used for 3D scene; orthographic used for HUD overlayIllumination / ShadingglEnable(GL_LIGHTING) with diffuse + ambient on the ball (Gouraud shading via OpenGL fixed function)Key-frame / Direct MotionDirect-motion specification — position explicitly computed each frame from physicsInteractive VisualizationReal-time parameter tuning while simulation runs




cmake_minimum_required VERSION 3.16
project: MagnusVisualizer
C++17
find_package: OpenGL, GLUT
include_dirs: include/ (for GLM)
sources: all .cpp files in src/
link: OpenGL + GLUT


main()
 └─ GLUT init + window
     └─ timer callback (every 16ms = 60fps)
          ├─ simulation.update()       ← Euler step, physics
          ├─ ball.updateRotation()     ← visual spin angle
          └─ glutPostRedisplay()
               └─ display callback
                    ├─ camera.apply()      ← gluLookAt + gluPerspective
                    ├─ renderer.drawGround()
                    ├─ renderer.drawBall()     ← glutSolidSphere + glRotate
                    ├─ vectors.drawArrows()    ← 3 colored force arrows
                    ├─ trail.draw()            ← GL_LINE_STRIP path
                    └─ ui.drawHUD()            ← 2D ortho overlay



This plan is fully self-contained using only C++ + OpenGL + GLUT + GLM, covers every feature in the PDF, and maps clearly to the computer graphics syllabus concepts you listed.