CC := g++

APP_NAME := software_render

SRC := main.cpp model.cpp renderer.cpp tgaimage.cpp geometry.cpp gl.cpp

compile:
	$(CC) -o $(APP_NAME) $(SRC) -O0 --std=c++11 -Wall -lSDL2 -lSDL2_image

run: compile
	./$(APP_NAME)

