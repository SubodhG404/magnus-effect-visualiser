#include "ui.h"
#include <GL/gl.h>
#include <cstdio>
#include <cstring>

UI::UI(Ball& b, Simulation& sim, Input& in) : ball(b), simulation(sim), input(in) {
}

void UI::drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void UI::drawLegend() {
    float y = -0.85f;
    float x = -0.95f;
    float spacing = 0.05f;
    
    glColor3f(0.0f, 1.0f, 0.0f);
    drawText("Green: Velocity", x, y);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    drawText("Red: Magnus Force", x, y + spacing);
    
    glColor3f(0.0f, 0.0f, 1.0f);
    drawText("Blue: Gravity", x, y + spacing * 2);
}

void UI::draw() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    char buffer[256];
    float y = 0.85f;
    float x = -0.95f;
    float spacing = 0.05f;
    
    if (simulation.isLaunched) {
        snprintf(buffer, sizeof(buffer), "Speed: %.2f m/s", simulation.getSpeed());
        drawText(buffer, x, y);
        
        snprintf(buffer, sizeof(buffer), "Spin: %.2f rad/s", ball.getSpinMagnitude());
        drawText(buffer, x, y + spacing);
        
        snprintf(buffer, sizeof(buffer), "Magnus Force: %.3f N", simulation.getMagnusForceMagnitude());
        drawText(buffer, x, y + spacing * 2);
    }
    
    snprintf(buffer, sizeof(buffer), "Mass: %.2f kg", ball.mass);
    drawText(buffer, x, y + spacing * 3);
    
    snprintf(buffer, sizeof(buffer), "Initial Speed: %.2f m/s", input.initialSpeed);
    drawText(buffer, x, y + spacing * 4);
    
    const char* spinName = "Unknown";
    switch (input.currentSpinType) {
        case SpinType::TOPSPIN: spinName = "Top Spin"; break;
        case SpinType::BACKSPIN: spinName = "Back Spin"; break;
        case SpinType::SIDESPIN: spinName = "Side Spin"; break;
        case SpinType::NOSPIN: spinName = "No Spin"; break;
    }
    snprintf(buffer, sizeof(buffer), "Spin Type: %s", spinName);
    drawText(buffer, x, y + spacing * 5);
    
    drawLegend();
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
