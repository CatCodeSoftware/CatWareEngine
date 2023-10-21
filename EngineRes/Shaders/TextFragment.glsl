#version 460 core
in vec2 v_TexCoords;
out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoords).r);
    color = u_Color * sampled;
}  