#include <GL/glut.h>
#include <iostream>
#include <cstring>
#include <cmath>

#include "ball.h"
#include "simulation.h"
#include "camera.h"
#include "renderer.h"
#include "vectors.h"
#include "trail.h"
#include "input.h"
#include "ui.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

Ball ball;
Simulation simulation(ball);
Camera camera;
Vectors vectors;
Trail trail;
Input inputHandler(ball, simulation, camera, vectors, trail);
Renderer renderer(ball, simulation, vectors, trail);
UI ui(ball, simulation, inputHandler);

int windowWidth = 1280;
int windowHeight = 720;

bool showControlPanel = true;
bool wasRunning = false;

void display() {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float uiWidth = windowWidth * 0.3f;
    float viewWidth = windowWidth - uiWidth;
    
    glViewport(0, 0, (int)viewWidth, windowHeight);
    camera.aspect = viewWidth / (float)windowHeight;
    camera.apply();
    renderer.draw();
    
    glViewport(0, 0, windowWidth, windowHeight);
    
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    
    ImGui::NewFrame();
    
    float panelX = viewWidth + 5;
    float panelWidth = uiWidth - 15;
    float panelHeight = windowHeight - 20;
    float panelY = 10;
    
    ImGui::SetNextWindowPos(ImVec2(panelX, panelY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);
    
    ImGui::Begin("Controls", &showControlPanel, 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::SetWindowSize(ImVec2(panelWidth, panelHeight));
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    
    ImGui::Text("MAGNUS EFFECT");
    ImGui::Separator();
    
    ImGui::Spacing();
    ImGui::Text("Initial Velocity");
    ImGui::Separator();
    ImGui::SliderFloat("Velocity X", &inputHandler.velocityX, 0.0f, 50.0f, "%.1f m/s");
    ImGui::SliderFloat("Velocity Y", &inputHandler.velocityY, -20.0f, 20.0f, "%.1f m/s");
    ImGui::SliderFloat("Velocity Z", &inputHandler.velocityZ, -30.0f, 30.0f, "%.1f m/s");
    
    float speed = sqrtf(inputHandler.velocityX * inputHandler.velocityX + 
                        inputHandler.velocityY * inputHandler.velocityY + 
                        inputHandler.velocityZ * inputHandler.velocityZ);
    ImGui::Text("Total Speed: %.2f m/s", speed);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Ball Properties");
    ImGui::Separator();
    ImGui::SliderFloat("Mass (kg)", &ball.mass, 0.1f, 2.0f, "%.2f kg");
    ImGui::SliderFloat("Spin Rate", &inputHandler.angularVelocityScale, 0.0f, 100.0f, "%.1f rad/s");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Spin Type");
    ImGui::Separator();
    
    const char* spinTypes[] = { "Top Spin", "Back Spin", "Side Spin", "No Spin" };
    int currentSpin = static_cast<int>(inputHandler.currentSpinType);
    if (ImGui::Combo("##spin", &currentSpin, spinTypes, 4)) {
        inputHandler.currentSpinType = static_cast<SpinType>(currentSpin);
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    ImGui::Text("Simulation Speed");
    ImGui::Separator();
    ImGui::SliderFloat("Speed", &simulation.timeScale, 0.1f, 3.0f, "%.1fx");
    
    ImGui::Spacing();
    ImGui::Separator();
    
    ImGui::Text("Magnus Effect");
    ImGui::Separator();
    ImGui::SliderFloat("Visibility", &simulation.magnusVisibility, 1.0f, 100.0f, "%.1fx");
    
    ImGui::Spacing();
    ImGui::Separator();
    
    ImVec2 buttonSize(ImGui::GetContentRegionAvail().x / 2 - 4, 50);
    
    if (ImGui::Button("LAUNCH", buttonSize)) {
        inputHandler.launch();
    }
    ImGui::SameLine();
    if (ImGui::Button("RESET", buttonSize)) {
        inputHandler.reset();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Visualization");
    ImGui::Separator();
    
    bool vectorsEnabled = vectors.enabled;
    if (ImGui::Checkbox("Show Force Vectors", &vectorsEnabled)) {
        vectors.enabled = vectorsEnabled;
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Current State");
    ImGui::Separator();
    
    if (simulation.isLaunched) {
        ImGui::Text("Speed: %.2f m/s", simulation.getSpeed());
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", ball.position.x, ball.position.y, ball.position.z);
        ImGui::Text("Spin: %.2f rad/s", ball.getSpinMagnitude());
        ImGui::Text("Magnus Force: %.3f N", simulation.getMagnusForceMagnitude());
    } else {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Ready to launch");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Legend");
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "■ With Magnus (strong)");
    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "■ Without Magnus (weak)");
    ImGui::TextColored(ImVec4(1, 0, 1, 1), "■ Ground Path");
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "■ Velocity");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "■ Magnus Force");
    ImGui::TextColored(ImVec4(0.3, 0.3, 1, 1), "■ Gravity");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Separator();
    ImGui::Text("Mouse drag: Orbit");
    ImGui::Text("Mouse wheel: Zoom");
    ImGui::Text("Arrow keys: Move");
    
    ImGui::PopStyleVar();
    
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    
    glutSwapBuffers();
}

void timer(int value) {
    wasRunning = simulation.isLaunched;
    simulation.update();
    
    if (simulation.isLaunched) {
        trail.addPosition(ball.position);
        trail.addReferencePosition(simulation.referenceBall.position);
    } else if (wasRunning && !simulation.isLaunched) {
        trail.clear();
        inputHandler.launch();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    camera.aspect = (float)(width - width * 0.3f) / (float)height;
}

int lastMouseX = 0;
int lastMouseY = 0;
bool mouseDragging = false;

bool isMouseIn3DView(int x, int y) {
    float uiWidth = windowWidth * 0.3f;
    float viewWidth = windowWidth - uiWidth;
    return x < viewWidth;
}

void mouse(int button, int state, int x, int y) {
    bool in3DView = isMouseIn3DView(x, y);
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (in3DView) {
                mouseDragging = true;
                lastMouseX = x;
                lastMouseY = y;
            }
        } else if (state == GLUT_UP) {
            mouseDragging = false;
        }
    }
    
    if (in3DView) {
        if (button == 3 && state == GLUT_DOWN) {
            camera.zoom(-1.0f);
        }
        if (button == 4 && state == GLUT_DOWN) {
            camera.zoom(1.0f);
        }
    } else {
        ImGui_ImplGLUT_MouseFunc(button, state, x, y);
    }
}

void mouseMotion(int x, int y) {
    if (mouseDragging && isMouseIn3DView(x, y)) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        inputHandler.handleMouseDrag(dx, dy);
        lastMouseX = x;
        lastMouseY = y;
    } else if (!isMouseIn3DView(x, y)) {
        ImGui_ImplGLUT_MotionFunc(x, y);
    }
}

void keyboard(unsigned char key, int x, int y) {
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
}

void specialKeys(int key, int x, int y) {
    inputHandler.handleSpecialKey(key, x, y);
    ImGui_ImplGLUT_SpecialFunc(key, x, y);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Magnus Effect Visualizer");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    reshape(windowWidth, windowHeight);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
    (void)io;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();
    
    glutTimerFunc(16, timer, 0);
    
    std::cout << "Magnus Effect Visualizer" << std::endl;
    
    glutMainLoop();
    
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
    
    return 0;
}
