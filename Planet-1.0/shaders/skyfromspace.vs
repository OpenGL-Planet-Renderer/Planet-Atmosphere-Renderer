#version 330 core

layout (location = 0) in vec3 Vertex;

uniform vec3 v3CameraPos;
uniform vec3 v3LightPos;
uniform vec3 v3InvWavelength;
uniform float fCameraHeight;
uniform float fCameraHeight2;
uniform float fOuterRadius;
uniform float fOuterRadius2;
uniform float fInnerRadius;
uniform float fInnerRadius2;
uniform float fKrESun;
uniform float fKmESun;
uniform float fKr4PI;
uniform float fKm4PI;
uniform float fScale;
uniform float fScaleDepth;
uniform float fScaleOverScaleDepth;
uniform mat4 MVPmatrix;

const int nSamples = 4;
const float fSamples = 4.0;

out vec3 color;
out vec3 secondaryColor;
out vec3 v3Direction;

float scale(float fCos){
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void){
    
    vec3 v3Pos = Vertex.xyz;
    vec3 v3Ray = v3Pos - v3CameraPos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;
    
    vec3 v3Start = v3CameraPos;
    float fHeight = length(v3Start);
    float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartOffset = fDepth*scale(fStartAngle);
    float fSampleLength = fFar / fSamples;
    float fScaledLength = fSampleLength * fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
    color = vec3(0.0, 0.0, 0.0);
    
    for(int i=0; i<nSamples; i++)    {
        
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
        
        color += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
        
    }
    
    secondaryColor = color * fKmESun;
    color = color * (v3InvWavelength * fKrESun);
    gl_Position = MVPmatrix * vec4(Vertex,1.0);
    v3Direction = v3CameraPos - v3Pos;
    
}
