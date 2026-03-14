#include "renderer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

Renderer::Renderer(Ball& b, Simulation& sim, Vectors& v, Trail& t) : ball(b), simulation(sim), vectors(v), trail(t) {
}

void Renderer::drawGround() {
    glPushMatrix();
    glColor3f(0.3f, 0.5f, 0.3f);
    glBegin(GL_LINES);
    float gridSize = 20.0f;
    float gridStep = 1.0f;
    for (float i = -gridSize; i <= gridSize; i += gridStep) {
        glVertex3f(i, 0.0f, -gridSize);
        glVertex3f(i, 0.0f, gridSize);
        glVertex3f(-gridSize, 0.0f, i);
        glVertex3f(gridSize, 0.0f, i);
    }
    glEnd();
    glPopMatrix();
}

void Renderer::drawBall() {
    glPushMatrix();
    glTranslatef(ball.position.x, ball.position.y, ball.position.z);
    
    if (ball.getSpinMagnitude() > 0.001f) {
        glm::vec3 axis = ball.rotationAxis;
        glRotatef(ball.rotationAngle, axis.x, axis.y, axis.z);
    }
    
    GLfloat matAmbient[] = {0.8f, 0.1f, 0.1f, 1.0f};
    GLfloat matDiffuse[] = {0.8f, 0.1f, 0.1f, 1.0f};
    GLfloat matSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat matShininess[] = {50.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    
    glutSolidSphere(ball.radius, 32, 32);
    glPopMatrix();
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat lightPosition[] = {10.0f, 20.0f, 10.0f, 0.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    
    drawGround();
    trail.draw();
    drawBall();
    
    if (simulation.isLaunched) {
        vectors.draw(ball.position, ball.velocity, simulation.currentMagnusForce, simulation.currentGravityForce);
    }
}
