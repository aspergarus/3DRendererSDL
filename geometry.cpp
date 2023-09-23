#include "geometry.h"

glm::vec3 m2v(glm::mat4 m) {
    return glm::vec3(
        m[0][0] / m[3][0],
        m[1][0] / m[3][0],
        m[2][0] / m[3][0]
    );
}

glm::mat4 v2m(glm::vec3 v) {
    // Matrix m(4, 1);
    glm::mat4 m;
	m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

glm::mat4 matIdentity4() {
	glm::mat4 m;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = (float) (i == j);
		}
	}
	return m;
}

glm::vec4 vec3ToVec4(glm::vec3 v) {
    glm::vec4 newVec(v, 1.0);
    return newVec;
}

glm::vec3 vec4ToVec3(glm::vec4 v) {
    return glm::vec3(
        v[0] / v[3],
        v[1] / v[3],
        v[2] / v[3]
    );
}

void printMatrice(glm::mat4 m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
			std::cout << m[i][j] << (j < 3 ? "\t" : "\n");
		}
	}
}

void printVec(glm::ivec2 v) {
    std::cout << "i(";
    for (int i = 0; i < 2; i++) {
        std::cout << v[i] << (i == 1 ? "" : ", "); 
    }
    std::cout << ")\n";
}

void printVec(glm::vec2 v) {
    std::cout << "f(";
    for (int i = 0; i < 2; i++) {
        std::cout << v[i] << (i < 1 ? ", " : ""); 
    }
    std::cout << ")";
    std::cout << std::endl;
}

void printVec(glm::vec3 v) {
    std::cout << "f(";
    for (int i = 0; i < 3; i++) {
        std::cout << v[i] << (i < 2 ? ", " : "");
    }
    std::cout << ")";
    std::cout << std::endl;
}

void printVec(glm::ivec3 v) {
    std::cout << "i(";
    for (int i = 0; i < 3; i++) {
        std::cout << v[i] << (i < 2 ? ", " : ""); 
    }
    std::cout << ")";
    std::cout << std::endl;
}

void printVec(glm::vec4 v) {
    std::cout << "f(";
    for (int i = 0; i < 4; i++) {
        std::cout << v[i] << (i < 3 ? ", " : ""); 
    }
    std::cout << ")" << std::endl;
}

void printVec(glm::ivec4 v) {
    std::cout << "i(";
    for (int i = 0; i < 4; i++) {
        std::cout << v[i] << (i < 3 ? ", " : ""); 
    }
    std::cout << ")" << std::endl;
}

void printVecs(glm::vec3 v[3]) {
    for (int i = 0; i < 3; i++) {
        printVec(v[i]);
    }
    std::cout << std::endl;
}

void printVecs(glm::ivec3 v[3]) {
    for (int i = 0; i < 3; i++) {
        printVec(v[i]);
    }
    std::cout << std::endl;
}

void printUV(glm::ivec2 uv[3]) {
    for (int i = 0; i < 3; i++) {
        std::cout << "i(" << uv[i].x << "," << uv[i].y << ")\n";
    }
    std::cout << std::endl;
}
