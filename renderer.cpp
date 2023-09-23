#include <iostream>

#include "renderer.h"
#include "model.h"
#include "geometry.h"
#include "gl.h"

int debug = 0;

void RendererIn3D::render(Model *model) {
    if (!m_processed || true) {
        this->m_model = model;

        // clears the screen
        SDL_RenderClear(m_rend);
        
        if (debug) {
            renderFace(model, 600);
            // renderFace(model, 601);
        } else {
            renderModel(model);
        }

        // return back black screen
        SDL_SetRenderDrawColor(m_rend, 0x00, 0x00, 0x00, 0xFF);

        // triggers the double buffers for multiple rendering
        SDL_RenderPresent(m_rend);

        m_processed = true;
        cleanZBuffer();
    }
}

void RendererIn3D::renderModel(Model *model) {
    for (int i = 0; i < model->nfaces(); i++) { 
		renderFace(model, i);
	}
}

void RendererIn3D::renderFace(Model *model, int faceId) {
    std::vector<int> face = model->face(faceId);
	glm::ivec3 screenCoords[3]; 
	glm::vec3 worldCoords[3];
    float intensitys[3];

	for (int j = 0; j < 3; j++) { 
		glm::vec3 v = model->vert(face[j]);

        screenCoords[j] = vec4ToVec3(vec3ToVec4(v) * m_modelView * m_projection * m_viewport);
        intensitys[j] = glm::dot(model->norm(faceId, j), m_lightPos);

        if (debug) {
            std::cout << "init vec3\n"; printVec(v);
            std::cout << "init vec4\n"; printVec(vec3ToVec4(v));
            std::cout << "vertice in projection:\n"; printVec(m_viewport * vec3ToVec4(v));
            std::cout << "vertice in projection and in viewport:\n"; printVec(m_projection * m_viewport * vec3ToVec4(v));
            std::cout << "vertice on screen:\n"; printVec(vec4ToVec3(m_projection * m_viewport * vec3ToVec4(v)));
            return;
        }

		worldCoords[j] = v;
	}

	glm::vec3 normal = glm::cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]);
	float intensity = glm::dot(glm::normalize(normal), m_lightPos);

    // shadow/lighting model
	// if (intensity > 0) {
    //     // add texture color
    //     glm::ivec2 uv[3];
    //     for (int k = 0; k < 3; k++) {
    //         uv[k] = model->uv(faceId, k);
    //     }

	// 	fillTriangleSmooth(screenCoords[0], screenCoords[1], screenCoords[2], uv[0], uv[1], uv[2], intensitys);
    //     if (debug) {
    //         return;
    //     }
	// }

    glm::ivec2 uv[3];
    for (int k = 0; k < 3; k++) {
        uv[k] = model->uv(faceId, k);
    }

    fillTriangleSmooth(screenCoords[0], screenCoords[1], screenCoords[2], uv[0], uv[1], uv[2], intensitys);
}

void RendererIn3D::fillTriangleSmooth(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec2 uv0, glm::ivec2 uv1, glm::ivec2 uv2, float intensity[3]) {
    if (v1.y==v2.y && v1.y==v3.y) return;
    if (v1.y>v2.y) {
        std::swap(v1, v2);
        std::swap(uv0, uv1);
        std::swap(intensity[0], intensity[1]);
    }
    if (v1.y>v3.y) {
        std::swap(v1, v3);
        std::swap(uv0, uv2);
        std::swap(intensity[0], intensity[2]);
    }
    if (v2.y>v3.y) {
        std::swap(v2, v3);
        std::swap(uv1, uv2);
        std::swap(intensity[1], intensity[2]);
    }
    int totalHeight = v3.y - v1.y;

	// i is y moving point of row, goes from 0 to totalHeight of triangle
    for (int i = 0; i < totalHeight; i++) {
        if (debug) {
            if (i > 5) {
                break;
            }
        }

        bool secondHalf = i >= v2.y - v1.y; 
        int segmentHeight = secondHalf ? v3.y - v2.y : v2.y - v1.y; 
        
		float alpha = (float)i / totalHeight;
        float beta  = (float)(i - (secondHalf ? v2.y - v1.y : 0)) / segmentHeight;

		glm::ivec3 A = v1 + vecProdFloat(v3 - v1, alpha);
        glm::ivec3 B = (secondHalf ? v2 : v1) + vecProdFloat((secondHalf ? v3 - v2 : v2 - v1), beta);

        glm::vec2 uvA = glm::vec2(uv0) + (glm::vec2(uv2) - glm::vec2(uv0)) * alpha;
        
        glm::vec2 uvB1 = (secondHalf ? glm::vec2(uv1) : glm::vec2(uv0));
        glm::vec2 uvB2 = beta * (secondHalf ? (glm::vec2(uv2) - glm::vec2(uv1)) : (glm::vec2(uv1) - glm::vec2(uv0)));
        glm::vec2 uvB = uvB1 + uvB2;

        float ityA =              intensity[0] + (intensity[2] - intensity[0]) * alpha;
        float ityB = secondHalf ? intensity[1] + (intensity[2] - intensity[1]) * beta : intensity[0] + (intensity[1] - intensity[0]) * beta;

        if (debug) {
            std::cout << "alpha " << alpha << ", beta " << beta << std::endl;
            std::cout << "uvA ";
            printVec(uvA);
            std::cout << "uvB ";
            printVec(uvB);
            std::cout << "\n";
        }
        
		if (A.x > B.x) {
            std::swap(A, B);
            std::swap(uvA, uvB);
            std::swap(ityA, ityB);
        }

		// j is x moving point on the current line, which we need to draw
        for (int j = A.x; j <= B.x; j++) {
			// interpolate coord z on the plane between A and B vectors.
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            glm::ivec3 P = glm::vec3(A) + glm::vec3(vecProdFloat(B - A, phi));

            float ityP = ityA + (ityB - ityA) * phi;

            // calculate point on texture
            glm::vec2 uvFP = uvA + (uvB - uvA) * phi;
            glm::ivec2 uvP = round(uvFP);

			int idx = P.x + P.y * m_width;
            if (m_zbuffer[idx] < P.z) {
                m_zbuffer[idx] = P.z;

                // get color of pixel on plane from texture
                TGAColor color = m_model->diffuse(uvP);
                
                // Making color gradation of grey 
                // TGAColor color; 
                // color.r = 255 * 1;
                // color.g = 255 * 1;
                // color.b = 255 * 1;

                // apply intensity of lighting
                SDL_SetRenderDrawColor(m_rend, round(color.r * ityP), round(color.g * ityP), round(color.b * ityP), 0xFF);
                drawPixelFlippedVert(P.x, P.y);
			} 
        } 
    }

	SDL_SetRenderDrawColor(m_rend, 0xFF, 0xFF, 0x00, 0xFF);
}

void RendererIn3D::fillTriangle(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec2 uv0, glm::ivec2 uv1, glm::ivec2 uv2, float intensity) {
    if (v1.y==v2.y && v1.y==v3.y) return;
    if (v1.y>v2.y) {
        std::swap(v1, v2);
        std::swap(uv0, uv1);
    }
    if (v1.y>v3.y) {
        std::swap(v1, v3);
        std::swap(uv0, uv2);
    }
    if (v2.y>v3.y) {
        std::swap(v2, v3);
        std::swap(uv1, uv2);
    }
    int totalHeight = v3.y - v1.y;

    // m_model->norms_;

	// i is y moving point of row, goes from 0 to totalHeight of triangle
    for (int i = 0; i < totalHeight; i++) {
        
        if (debug) {
            if (i > 5) {
                break;
            }
        }


        bool secondHalf = i >= v2.y - v1.y; 
        int segmentHeight = secondHalf ? v3.y - v2.y : v2.y - v1.y; 
        
		float alpha = (float)i / totalHeight;
        float beta  = (float)(i - (secondHalf ? v2.y - v1.y : 0)) / segmentHeight;

		glm::ivec3 A = v1 + vecProdFloat(v3 - v1, alpha);
        // glm::ivec3 B = secondHalf ? v2 + vecProdFloat(v3 - v2, beta) : v1 + vecProdFloat(v2 - v1, beta);
        glm::ivec3 B = (secondHalf ? v2 : v1) + vecProdFloat((secondHalf ? v3 - v2 : v2 - v1), beta);

        glm::vec2 uvA = glm::vec2(uv0) + (glm::vec2(uv2) - glm::vec2(uv0)) * alpha;
        
        glm::vec2 uvB1 = (secondHalf ? glm::vec2(uv1) : glm::vec2(uv0));
        glm::vec2 uvB2 = beta * (secondHalf ? (glm::vec2(uv2) - glm::vec2(uv1)) : (glm::vec2(uv1) - glm::vec2(uv0)));
        glm::vec2 uvB = uvB1 + uvB2;

        if (debug) {
            std::cout << "alpha " << alpha << ", beta " << beta << std::endl;
            std::cout << "uvA ";
            printVec(uvA);
            std::cout << "uvB ";
            printVec(uvB);
            std::cout << "\n";
        }
        
		if (A.x > B.x) {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }

		// j is x moving point on the current line, which we need to draw
        for (int j = A.x; j <= B.x; j++) {
			// interpolate coord z on the plane between A and B vectors.
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            glm::ivec3 P = glm::vec3(A) + glm::vec3(vecProdFloat(B - A, phi));

            // calculate point on texture
            glm::vec2 uvFP = uvA + (uvB - uvA) * phi;
            glm::ivec2 uvP = round(uvFP);

			int idx = P.x + P.y * m_width;
            if (m_zbuffer[idx] < P.z) {
                m_zbuffer[idx] = P.z;

                // get color of pixel on plane from texture
                TGAColor color = m_model->diffuse(uvP);
                
                // Making color gradation of grey 
                // TGAColor color; 
                // color.r = 255 * intensity;
                // color.g = 255 * intensity;
                // color.b = 255 * intensity;

                // apply intensity of lighting
                SDL_SetRenderDrawColor(m_rend, round(color.r * intensity), round(color.g * intensity), round(color.b * intensity), 0xFF);
                drawPixelFlippedVert(P.x, P.y);
			} 
        } 
    }

	SDL_SetRenderDrawColor(m_rend, 0xFF, 0xFF, 0x00, 0xFF);
}

// fill triangle without color(use only 1 color and change it depending on the light.
// For example from white to black through all gradations of grey.
void RendererIn3D::fillTriangle(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3, glm::ivec3 color) {
	SDL_SetRenderDrawColor(m_rend, color[0], color[1], color[2], 0xFF);

    if (v1.y==v2.y && v1.y==v3.y) return;
    if (v1.y>v2.y) std::swap(v1, v2);
    if (v1.y>v3.y) std::swap(v1, v3);
    if (v2.y>v3.y) std::swap(v2, v3);
    int totalHeight = v3.y - v1.y;

	// i is y moving point of row, goes from 0 to totalHeight of triangle
    for (int i = 0; i < totalHeight; i++) { 
        bool secondHalf = i >= v2.y - v1.y; 
        int segmentHeight = secondHalf ? v3.y - v2.y : v2.y - v1.y; 
        
		float alpha = (float)i / totalHeight;
        float beta  = (float)(i - (secondHalf ? v2.y - v1.y : 0)) / segmentHeight;
		
		glm::ivec3 A = v1 + vecProdFloat(v3 - v1, alpha); 
        glm::ivec3 B = secondHalf ? v2 + vecProdFloat(v3 - v2, beta) : v1 + vecProdFloat(v2 - v1, beta); 
        
		if (A.x > B.x) std::swap(A, B); 

		// j is x moving point on the current line, which we need to draw
        for (int j = A.x; j <= B.x; j++) {
			// interpolate coord z on the plane between A and B vectors.
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (B.x - A.x);
            glm::ivec3 P = glm::vec3(A) + glm::vec3(vecProdFloat(B - A, phi));
            
			int idx = P.x + P.y * m_width;
            if (m_zbuffer[idx] < P.z) {
                m_zbuffer[idx] = P.z;
                drawPixelFlippedVert(P.x, P.y);
			} 
        } 
    }

	SDL_SetRenderDrawColor(m_rend, 0xFF, 0xFF, 0x00, 0xFF);
}

glm::ivec3 RendererIn3D::vecProdFloat(glm::ivec3 vec, float a) {
	return glm::ivec3(round(vec.x * a), round(vec.y * a), round(vec.z * a));
}

glm::ivec2 RendererIn3D::vec2ProdFloat(glm::ivec2 vec, float a) {
	return glm::ivec2(round(vec.x * a), round(vec.y * a));
}

void RendererIn3D::drawPixelFlippedVert(int x, int y) {
	SDL_RenderDrawPoint(m_rend, x, m_height - y);
}

void RendererIn3D::cleanZBuffer() {
    int zbufferNum = m_width * m_height;
    float minFloat = std::numeric_limits<float>::min();
    for (int i = 0; i < zbufferNum; i++) {
        m_zbuffer[i] = minFloat;
    }
}

void RendererIn3D::setLight(glm::vec3 light) {
    m_lightPos = light;
}

void RendererIn3D::setProjection(glm::mat4 m) {
    m_projection = m;
}

void RendererIn3D::setModelView(glm::mat4 m) {
    m_modelView = m;
}

void RendererIn3D::setViewport(glm::mat4 m) {
    m_viewport = m;
}

RendererIn3D::~RendererIn3D() {
    delete[] m_zbuffer;
}
