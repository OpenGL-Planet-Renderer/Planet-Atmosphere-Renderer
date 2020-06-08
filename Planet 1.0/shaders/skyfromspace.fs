#version 330 core

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

in vec3 color;
in vec3 secondaryColor;
in vec3 v3Direction;

out vec4 FragColor;

void main (void){
    float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    FragColor.xyz = color + fMiePhase * secondaryColor;
    FragColor.w = 1.0;
}
