#version 130

uniform mat4 mvp;
uniform mat4 view;
uniform vec3 lightposition;

in vec3 position;
in vec3 vnormal;
out vec3 lightvector;
out vec3 fnormal;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    lightvector = (view * vec4(lightposition, 1.0)).xyz - (view * vec4(position, 1.0)).xyz;
    fnormal = vnormal;
}
