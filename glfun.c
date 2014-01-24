#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

static void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argcp, char** argv)
{
    glutInit(&argcp, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");
    glutDisplayFunc(&render);

    glewInit();

    glutMainLoop();
    return 0;
}
