#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "ball.h"
#include "simulation.h"
#include "input.h"
#include "trail.h"
#include <GL/glut.h>

class ControlPanel {
public:
    Ball& ball;
    Simulation& simulation;
    Input& input;
    Trail& trail;

    bool showPanel;

    ControlPanel(Ball& b, Simulation& sim, Input& in, Trail& t);

    void draw(float uiWidth, float totalWindowWidth, float windowHeight);
};

#endif
