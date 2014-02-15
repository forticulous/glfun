#version 130

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 mInvTrans;
uniform mat4 viewInv;

in vec3 position;
in vec3 normal;
out vec3 fposition;
out vec3 fnormal;


void main() {
    fposition = (model * vec4(position, 1.0)).xyz;
    fnormal = normalize((mInvTrans * vec4(normal, 1.0)).xyz);

    gl_Position = mvp * vec4(position, 1.0);
}
