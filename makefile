CC=g++
FLAGS=-Wall -Wextra -Werror -O2
LIBS=-I/usr/include/GL -lGL -lGLEW -lglut

glfun: glfun.cpp
	$(CC) $(FLAGS) -o glfun glfun.cpp $(LIBS)
