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
        int result;
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

}
