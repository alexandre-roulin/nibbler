#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform int colorSnake;

void main()
{
    vec4 color;
    if (colorSnake != 0)
        color = vec4(colorSnake & 0xFF, colorSnake & 0x00FF, colorSnake & 0x0000FF, 1.f);
    else
        color = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(color.xyz, 1.0f);
}