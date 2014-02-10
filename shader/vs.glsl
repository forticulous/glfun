#version 130

uniform mat4 mvp;
uniform mat3 m3x3InvTrans;
uniform vec3 lightposition;

in vec3 position;
in vec3 vnormal;
out vec3 lightvector;
out vec3 fnormal;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    lightvector = normalize(lightposition);
    fnormal = normalize(m3x3InvTrans * vnormal);
}
