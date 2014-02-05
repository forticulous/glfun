#version 130

uniform sampler2D boxtexture;

in vec2 ftexcoord;

void main() {
    gl_FragColor = texture2D(boxtexture, ftexcoord);
}
