// gradient.fs (Fragment Shader)
#version 330

out vec4 FragColor;

uniform vec2 resolution;
uniform vec4 topColor;
uniform vec4 bottomColor;

void main() {
    float t = gl_FragCoord.y / resolution.y;
    FragColor = mix(bottomColor, topColor, t);
}
