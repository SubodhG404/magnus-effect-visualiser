#ifndef INPUT_H
#define INPUT_H

#include "ball.h"
#include "simulation.h"
#include "camera.h"
#include "vectors.h"
#include "trail.h"
#include <GL/glut.h>

class Input {
public:
    Ball& ball;
    Simulation& simulation;
    Camera& camera;
    Vectors& vectors;
    Trail& trail;

    float velocityX;
    float velocityY;
    float velocityZ;
    float spinRPM;
    float spinAxisX;
    float spinAxisY;
    float spinAxisZ;
    float startPosX;
    float startPosY;
    float startPosZ;

    Input(Ball& b, Simulation& sim, Camera& cam, Vectors& v, Trail& t);

    void handleKeyPress(unsigned char key, int x, int y);
    void handleSpecialKey(int key, int x, int y);
    void handleMouseDrag(int dx, int dy);
    void handleMouseWheel(int wheel, int direction, int x, int y);
    void launch();
    void reset();
    glm::vec3 getCurrentSpin() const;
};

#endif
