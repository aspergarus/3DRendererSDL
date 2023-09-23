#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            glm::vec3 v;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            glm::vec3 n;
            for (int i = 0; i < 3; i++) {
                iss >> n[i];
            }
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            glm::vec2 uv;
            for (int i = 0; i < 2; i++) {
                iss >> uv[i];
            }
            uv_.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<glm::ivec3> f;
            glm::ivec3 tmp;
            iss >> trash;
            while (iss >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) {
                for (int i = 0; i < 3; i++) {
                    tmp[i]--; // in wavefront obj all indices start at 1, not zero
                }
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int faceId) {
    std::vector<int> face;
    for (int vertexId = 0; vertexId < (int)faces_[faceId].size(); vertexId++) {
        face.push_back(faces_[faceId][vertexId][0]);
    }
    return face;
}

glm::vec3 Model::vert(int i) {
    return verts_[i];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(glm::ivec2 uv) {
    return diffusemap_.get(uv.x, uv.y);
}

glm::ivec2 Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    return glm::ivec2(uv_[idx].x * diffusemap_.get_width(), uv_[idx].y * diffusemap_.get_height());
}

glm::vec3 Model::norm(int faceId, int verticeId) {
    int normalId = faces_[faceId][verticeId][2];
    return glm::normalize(norms_[normalId]);
}
