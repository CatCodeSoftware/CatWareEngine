#version 460 core

uniform sampler2D u_Texture;

out vec4 fragColor;
in vec2 v_textureCoords;

void main( )
{
    fragColor = texture( u_Texture, v_textureCoords );
}