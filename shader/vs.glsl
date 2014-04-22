#version 130

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 mInvTrans;

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 fposition;
out vec3 fnormal;
out vec2 fuv;

void main() {
    fposition = (model * vec4(position, 1.0)).xyz;
    fnormal = normalize((mInvTrans * vec4(normal, 1.0)).xyz);
    fuv = uv;

    gl_Position = mvp * vec4(position, 1.0);
}
