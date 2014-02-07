#version 130

uniform sampler2D boxtexture;

in vec2 ftexcoord;

void main() {
    vec2 fliptexcoord = vec2(ftexcoord.x, 1.0 - ftexcoord.y);
    gl_FragColor = texture2D(boxtexture, fliptexcoord);
}
