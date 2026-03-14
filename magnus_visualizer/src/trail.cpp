#include "trail.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

Trail::Trail() {
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
    if (positions.empty() && referencePositions.empty()) return;
    
    glDisable(GL_LIGHTING);
    
    // Draw reference path (without Magnus) - gray, low opacity, dashed
    if (!referencePositions.empty()) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2, 0x00FF);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < referencePositions.size(); ++i) {
            float alpha = 0.3f * (1.0f - (float)i / (float)referencePositions.size());
            glColor4f(0.5f, 0.5f, 0.5f, alpha);
            const glm::vec3& p = referencePositions[i];
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        
        // Ground projection for reference
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < referencePositions.size(); ++i) {
            float alpha = 0.2f * (1.0f - (float)i / (float)referencePositions.size());
            glColor4f(0.5f, 0.5f, 0.5f, alpha);
            const glm::vec3& p = referencePositions[i];
            glVertex3f(p.x, 0.01f, p.z);
        }
        glEnd();
    }
    
    // Draw actual path (with Magnus) - yellow, high opacity
    if (!positions.empty()) {
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < positions.size(); ++i) {
            float alpha = 1.0f - (float)i / (float)positions.size();
            glColor4f(1.0f, 1.0f, 0.0f, alpha);
            const glm::vec3& p = positions[i];
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
        
        // Ground projection for actual path
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < positions.size(); ++i) {
            float alpha = 0.7f * (1.0f - (float)i / (float)positions.size());
            glColor4f(1.0f, 0.0f, 1.0f, alpha);
            const glm::vec3& p = positions[i];
            glVertex3f(p.x, 0.01f, p.z);
        }
        glEnd();
    }
    
    glEnable(GL_LIGHTING);
}
