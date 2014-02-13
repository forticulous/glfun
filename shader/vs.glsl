#version 130

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 mInvTrans;
uniform mat4 viewInv;

in vec3 position;
in vec3 normal;
out vec3 color;

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
    vec3 positionWorldSpace = (model * vec4(position, 1.0)).xyz;
    vec3 lightDirection = normalize(light.position - positionWorldSpace);
    vec3 normalWorldSpace = normalize((mInvTrans * vec4(normal, 1.0)).xyz);
    float dotProduct = dot(normalWorldSpace, lightDirection);
    vec3 diffuse = light.diffuse * material.diffuse *
                   max(dotProduct, 0.0);

    vec3 cameraWorldSpace = (viewInv * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    vec3 viewDirection = normalize(cameraWorldSpace - positionWorldSpace);
    vec3 specular;
    if (dotProduct < 0.0) {
        specular = vec3(0.0, 0.0, 0.0);
    } else {
        specular = light.specular * material.specular *
                   pow(max(dot(reflect(-lightDirection, normalWorldSpace), viewDirection), 0.0), material.shininess);
    }

    color = material.ambient + diffuse + specular;

    gl_Position = mvp * vec4(position, 1.0);
}
