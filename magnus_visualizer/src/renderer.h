#ifndef RENDERER_H
#define RENDERER_H

#include "ball.h"
#include "simulation.h"
#include "camera.h"
#include "vectors.h"
#include "trail.h"
#include <GL/glut.h>

class Renderer {
public:
    Ball& ball;
    Simulation& simulation;
    Vectors& vectors;
    Trail& trail;

    Renderer(Ball& b, Simulation& sim, Vectors& v, Trail& t);
    void drawGround();
    void drawBall();
    void draw();
};

#endif
