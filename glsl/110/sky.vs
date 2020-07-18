#version 110

varying vec3 var_pos;
varying vec3 fsun;
uniform float time;
uniform float aspectRatio;

void main() {
    gl_Position = gl_Vertex;
    gl_Position.z = 1.;
    vec2 pos = gl_Vertex.xy / vec2( 1., aspectRatio);
    var_pos = ( vec4( pos, -1., 0 ) * gl_ModelViewMatrix ).xyz;
    fsun = vec3( 0.0, -cos( radians (time*360.) ), -sin( radians (time*360.) ) );
}