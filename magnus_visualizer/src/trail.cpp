#include "trail.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

Trail::Trail() : enabled(true), magnusEnabled(true), referenceEnabled(true),
                 magnusTrajectoryEnabled(true), magnusGroundEnabled(true),
                 referenceTrajectoryEnabled(true), referenceGroundEnabled(true) {
}

void Trail::addPosition(const glm::vec3& pos) {
    positions.push_front(pos);
    if (positions.size() > MAX_POINTS) {
        positions.pop_back();
    }
}

void Trail::addReferencePosition(const glm::vec3& pos) {
    referencePositions.push_front(pos);
    if (referencePositions.size() > MAX_POINTS) {
        referencePositions.pop_back();
    }
}

void Trail::clear() {
    positions.clear();
    referencePositions.clear();
}

void Trail::draw() {
    if (!enabled) return;
    if (positions.empty() && referencePositions.empty()) return;
    
    glDisable(GL_LIGHTING);
    
    if (referenceEnabled && !referencePositions.empty()) {
        if (referenceTrajectoryEnabled) {
            glBegin(GL_LINE_STRIP);
            for (size_t i = 0; i < referencePositions.size(); ++i) {
                float alpha = 1.0f - (float)i / (float)referencePositions.size();
                glColor4f(0.4f, 0.4f, 0.4f, alpha);
                const glm::vec3& p = referencePositions[i];
                glVertex3f(p.x, p.y, p.z);
            }
            glEnd();
        }
        
        if (referenceGroundEnabled) {
            glBegin(GL_LINE_STRIP);
            for (size_t i = 0; i < referencePositions.size(); ++i) {
                float alpha = 0.4f * (1.0f - (float)i / (float)referencePositions.size());
                glColor4f(0.8f, 0.8f, 0.8f, alpha);
                const glm::vec3& p = referencePositions[i];
                glVertex3f(p.x, 0.02f, p.z);
            }
            glEnd();
        }
    }
    
    if (magnusEnabled && !positions.empty()) {
        if (magnusTrajectoryEnabled) {
            glBegin(GL_LINE_STRIP);
            for (size_t i = 0; i < positions.size(); ++i) {
                float alpha = 1.0f - (float)i / (float)positions.size();
                glColor4f(1.0f, 0.8f, 0.0f, alpha);
                const glm::vec3& p = positions[i];
                glVertex3f(p.x, p.y, p.z);
            }
            glEnd();
        }
        
        if (magnusGroundEnabled) {
            glBegin(GL_LINE_STRIP);
            for (size_t i = 0; i < positions.size(); ++i) {
                float alpha = 0.7f * (1.0f - (float)i / (float)positions.size());
                glColor4f(1.0f, 0.6f, 0.2f, alpha);
                const glm::vec3& p = positions[i];
                glVertex3f(p.x, 0.02f, p.z);
            }
            glEnd();
        }
    }
    
    glEnable(GL_LIGHTING);
}
