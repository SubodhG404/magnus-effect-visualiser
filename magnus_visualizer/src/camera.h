#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;
    
    float azimuth;
    float elevation;
    float distance;
    glm::vec3 target;
    glm::vec3 position;

    Camera();

    void apply();
    void orbit(float deltaAzimuth, float deltaElevation);
    void zoom(float delta);
    void pan(float deltaX, float deltaZ);
    void setTarget(const glm::vec3& t);
};

#endif
