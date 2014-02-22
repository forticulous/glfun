#version 130

uniform mat4 viewInv;
uniform sampler2D suzanneTexture;

in vec2 fuv;
in vec3 fposition;
in vec3 fnormal;

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
    vec3(0.15, 0.15, 0.15),
    vec3(0.5, 0.5, 0.5),
    vec3(1.0, 1.0, 1.0),
    2.5
);

void main() {
    vec3 lightDirection = normalize(light.position - fposition);
    float dotProduct = dot(fnormal, lightDirection);
    vec3 diffuse = light.diffuse * material.diffuse *
                   max(dotProduct, 0.0);

    vec3 cameraWorldSpace = (viewInv * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    vec3 viewDirection = normalize(cameraWorldSpace - fposition);
    vec3 specular;
    if (dotProduct < 0.0) {
        specular = vec3(0.0, 0.0, 0.0);
    } else {
        specular = light.specular * material.specular *
                   pow(max(dot(reflect(-lightDirection, fnormal), viewDirection), 0.0), material.shininess);
    }

    vec4 texColor = texture2D(suzanneTexture, fuv);
    vec3 color = material.ambient + diffuse + specular;
    gl_FragColor = texColor * vec4(color, 1.0);
}
