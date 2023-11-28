#version 460 core

layout( location = 0 ) in vec2 position;
layout( location = 1 ) in vec2 textureCoords;

uniform mat4 u_Projection;
uniform mat4 u_Transform;

uniform vec2 u_Position;
uniform vec2 u_Size;

out vec2 v_textureCoords;

void main( )
{
    v_textureCoords = textureCoords;

    vec2 finalPosition;
    finalPosition.x = ( position.x + u_Position.x ) * u_Size.x;
    finalPosition.y = ( position.y + u_Position.y ) * u_Size.y;

    gl_Position = vec4(finalPosition, 0, 1 ) * u_Transform * u_Projection;
}