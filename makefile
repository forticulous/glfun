CC=g++
FLAGS=-Wall -Wextra -Werror -O2
LIBS=-I/usr/include -lGL -lGLEW -lglut

glfun: glfun.cpp utils.hpp utils.cpp mesh.cpp texture/box.c
	$(CC) $(FLAGS) -o glfun glfun.cpp utils.cpp $(LIBS)
