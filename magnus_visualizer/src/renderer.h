#ifndef RENDERER_H
#define RENDERER_H

#include "ball.h"
#include "simulation.h"
#include "camera.h"
#include "trail.h"
#include <GL/glut.h>

class Renderer {
public:
    Ball& ball;
    Simulation& simulation;
    Trail& trail;

    Renderer(Ball& b, Simulation& sim, Trail& t);

    void drawGround();

    void drawBall();

    void draw();
};

#endif