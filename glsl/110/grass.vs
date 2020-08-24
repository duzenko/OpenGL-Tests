#version 110

uniform mat4 modelMatrix;

varying vec3 worldPos;
varying vec3 viewPos;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    worldPos = (modelMatrix*gl_Vertex).xyz;
    viewPos = (gl_ModelViewMatrix*gl_Vertex).xyz;
}