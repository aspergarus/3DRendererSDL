#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "model.h"
#include "renderer.h"
#include "tgaimage.h"
#include "geometry.h"
#include "gl.h"

const int windowW = 800;
const int windowH = 800;
const int depth = 255;

float lightSpeed = 0.1;
// glm::vec3 lightPos = glm::vec3(1.0, -1.0, 1.0);
glm::vec3 lightPos = glm::vec3(1.0, -1.0, 2.0);
glm::vec3 camera(1, 1, 3);
glm::vec3 eye(1, 1, 3);
glm::vec3 center(0,0,0);
glm::vec3 up(0, 1, 0);

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* win = SDL_CreateWindow("GAME",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowW, windowH, SDL_WINDOW_SHOWN
	);

	// triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	// Setup renderer with SDL_Renderer reference, size and position of light
	RendererIn3D* renderer = new RendererIn3D(rend, windowW, windowH, depth, lightPos);

	Model *model = new Model("./assets/african_head.obj");
	// Model *cube = new Model(MODEL_PLANES);

    // controls animation loop
    int close = 0;

	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool moveForward = false;
	bool moveBack = false;

	// true only for first render. Then we will rerender model only if it moves
	bool sceneChanged = true;
 
    // animation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					close = 1;
					break;
	
				case SDL_KEYUP:
					// std::cout << "UP State:" << (event.key.state ? "pressed" : "released") << " Key: " << event.key.keysym.scancode << std::endl;
					if (event.key.state == SDL_RELEASED) {
						switch (event.key.keysym.scancode) {
							case SDL_SCANCODE_W:
							case SDL_SCANCODE_UP:
								moveUp = false;
								break;
							case SDL_SCANCODE_A:
							case SDL_SCANCODE_LEFT:
								moveLeft = false;
								break;
							case SDL_SCANCODE_S:
							case SDL_SCANCODE_DOWN:
								moveDown = false;
								break;
							case SDL_SCANCODE_D:
							case SDL_SCANCODE_RIGHT:
								moveRight = false;
								break;
							case SDL_SCANCODE_Z:
								moveForward = false;
								break;
							case SDL_SCANCODE_X:
								moveBack = false;
								break;
							default:
								break;
						}
					}
					break;
				
				case SDL_KEYDOWN:
					// std::cout << "DOWN State:" << (event.key.state ? "pressed" : "released") << " Key: " << event.key.keysym.scancode << std::endl;

					// keyboard API for key pressed
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							moveUp = true;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							moveLeft = true;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							moveDown = true;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							moveRight = true;
							break;
						case SDL_SCANCODE_Z:
							moveForward = true;
							break;
						case SDL_SCANCODE_X:
							moveBack = true;
							break;
						default:
							break;
					}
					break;
			}
        }

		if (moveLeft) {
			lightPos.x += lightSpeed;
			sceneChanged = true;
		}
		if (moveRight) {
			lightPos.x -= lightSpeed;
			sceneChanged = true;
		}
		if (moveUp) {
			lightPos.y += lightSpeed;
			sceneChanged = true;
		}
		if (moveDown) {
			lightPos.y -= lightSpeed;
			sceneChanged = true;
		}
		if (moveForward) {
			lightPos.z += lightSpeed;
			sceneChanged = true;
		}
		if (moveBack) {
			lightPos.z -= lightSpeed;
			sceneChanged = true;
		}

		if (sceneChanged) {
			glm::mat4 projectionMat = projection(-1.f / glm::length(eye - center));
			glm::mat4 viewPort = viewport(windowW/8, windowH/8, windowW * 3 / 4, windowH * 3 / 4, depth);
			glm::mat4 modelView = lookat(eye, center, up);
			
			renderer->setModelView(modelView);
			renderer->setProjection(projectionMat);
			renderer->setViewport(viewPort);

			lightPos = glm::normalize(lightPos);
			renderer->setLight(lightPos);

			renderer->render(model);
			
			sceneChanged = false;
		}
 
        // calculates to 60 fps
        SDL_Delay(1000 / 300);
    }

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();

    return 0;
}
