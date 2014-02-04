#version 130

uniform mat4 mvp;

in vec3 position;
in vec3 vcolor;
out vec3 fcolor;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    fcolor = vcolor;
}
