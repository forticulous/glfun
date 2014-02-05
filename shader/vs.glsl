#version 130

uniform mat4 mvp;

in vec3 position;
in vec2 vtexcoord;
out vec2 ftexcoord;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    ftexcoord = vtexcoord;
}
