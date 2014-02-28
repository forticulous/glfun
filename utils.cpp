#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
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

    void loadObj(const char* filename, Mesh& mesh) {
        ifstream in(filename, ios::in);
        if (!in) {
            cerr << "Cannot open " << filename << endl; 
            exit(1); 
        }

        vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        vector<glm::vec3> temp_vertices;
        vector<glm::vec2> temp_uvs;
        vector<glm::vec3> temp_normals;       

        string line;
        while (getline(in, line)) {
            if (line[0] == '#') {
                // comment
                continue;
            } else if (line.substr(0,2) == "v ") {
                // vertex
                glm::vec3 vertex;
                sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            } else if (line.substr(0, 3) == "vt ") {
                // texture
                glm::vec2 uv;
                sscanf(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
                uv.y = -uv.y; // Textures are inverted
                temp_uvs.push_back(uv);
            } else if (line.substr(0, 3) == "vn ") {
                // normals
                glm::vec3 normal;
                sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            } else if (line.substr(0, 2) == "f ") {
                // fragment
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
                    &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
                    &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

                if (matches != 9) {
                    cout << "parser can't read fragment line" << endl;
                    return;
                }

                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            } else { 
                // ignoring this line
            }
        }

        unordered_map<string, unsigned int> uniques;
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            // Get the indices of its attributes
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];
            
            // Get the attributes thanks to the index
            glm::vec3 vertex = temp_vertices[vertexIndex - 1];
            glm::vec2 uv = temp_uvs[uvIndex - 1];
            glm::vec3 normal = temp_normals[normalIndex - 1];
            
            // Put the attributes in buffers
            mesh.vertices.push_back(vertex);
            mesh.uvs.push_back(uv);
            mesh.normals.push_back(normal);

            // Limit elements to unique combination of vertex + uv + normal
            string key = to_string(vertexIndex) + to_string(uvIndex) + to_string(normalIndex);
            if (uniques.count(key) == 0) {
                mesh.elements.push_back(i);
                uniques.insert(make_pair(key, i));
            } else {
                mesh.elements.push_back(uniques[key]);
            }
        }

    }

}
