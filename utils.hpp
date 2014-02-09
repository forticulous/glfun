#ifndef UTILS_H
#define UTILS_H
#endif
#include <string>
#include <vector>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "mesh.cpp"

namespace utils {

    using namespace std;

    string fileToString(const char* path);

    void logShaderStatus(string name, GLuint shader);

    void logProgramStatus(GLuint program);

    void logError(void);

    void logAttribStatus(GLint attrib, string name);

    void logUniformStatus(GLint attrib, string name);

    void loadObj(const char* filename, Mesh& mesh); 

}
