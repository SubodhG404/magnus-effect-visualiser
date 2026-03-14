#include "vectors.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

Vectors::Vectors() : enabled(true) {
}

void Vectors::drawArrow(const glm::vec3& start, const glm::vec3& direction, const glm::vec3& color, float magnitude) {
    if (magnitude < 0.001f) return;
    
    glm::vec3 dirVec = glm::normalize(direction);
    float length = magnitude * 0.5f;
    if (length > 3.0f) length = 3.0f;
    if (length < 0.2f) length = 0.2f;
    
    glm::vec3 end = start + dirVec * length;
    
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(color.r, color.g, color.b);
    
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
    
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (std::abs(glm::dot(dirVec, up)) > 0.99f) {
        up = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    glm::vec3 right = glm::normalize(glm::cross(up, dirVec));
    up = glm::normalize(glm::cross(dirVec, right));
    
    float arrowHeadLength = 0.15f;
    float arrowHeadWidth = 0.08f;
    
    glm::vec3 headBase = end - dirVec * arrowHeadLength;
    glm::vec3 head1 = headBase + right * arrowHeadWidth;
    glm::vec3 head2 = headBase - right * arrowHeadWidth;
    glm::vec3 head3 = headBase + up * arrowHeadWidth;
    glm::vec3 head4 = headBase - up * arrowHeadWidth;
    
    glBegin(GL_TRIANGLES);
    glVertex3f(end.x, end.y, end.z);
    glVertex3f(head1.x, head1.y, head1.z);
    glVertex3f(head2.x, head2.y, head2.z);
    
    glVertex3f(end.x, end.y, end.z);
    glVertex3f(head3.x, head3.y, head3.z);
    glVertex3f(head4.x, head4.y, head4.z);
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Vectors::draw(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& magnusForce, const glm::vec3& gravityForce) {
    if (!enabled) return;
    
    drawArrow(position, velocity, glm::vec3(0.0f, 1.0f, 0.0f), glm::length(velocity));
    drawArrow(position, magnusForce, glm::vec3(1.0f, 0.0f, 0.0f), glm::length(magnusForce));
    drawArrow(position, gravityForce, glm::vec3(0.0f, 0.0f, 1.0f), glm::length(gravityForce) * 0.1f);
}

void Vectors::toggle() {
    enabled = !enabled;
}
