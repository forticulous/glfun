#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Mesh {
    private:
        GLuint vbo_vertices, vbo_normals, ibo_elements;
    public:
        vector<glm::vec4> vertices;
        vector<glm::vec3> normals;
        vector<GLushort> elements;
        glm::mat4 object2world;
        Mesh() : vbo_vertices(0), vbo_normals(0), ibo_elements(0), object2world(glm::mat4(1)) {}
};
