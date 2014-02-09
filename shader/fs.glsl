#version 130

uniform vec3 color;

in vec3 lightvector;
in vec3 fnormal;

void main() {
    float dotproduct = max(dot(normalize(lightvector), normalize(fnormal)), 0.0);
    gl_FragColor = dotproduct * vec4(color, 1.0);
}
