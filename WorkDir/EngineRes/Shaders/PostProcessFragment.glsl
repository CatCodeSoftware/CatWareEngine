#version 460 core

uniform sampler2D u_Texture;

out vec4 fragColor;
in vec2 v_textureCoords;

uniform vec2 u_Resolution;

uniform float u_Brightness;
uniform float u_Contrast;
uniform float u_Exposure;
uniform float u_Saturation;
uniform float u_Sharpness;

uniform vec4 u_Tint;

// All components are in the range [0…1], including hue.
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main( )
{
    fragColor = texture( u_Texture, v_textureCoords );

    // sharpness
    vec2 step = 1.0 / u_Resolution.xy;

    vec3 texA = texture( u_Texture, v_textureCoords + vec2(-step.x, -step.y) * 1.5 ).rgb;
    vec3 texB = texture( u_Texture, v_textureCoords + vec2( step.x, -step.y) * 1.5 ).rgb;
    vec3 texC = texture( u_Texture, v_textureCoords + vec2(-step.x,  step.y) * 1.5 ).rgb;
    vec3 texD = texture( u_Texture, v_textureCoords + vec2( step.x,  step.y) * 1.5 ).rgb;

    vec3 around = 0.25 * (texA + texB + texC + texD);
    vec3 center  = texture( u_Texture, v_textureCoords ).rgb;

    vec3 col = center + (center - around) * u_Sharpness;

    fragColor = vec4(col, 1.0);

    fragColor.xyz += u_Brightness; // Brightness
    fragColor.xyz = (( fragColor.xyz - 0.5f ) * u_Contrast ) + 0.5f; // contrast
    fragColor.xyz *= u_Exposure; // exposure

    // saturation
    vec3 hsv = rgb2hsv( fragColor.xyz );
    hsv.y += u_Saturation;
    fragColor = vec4( hsv2rgb( hsv ), fragColor.w );

    fragColor *= u_Tint;
}