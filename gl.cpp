#include <glm/glm.hpp>
#include "gl.h"
#include "geometry.h"

glm::mat4 lookat(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    glm::vec3 z = glm::normalize(eye - center);
    glm::vec3 x = glm::normalize(glm::cross(up, z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));
    
    glm::mat4 Minv = matIdentity4();
    glm::mat4 Tr   = matIdentity4();
    
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }
    
    return Minv * Tr;
}

glm::mat4 viewport(int x, int y, int w, int h, int depth) {
    glm::mat4 m = matIdentity4();

	m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

glm::mat4 projection(float k) {
    glm::mat4 projection = matIdentity4();
	projection[3][2] = k;
    return projection;
}
