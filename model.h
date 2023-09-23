#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <glm/glm.hpp>
#include "tgaimage.h"

enum MODEL_TYPE {
    MODEL_CUBE,
    MODEL_PLANES
};

class Model {
private:
	std::vector<glm::vec3> verts_;
	std::vector<std::vector<glm::ivec3> > faces_; // vertex/uv/normal
	std::vector<glm::vec3> norms_;
	std::vector<glm::vec2> uv_;
	TGAImage diffusemap_;
	void load_texture(std::string filename, const char *suffix, TGAImage &img);

public:
	Model(MODEL_TYPE type);
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	glm::vec3 vert(int i);
	glm::vec3 norm(int faceId, int verticeId);
	std::vector<int> face(int idx);
	glm::ivec2 uv(int iface, int nvert);
	TGAColor diffuse(glm::ivec2 uv);
};

#endif //__MODEL_H__
