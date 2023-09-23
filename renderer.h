#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "model.h"

class RendererIn3D {
    private:
        SDL_Renderer *m_rend;
        bool m_processed;
        int m_width;
        int m_height;
        int m_depth;
        float* m_zbuffer;
        glm::vec3 m_lightPos;
        Model* m_model;
        glm::mat4 m_modelView, m_projection, m_viewport;

        void renderModel(Model *model);
        void renderFace(Model *model, int i);
        void fillTriangle(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec3 color);
        void fillTriangle(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec2 uv0, glm::ivec2 uv1, glm::ivec2 uv2, float intensity);
        void fillTriangleSmooth(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec2 uv0, glm::ivec2 uv1, glm::ivec2 uv2, float intensity[3]);
        void drawPixelFlippedVert(int x, int y);
        void drawLine(glm::vec2 a, glm::vec2 b);
        void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
        glm::ivec3 vecProdFloat(glm::ivec3 vec, float a);
        glm::ivec2 vec2ProdFloat(glm::ivec2 vec, float a);
        glm::ivec3 roundVec(glm::vec3 v);
        void cleanZBuffer();

    public:
        RendererIn3D(SDL_Renderer *rend, int width, int height, int depth, glm::vec3 &lightPos): 
            m_rend(rend), m_processed(false),
            m_width(width), m_height(height), m_depth(depth),
            m_lightPos(lightPos) 
        {
            int zbufferNum = m_width * m_height;
            m_zbuffer = new float[zbufferNum];
            float minFloat = std::numeric_limits<float>::min();
            for (int i = 0; i < zbufferNum; i++) {
                m_zbuffer[i] = minFloat;
            }
        }
        ~RendererIn3D();

        void render(Model *model);
        void setLight(glm::vec3 light);
        void setProjection(glm::mat4 m);
        void setModelView(glm::mat4 m);
        void setViewport(glm::mat4 m);
};

#endif