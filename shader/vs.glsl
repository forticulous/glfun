#version 130

in vec3 position;
in vec3 vcolor;
out vec3 fcolor;

void main() {
    gl_Position = vec4(position, 1.0);
    fcolor = vcolor;
}
