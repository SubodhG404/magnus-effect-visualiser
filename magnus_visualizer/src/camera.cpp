#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera() : fov(45.0f), aspect(16.0f/9.0f), nearPlane(0.1f), farPlane(100.0f),
                   azimuth(45.0f), elevation(30.0f), distance(15.0f), target(0.0f, 0.5f, 0.0f), position(0.0f, 0.5f, 0.0f) {
}

void Camera::apply() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, nearPlane, farPlane);

    float azimuthRad = azimuth * 3.14159f / 180.0f;
    float elevationRad = elevation * 3.14159f / 180.0f;

    float eyeX = target.x + distance * cos(elevationRad) * sin(azimuthRad);
    float eyeY = target.y + distance * sin(elevationRad);
    float eyeZ = target.z + distance * cos(elevationRad) * cos(azimuthRad);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, target.x, target.y, target.z, 0.0f, 1.0f, 0.0f);
}

void Camera::orbit(float deltaAzimuth, float deltaElevation) {
    azimuth += deltaAzimuth;
    elevation += deltaElevation;
    if (elevation > 89.0f) elevation = 89.0f;
    if (elevation < -89.0f) elevation = -89.0f;
}

void Camera::zoom(float delta) {
    distance += delta;
    if (distance < 2.0f) distance = 2.0f;
    if (distance > 50.0f) distance = 50.0f;
}

void Camera::pan(float deltaX, float deltaZ) {
    target.x += deltaX;
    target.z += deltaZ;
    position.x += deltaX;
    position.z += deltaZ;
}

void Camera::setTarget(const glm::vec3& t) {
    target = t;
}
