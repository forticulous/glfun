#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Mesh {
    public:
        vector<glm::vec3> vertices;
        vector<glm::vec2> uvs;
        vector<glm::vec3> normals;
        vector<GLushort> elements;
        Mesh() {}
};
