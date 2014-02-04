#ifndef UTILS_H
#define UTILS_H
#endif
#include <string>
#include <GL/gl.h>

namespace utils {

    using namespace std;

    string fileToString(const char* path);

    void logShaderStatus(string name, GLuint shader);

    void logProgramStatus(GLuint program);

}
