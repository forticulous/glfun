#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"

#include "texture/box.c"

using namespace std;

const char* VERTEX_SHADER_PATH = "shader/vs.glsl";
const char* FRAGMENT_SHADER_PATH = "shader/fs.glsl";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

GLuint vertexBuffer, texCoordBuffer, elementBuffer;
GLuint texture;
GLuint program;
GLuint vaoHandle;
GLint uniformMVP, uniformBoxTexture;
GLint attrPosition, attrVTexCoord;

void bufferData(void) {
    // Cube vertices
    const GLfloat vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // top
        -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        // back
         1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,
        // bottom
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        // left
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        // right
         1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0,  1.0,  1.0,
    };
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tex Coords
    GLfloat texcoords[] = {
        // front
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // top
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // back
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // bottom
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // left
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // right
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
    };
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

    // Cube elements
    const GLushort elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        4, 5, 6,
        6, 7, 4,
        // back
        8, 9, 10,
        10, 11, 8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // Texture data
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box.width, box.height, 0, GL_RGB, GL_UNSIGNED_BYTE, box.pixel_data);

    // Clean up
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint compileShader(const char* path, GLenum type, string name);

GLuint initProgram(void) {
    GLuint vertexShader = compileShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, "vs.glsl");
    GLuint fragShader = compileShader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, "fs.glsl");

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    utils::logProgramStatus(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return program;
}

void initAttrib(void) {
    // Attribs  
    attrPosition  = glGetAttribLocation(program, "position");
    utils::logAttribStatus(attrPosition, "position");
    attrVTexCoord  = glGetAttribLocation(program, "vtexcoord");
    utils::logAttribStatus(attrVTexCoord, "vtexcoord");

    // Uniforms
    uniformMVP = glGetUniformLocation(program, "mvp");
    utils::logUniformStatus(uniformMVP, "mvp");
    uniformBoxTexture = glGetUniformLocation(program, "boxtexture");
    utils::logUniformStatus(uniformBoxTexture, "boxtexture");
}

void initUniform(void) {
    glUseProgram(program);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(2.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 10.0f);
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniformBoxTexture, 0);

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint initVertexArray(void) {
    // VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(attrPosition);
    glVertexAttribPointer(attrPosition, 3, GL_FLOAT, GL_FALSE, 0, 0); 

    // Tex Coords
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glEnableVertexAttribArray(attrVTexCoord);
    glVertexAttribPointer(attrVTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vaoHandle;
}

void init(void) {
    glewInit();
    bufferData();
    initProgram();
    initAttrib();
    initUniform();
    initVertexArray();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vaoHandle);
    glBindTexture(GL_TEXTURE_2D, texture);

    int size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Hello World");

    glutDisplayFunc(render);

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    init();

    glutMainLoop();
    return 0;
}
