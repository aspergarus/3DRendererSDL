#include <glm/glm.hpp>
#include <iostream>

#ifndef GEOMETRY_H
#define GEOMETRY_H

glm::mat4 matIdentity4();

glm::vec4 vec3ToVec4(glm::vec3 v);
glm::vec3 vec4ToVec3(glm::vec4 v);

void printMatrice(glm::mat4 m);

void printVec(glm::ivec2 v);

void printVec(glm::vec2 v);

void printVec(glm::vec3 v);

void printVec(glm::ivec3 v);

void printVec(glm::vec4 v);

void printVec(glm::ivec4 v);

void printVecs(glm::vec3 v[3]);

void printVecs(glm::ivec3 v[3]);

void printUV(glm::ivec2 uv[3]);

#endif