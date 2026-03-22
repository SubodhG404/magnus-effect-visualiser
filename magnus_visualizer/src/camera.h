#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;

    Camera();

    void apply();
    void rotate(float yaw, float pitch);
    void move(float amount);
    void strafe(float amount);
    void moveUp(float amount);
    void handleMouseMove(int dx, int dy);
    void toggleMouseLook();
    bool mouseLookEnabled;
};

#endif
