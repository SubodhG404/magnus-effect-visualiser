#ifndef UI_H
#define UI_H

#include "ball.h"
#include "simulation.h"
#include "input.h"
#include <GL/glut.h>

class UI {
public:
    Ball& ball;
    Simulation& simulation;
    Input& input;

    UI(Ball& b, Simulation& sim, Input& in);

    void draw();
    void drawText(const char* text, float x, float y);
    void drawLegend();
};

#endif
