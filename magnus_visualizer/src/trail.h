#ifndef TRAIL_H
#define TRAIL_H

#include <glm/glm.hpp>
#include <deque>

class Trail {
public:
    static const size_t MAX_POINTS = 2000;
    std::deque<glm::vec3> positions;
    std::deque<glm::vec3> referencePositions;
    bool enabled;
    bool magnusEnabled;
    bool referenceEnabled;
    bool magnusTrajectoryEnabled;
    bool magnusGroundEnabled;
    bool referenceTrajectoryEnabled;
    bool referenceGroundEnabled;

    Trail();

    void addPosition(const glm::vec3& pos);
    void addReferencePosition(const glm::vec3& pos);
    void clear();
    void draw();
};

#endif
