#version 130

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 mInvTrans;

in vec3 position;
in vec3 normal;
out vec3 fposition;
out vec3 fnormal;

struct lightSource {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};
lightSource light = lightSource(
    vec3(-8.0, 0.0, 0.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0)
);

struct materialSource {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
materialSource material = materialSource(
    vec3(0.0, 0.0, 0.15),
    vec3(0.0, 1.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    2.5
);


void main() {
    fposition = (model * vec4(position, 1.0)).xyz;
    fnormal = normalize((mInvTrans * vec4(normal, 1.0)).xyz);

    gl_Position = mvp * vec4(position, 1.0);
}
