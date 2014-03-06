CC=g++
FLAGS=-Wall -Wextra -Werror -O2 -std=c++11
LIBS=-I/usr/include -lGL -lGLEW -lglut

glfun: glfun.cpp utils.hpp utils.cpp mesh.cpp texture/snooTexture.c
	$(CC) $(FLAGS) -o glfun glfun.cpp utils.cpp $(LIBS)
