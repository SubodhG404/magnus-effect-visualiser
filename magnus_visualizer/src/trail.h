#ifndef TRAIL_H
#define TRAIL_H

#include <glm/glm.hpp>
#include <deque>

class Trail {
public:
    static const size_t MAX_POINTS = 500;
    std::deque<glm::vec3> positions;
    std::deque<glm::vec3> referencePositions;

    Trail();

    void addPosition(const glm::vec3& pos);
    void addReferencePosition(const glm::vec3& pos);
    void clear();
    void draw();
};

#endif
