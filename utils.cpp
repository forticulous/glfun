#include <fstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "utils.hpp"

namespace utils {

    using namespace std;

    string fileToString(const char* path) {
        fstream in(path, ios::in);
        if (!in.is_open()) {
            return "";
        }
        string source, line = "";
        while (getline(in, line))
            source += "\n" + line;
        in.close();
        return source;    
    }

    void logShaderStatus(string name, GLuint shader) {
        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_TRUE) {
            return;
        }
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        vector<char> statusMessage(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, &statusMessage[0]);
        cout << "Shader status(" << name << "): " << &statusMessage[0] << endl;
    }

    void logProgramStatus(GLuint program) {
        GLint result;        
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result == GL_TRUE) {
            return;
        }
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        vector<char> statusMessage( max(logLength, int(1)) );
        glGetProgramInfoLog(program, logLength, NULL, &statusMessage[0]);
        cout << "Program status: " << &statusMessage[0] << endl;
    }

    void logError(void) {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR) {
            return;
        }
        cout << "Error: " << err << endl;
    }

    void logAttribStatus(GLint attrib, string name) {
        if (attrib != -1) {
            return;
        }
        cout << "Attrib (" << name << ") not bound." << endl;
    }

    void logUniformStatus(GLint unif, string name) {
        if (unif != -1) {
            return;
        }
        cout << "Uniform (" << name << ") not bound." << endl;
    }

}
