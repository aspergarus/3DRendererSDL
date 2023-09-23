#include <glm/glm.hpp>

glm::mat4 lookat(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
glm::mat4 viewport(int x, int y, int w, int h, int depth);
glm::mat4 projection(float k);