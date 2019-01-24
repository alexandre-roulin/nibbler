#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform int colorSnake;

//vec3        lightAmbient = vec3(0.15f);
//vec3        lightDiffuse = vec3(0.85f);
//vec3        lightSpecular = vec3(1.f);
//vec3        lightPosition = vec3(0.f, 17.f, 30.f);
//
//float       defaultShininess = 76.8f;
//vec3        defaultAmbient = vec3(0.0215f, 0.1745f, 0.0215f);
//vec3        defaultDifuse = vec3(0.0756f, 0.61423f, 0.07568f);
//vec3        defaultSpecular = vec3(0.633f, 0.7278f, 0.633f);
//
//vec3	phong()
//{
//	vec3					dirLight;
//	vec3					norm;
//	vec3					diffuse;
//	vec3					ambient;
//	vec3					result_phong;
//
//	ambient = defaultAmbient * lightAmbient;
//	norm = normalize(Normal);
//	dirLight = normalize(Position - lightPosition);
//	diffuse = max(dot(norm, -dirLight), 0) * defaultDifuse * lightDiffuse;
//	result_phong = (ambient + diffuse);
//	return (result_phong);
//}

void main()
{
    vec4 color;
    if (colorSnake != 0)
        color = vec4(colorSnake & 0xFF, (colorSnake >> 8) & 0xFF, (colorSnake >> 16) & 0xFF, 1.f);
    else {
        color = texture(texture_diffuse1, TexCoords);
    }
    FragColor = vec4(color.xyz, 1.0f);
}