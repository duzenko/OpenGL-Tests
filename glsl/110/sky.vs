#version 120

varying vec3 var_pos;
varying vec3 var_pos2;
varying vec3 fsun;
uniform mat4 P;
uniform mat4 RX;
uniform mat4 RY;
uniform float time = 0.0;
uniform float aspectRatio = 1.0;

void main() {
    gl_Position = gl_Vertex;
    vec2 pos = gl_Vertex.xy / vec2( 1, aspectRatio);
    var_pos = ( RY * RX * vec4( pos, -1, 1 ) ).xyz;
    var_pos2 = gl_Vertex.xyz;
    fsun = vec3( 0.0, sin( time * 0.01 ), -cos( time * 0.01 ) );
}