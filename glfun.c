#include <string.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

const float vertices[] = {
     0.75f,  0.75f, 0.0f,
     0.75f, -0.75f, 0.0f,
    -0.75f, -0.75f, 0.0f,
};

GLuint vertexBuffer;
GLuint program;

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

    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

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

char *fileToString(const char *path)
{
    FILE *fd;
    long len, r;
    char *str;
 
    if (!(fd = fopen(path, "r")))
    {
        fprintf(stderr, "Can't open file '%s' for reading\n", path);
        return NULL;
    }
 
    fseek(fd, 0, SEEK_END);
    len = ftell(fd);
 
    fseek(fd, 0, SEEK_SET);
 
    if (!(str = malloc(len * sizeof(char))))
    {
        fprintf(stderr, "Can't malloc space for '%s'\n", path);
        return NULL;
    }
 
    r = fread(str, sizeof(char), len, fd);
 
    str[r - 1] = '\0';
 
    fclose(fd);
 
    return str;
}

void initShader(const char* vertexShaderPath)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const char *vertexShaderCode = fileToString(vertexShaderPath);

    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    free((char*)vertexShaderCode);
    glCompileShader(vertexShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
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
    initShader("shader/vs.glsl");

    glutMainLoop();
    return 0;
}
