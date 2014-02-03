CC=g++
FLAGS=-Wall -Wextra -Werror -O2
LIBS=-I/usr/include/GL -lGL -lGLEW -lglut

glfun: glfun.cpp utils.cpp
	$(CC) $(FLAGS) -o glfun glfun.cpp utils.cpp  $(LIBS)
