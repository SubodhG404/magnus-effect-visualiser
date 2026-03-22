#include "renderer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <cmath>

Renderer::Renderer(Ball& b, Simulation& sim, Vectors& v, Trail& t) : ball(b), simulation(sim), vectors(v), trail(t) {
}

void Renderer::drawGround() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    
    glColor3f(0.15f, 0.35f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();
    
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int i = -50; i <= 50; i += 5) {
        glVertex3f(i, 0.01f, -50.0f);
        glVertex3f(i, 0.01f, 50.0f);
        glVertex3f(-50.0f, 0.01f, i);
        glVertex3f(50.0f, 0.01f, i);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Renderer::drawBall() {
    glPushMatrix();
    glTranslatef(ball.position.x, ball.position.y, ball.position.z);
    
    if (ball.getSpinMagnitude() > 0.001f) {
        glm::vec3 axis = ball.rotationAxis;
        glRotatef(ball.rotationAngle, axis.x, axis.y, axis.z);
    }
    
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat shininess[] = {50.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, white);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
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
