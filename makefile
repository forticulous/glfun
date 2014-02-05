CC=g++
FLAGS=-Wall -Wextra -Werror -O2
LIBS=-I/usr/include -lGL -lGLEW -lglut

glfun: glfun.cpp utils.cpp texture/box.c
	$(CC) $(FLAGS) -o glfun texture/box.c glfun.cpp utils.cpp $(LIBS)
