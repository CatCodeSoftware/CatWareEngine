#version 460 core

layout( location = 0 ) in vec2 position;
layout( location = 1 ) in vec2 textureCoords;

out vec2 v_textureCoords;

uniform mat4 u_Transform;
uniform mat4 u_Projection;

void main( )
{
    v_textureCoords = textureCoords;
    gl_Position = vec4( position, 0, 1 ) * u_Projection;
}