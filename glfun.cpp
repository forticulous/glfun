#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

const char* VERTEX_SHADER_PATH = "shader/vs.glsl";

GLuint vertexBuffer;
GLuint program;

GLuint initVertexBuffer(void) {
    const float vertices[] = {
         0.75f,  0.75f, 0.0f,
         0.75f, -0.75f, 0.0f,
        -0.75f, -0.75f, 0.0f,
    };
    glGenBuffers(1, &vertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vertexBuffer;
}

void render(void) {
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

void idle(void) {
    glutPostRedisplay();
}

int arrLen(char array[]) {
    return sizeof(array) / sizeof(array[0]);
}

string fileToString(const char* path) {
    string source;
    ifstream in(path, ios::in);
    if (!in.is_open()) {
        return "";
    }
    string line = "";
    while (getline(in, line))
        source += "\n" + line;
    in.close();
    return source;
}

GLuint initProgram(const char* vertexShaderPath) {
    string vss = fileToString(vertexShaderPath);
    const char* cvss = vss.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &cvss, NULL);
    glCompileShader(vertexShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    return program;
}

void init(void) {
    glewInit();
    initVertexBuffer();
    initProgram(VERTEX_SHADER_PATH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");

    glutDisplayFunc(render);
    glutIdleFunc(idle);

    cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    init();

    glutMainLoop();
    return 0;
}
