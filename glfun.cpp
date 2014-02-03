#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "utils.hpp"

using namespace std;

const char* VERTEX_SHADER_PATH = "shader/vs.glsl";
const char* FRAGMENT_SHADER_PATH = "shader/fs.glsl";

GLuint program;
GLuint vaoHandle;

GLuint initVertexArray(void) {
    // VBO
    const float vertices[] = {
         0.0f,   0.75f, 0.0f,
         1.0f,   0.0f,  0.0f,
        -0.75f, -0.75f, 0.0f,
         0.0f,   1.0f,  0.0f,
         0.75f, -0.75f, 0.0f,
         0.0f,   0.0f,  1.0f
    };
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*) (sizeof(float) * 3));

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vaoHandle;
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vaoHandle);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

    glutSwapBuffers();
    glutPostRedisplay();
}

GLuint compileShader(const char* path, GLenum type, string name) {
    string vss = utils::fileToString(path);
    const char* cvss = vss.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cvss, NULL);
    glCompileShader(shader);
    utils::logShaderStatus(name, shader);
    return shader;
}

GLuint initProgram(void) {
    GLuint vertexShader = compileShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, "vert");
    GLuint fragShader = compileShader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, "frag");

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return program;
}

void init(void) {
    glewInit();
    initVertexArray();
    initProgram();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");

    glutDisplayFunc(render);

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    init();

    glutMainLoop();
    return 0;
}
