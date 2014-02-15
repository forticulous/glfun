#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
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

    void loadObj(const char* filename, Mesh& mesh) {
        ifstream in(filename, ios::in);
        if (!in) {
            cerr << "Cannot open " << filename << endl; 
            exit(1); 
        }
       
        string line;
        while (getline(in, line)) {
            if (line.substr(0,2) == "v ") {
                istringstream s(line.substr(2));
                glm::vec4 v; 
                s >> v.x; 
                s >> v.y; 
                s >> v.z; 
                v.w = 1.0f;
                mesh.vertices.push_back(v);
            } else if (line.substr(0,2) == "f ") {
                istringstream s(line.substr(2));
                GLushort a,b,c;
                s >> a; 
                s >> b; 
                s >> c;
                a--; 
                b--; 
                c--;
                mesh.elements.push_back(a); 
                mesh.elements.push_back(b); 
                mesh.elements.push_back(c);
            } else if (line[0] == '#') {
                /* ignoring this line */ 
            } else { 
                /* ignoring this line */ 
            }
        }
       
        mesh.normals.resize(mesh.vertices.size(), glm::vec3(0.0, 0.0, 0.0));
        vector<GLushort> nb_seen(mesh.vertices.size(), 0);
        for (vector<GLushort>::size_type i = 0; i < mesh.elements.size(); i+=3) {
            GLushort ia = mesh.elements[i];
            GLushort ib = mesh.elements[i+1];
            GLushort ic = mesh.elements[i+2];
            glm::vec3 normal = glm::normalize(glm::cross(
                glm::vec3(mesh.vertices[ib]) - glm::vec3(mesh.vertices[ia]),
                glm::vec3(mesh.vertices[ic]) - glm::vec3(mesh.vertices[ia])));

            int v[3]; 
            v[0] = ia; 
            v[1] = ib; 
            v[2] = ic;
            for (int j = 0; j < 3; j++) {
                GLushort cur_v = v[j];
                nb_seen[cur_v]++;
                if (nb_seen[cur_v] == 1) {
                    mesh.normals[cur_v] = normal;
                } else {
                    // average
                    mesh.normals[cur_v].x = mesh.normals[cur_v].x * (1.0 - 1.0 / nb_seen[cur_v]) + normal.x * 1.0 / nb_seen[cur_v];
                    mesh.normals[cur_v].y = mesh.normals[cur_v].y * (1.0 - 1.0 / nb_seen[cur_v]) + normal.y * 1.0 / nb_seen[cur_v];
                    mesh.normals[cur_v].z = mesh.normals[cur_v].z * (1.0 - 1.0 / nb_seen[cur_v]) + normal.z * 1.0 / nb_seen[cur_v];
                    mesh.normals[cur_v] = glm::normalize(mesh.normals[cur_v]);
                }
            }
        }
    }

}
