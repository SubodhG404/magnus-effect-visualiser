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
int mouseX = 0;
int mouseY = 0;
int mouseState = 0;
int mouseButton = 0;

bool showControlPanel = true;
bool keyStates[256] = {false};

void display() {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float uiWidth = windowWidth * 0.28f;
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
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    
    ImGui::SetWindowSize(ImVec2(panelWidth, panelHeight));
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
    
    ImGui::SetWindowFontScale(1.2f);
    
    ImGui::Text("MAGNUS EFFECT");
    ImGui::Separator();
    
    float w = ImGui::GetContentRegionAvail().x;
    
    ImGui::Text("Velocity (m/s)");
    ImGui::InputFloat("VX", &inputHandler.velocityX, 0, 0, "%.1f");
    ImGui::InputFloat("VY", &inputHandler.velocityY, 0, 0, "%.1f");
    ImGui::InputFloat("VZ", &inputHandler.velocityZ, 0, 0, "%.1f");
    
    ImGui::Separator();
    ImGui::Text("Spin");
    ImGui::InputFloat("RPM", &inputHandler.spinRPM, 0, 0, "%.0f");
    ImGui::InputFloat("Axis X", &inputHandler.spinAxisX, 0, 0, "%.2f");
    ImGui::InputFloat("Axis Y", &inputHandler.spinAxisY, 0, 0, "%.2f");
    ImGui::InputFloat("Axis Z", &inputHandler.spinAxisZ, 0, 0, "%.2f");
    
    ImGui::Separator();
    ImGui::Text("Physics");
    ImGui::InputFloat("Mass (kg)", &ball.mass, 0, 0, "%.2f");
    ImGui::InputFloat("Radius (m)", &ball.radius, 0, 0, "%.3f");
    
    ImGui::Separator();
    ImGui::Text("Start Position");
    ImGui::InputFloat("X", &inputHandler.startPosX, 0, 0, "%.1f");
    ImGui::InputFloat("Y", &inputHandler.startPosY, 0, 0, "%.1f");
    ImGui::InputFloat("Z", &inputHandler.startPosZ, 0, 0, "%.1f");
    
    ImGui::Separator();
    ImGui::Text("Simulation");
    ImGui::InputFloat("Speed Scale", &simulation.timeScale, 0, 0, "%.1f");
    
    ImGui::Separator();
    bool trailOn = trail.enabled;
    if (ImGui::Checkbox("Show Trails", &trailOn)) trail.enabled = trailOn;
    ImGui::Indent();
    bool magnusTrail = trail.magnusEnabled;
    if (ImGui::Checkbox("With Magnus", &magnusTrail)) trail.magnusEnabled = magnusTrail;
    ImGui::Indent();
    bool magTraj = trail.magnusTrajectoryEnabled;
    if (ImGui::Checkbox("  Trajectory (Yellow)", &magTraj)) trail.magnusTrajectoryEnabled = magTraj;
    bool magGround = trail.magnusGroundEnabled;
    if (ImGui::Checkbox("  Ground (Orange)", &magGround)) trail.magnusGroundEnabled = magGround;
    ImGui::Unindent();
    bool refTrail = trail.referenceEnabled;
    if (ImGui::Checkbox("Without Magnus", &refTrail)) trail.referenceEnabled = refTrail;
    ImGui::Indent();
    bool refTraj = trail.referenceTrajectoryEnabled;
    if (ImGui::Checkbox("  Trajectory (Gray)", &refTraj)) trail.referenceTrajectoryEnabled = refTraj;
    bool refGround = trail.referenceGroundEnabled;
    if (ImGui::Checkbox("  Ground (Light)", &refGround)) trail.referenceGroundEnabled = refGround;
    ImGui::Unindent();
    ImGui::Unindent();
    bool vecOn = vectors.enabled;
    if (ImGui::Checkbox("Show Vectors", &vecOn)) vectors.enabled = vecOn;
    
    ImGui::Separator();
    
    ImVec2 btnSize(w / 2 - 4, 40);
    
    if (ImGui::Button("LAUNCH", btnSize)) {
        inputHandler.launch();
    }
    ImGui::SameLine();
    if (ImGui::Button("RESET", btnSize)) {
        inputHandler.reset();
    }
    
    ImGui::Separator();
    ImGui::Text("State");
    if (simulation.isLaunched) {
        ImGui::Text("Speed: %.1f m/s", simulation.getSpeed());
        ImGui::Text("Pos: (%.1f, %.1f, %.1f)", ball.position.x, ball.position.y, ball.position.z);
        ImGui::Text("Magnus: %.3f N", simulation.getMagnusForceMagnitude());
    } else {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Ready");
    }
    
    ImGui::Separator();
    ImGui::Text("Legend");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Yellow: With Magnus");
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Gray: Without Magnus");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Red: Magnus Force");
    
    ImGui::PopStyleVar(2);
    
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    
    glutSwapBuffers();
}

bool isMouseIn3DView() {
    float uiWidth = windowWidth * 0.28f;
    float viewWidth = windowWidth - uiWidth;
    return mouseX < viewWidth;
}

void timer(int value) {
    if (simulation.isLaunched) {
        simulation.update();
        
        if (simulation.isLaunched) {
            trail.addPosition(ball.position);
            trail.addReferencePosition(simulation.referenceBall.position);
        }
    }
    
    if (!ImGui::GetIO().WantCaptureKeyboard && isMouseIn3DView()) {
        float moveSpeed = 0.25f;
        if (keyStates['w'] || keyStates['W']) camera.move(moveSpeed);
        if (keyStates['s'] || keyStates['S']) camera.move(-moveSpeed);
        if (keyStates['a'] || keyStates['A']) camera.strafe(-moveSpeed);
        if (keyStates['d'] || keyStates['D']) camera.strafe(moveSpeed);
        if (keyStates['q'] || keyStates['Q']) camera.moveUp(-moveSpeed);
        if (keyStates['e'] || keyStates['E']) camera.moveUp(moveSpeed);
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    float uiWidth = width * 0.28f;
    camera.aspect = (float)(width - uiWidth) / (float)height;
}

void keyboard(unsigned char key, int x, int y) {
    keyStates[key] = true;
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
    ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);
}

void specialKeys(int key, int x, int y) {
    ImGui_ImplGLUT_SpecialFunc(key, x, y);
}

void specialUp(int key, int x, int y) {
    ImGui_ImplGLUT_SpecialUpFunc(key, x, y);
}

int lastMouseX = 0;
int lastMouseY = 0;
bool mouseDragging = false;

void mouse(int button, int state, int x, int y) {
    mouseX = x;
    mouseY = y;
    mouseButton = button;
    mouseState = state;
    
    ImGui_ImplGLUT_MouseFunc(button, state, x, y);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isMouseIn3DView()) {
        mouseDragging = true;
        lastMouseX = x;
        lastMouseY = y;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mouseDragging = false;
    } else if (button == GLUT_RIGHT_BUTTON) {
        mouseDragging = false;
    }
    
}

void mouseMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
    
    ImGui_ImplGLUT_MotionFunc(x, y);
    
    if (mouseDragging && isMouseIn3DView()) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        inputHandler.handleMouseDrag(dx, dy);
        lastMouseX = x;
        lastMouseY = y;
    }
}

void passiveMouseMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
    ImGui_ImplGLUT_MotionFunc(x, y);
    lastMouseX = x;
    lastMouseY = y;
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
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(passiveMouseMotion);
    
    reshape(windowWidth, windowHeight);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;
    
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