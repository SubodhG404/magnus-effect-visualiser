#include <GL/glut.h>
#include <iostream>

#include "ball.h"
#include "simulation.h"
#include "camera.h"
#include "renderer.h"
#include "trail.h"
#include "input.h"
#include "controlpanel.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

Ball ball;
Simulation simulation(ball);
Camera camera;
Trail trail;
Input inputHandler(ball, simulation, camera, trail);
Renderer renderer(ball, simulation, trail);
ControlPanel controlPanel(ball, simulation, inputHandler, trail);

int windowWidth = 1280;
int windowHeight = 720;
int mouseX = 0;
int mouseY = 0;

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
    
    controlPanel.draw(uiWidth, windowWidth, windowHeight);
    
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