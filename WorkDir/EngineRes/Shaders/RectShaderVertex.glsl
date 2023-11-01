#version 460 core

layout( location = 0 ) in vec2 position;

uniform mat4 u_Projection;
uniform mat4 u_Transform;


void main( )
{
    gl_Position = vec4( position, 0, 1 ) * u_Transform * u_Projection;
}