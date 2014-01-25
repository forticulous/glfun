CC=gcc

glfun: glfun.c
	$(CC) -o glfun glfun.c -I/usr/include/GL -lGL -lGLEW -lglut
