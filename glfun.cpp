#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"

using namespace std;

const char* VERTEX_SHADER_PATH = "shader/vs.glsl";
const char* FRAGMENT_SHADER_PATH = "shader/fs.glsl";
const char* SUZANNE_OBJ_PATH = "mesh/suzanne.obj";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

GLuint vertexBuffer, normalBuffer, elementBuffer;
GLuint program;
GLuint vaoHandle;
GLint uniformMVP, uniformModel, uniformMInvTrans, uniformViewInv;
GLint attrPosition, attrNormal;
Mesh suzanne;

void bufferData(void) {
    utils::loadObj(SUZANNE_OBJ_PATH, suzanne);
    // vertices
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, suzanne.vertices.size() * sizeof(suzanne.vertices[0]), suzanne.vertices.data(), GL_STATIC_DRAW);

    // normals
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, suzanne.normals.size() * sizeof(suzanne.normals[0]), suzanne.normals.data(), GL_STATIC_DRAW);

    // elements
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, suzanne.elements.size() * sizeof(suzanne.elements[0]), suzanne.elements.data(), GL_STATIC_DRAW);

    // Clean up
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

    // Attribs  
    attrPosition  = glGetAttribLocation(program, "position");
    utils::logAttribStatus(attrPosition, "position");
    attrNormal  = glGetAttribLocation(program, "normal");
    utils::logAttribStatus(attrNormal, "normal");

    // Uniforms
    uniformMVP = glGetUniformLocation(program, "mvp");
    utils::logUniformStatus(uniformMVP, "mvp");
    uniformMInvTrans = glGetUniformLocation(program, "mInvTrans");
    utils::logUniformStatus(uniformMInvTrans, "mInvTrans");
    uniformModel = glGetUniformLocation(program, "model");
    utils::logUniformStatus(uniformModel, "model");
    uniformViewInv = glGetUniformLocation(program, "viewInv");
    utils::logUniformStatus(uniformViewInv, "viewInv");

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return program;
}

GLuint initVertexArray(void) {
    // VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(attrPosition);
    glVertexAttribPointer(attrPosition, 4, GL_FLOAT, GL_FALSE, 0, 0); 

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glEnableVertexAttribArray(attrNormal);
    glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, 0, 0); 

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
    initVertexArray();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vaoHandle);

    glDrawElements(GL_TRIANGLES, suzanne.elements.size(), GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void) {
    glUseProgram(program);

    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 15;  // base 15° per second

    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle * 3.0f, glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, angle * 2.0f, glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, angle * 4.0f, glm::vec3(0, 0, 1));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 model = trans * rot;
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 mInvTrans = glm::transpose(glm::inverse(model));
    glUniformMatrix4fv(uniformMInvTrans, 1, GL_FALSE, glm::value_ptr(mInvTrans));

    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 viewInv = glm::inverse(view);
    glUniformMatrix4fv(uniformViewInv, 1, GL_FALSE, glm::value_ptr(viewInv));
    
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 10.0f);
    
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

    glUseProgram(0);
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
    glutIdleFunc(idle);

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    init();

    glutMainLoop();
    return 0;
}
