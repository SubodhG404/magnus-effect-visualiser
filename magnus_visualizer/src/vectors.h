#ifndef VECTORS_H
#define VECTORS_H

#include <glm/glm.hpp>

class Vectors {
public:
    bool enabled;

    Vectors();

    void drawArrow(const glm::vec3& start, const glm::vec3& direction, const glm::vec3& color, float magnitude);
    void draw(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& magnusForce, const glm::vec3& gravityForce);
    void toggle();
};

#endif
