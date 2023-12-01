#version 460 core

uniform vec4 u_Tint;
uniform sampler2D u_Texture;
uniform bool u_IsTextured;

out vec4 fragColor;

in vec2 v_textureCoords;

void main( )
{   
    if ( u_IsTextured )
        fragColor = texture( u_Texture, v_textureCoords ) * u_Tint;
    else
        fragColor = u_Tint;
}