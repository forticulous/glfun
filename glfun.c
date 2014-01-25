#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

const float vertices[] = {
     0.75f,  0.75f, 0.0f,
     0.75f, -0.75f, 0.0f,
    -0.75f, -0.75f, 0.0f,
};

GLuint vertexBuffer;

void initVertexBuffer(void)
{
    glGenBuffers(1, &vertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

void idle(void)
{
    glutPostRedisplay();
}

int arrLen(char array[])
{
    return sizeof(array) / sizeof(array[0]);
}

char* fileToString(const char* shaderPath)
{
    char returnVal[1000];
    char buff[1000];

    FILE *file = fopen(shaderPath, "r");
    if (!file)
        printf("Failed to open file\n");

    while (fgets(buff, arrLen(buff), file) != NULL)
        strcat(returnVal, buff);

    fclose(file);
    return returnVal;
}

GLuint initShader(const char* vertexShaderPath)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    char* vertexShaderCode = fileToString(vertexShaderPath);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");

    glutDisplayFunc(render);
    glutIdleFunc(idle);

    glewInit();

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    initVertexBuffer();
    initShader("vs.glsl");

    glutMainLoop();
    return 0;
}
