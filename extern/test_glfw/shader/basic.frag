#version 330 core
out vec4 FragColor;

smooth in vec2 TexCoords;
smooth in vec3 Normal;
smooth in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform int colorSnake;
uniform vec3 cameraPosition;

float       lightAmbient = 0.15f;
float       lightDiffuse = 1.f;
float       lightSpecular = 1.f;
vec3        lightPosition = vec3(0.f, 0.f, 30.f);

float       defaultShininess = 102.8f;
vec3        defaultAmbient = vec3(0.0215f, 0.1745f, 0.0215f);
vec3        defaultDifuse = vec3(0.0756f, 0.61423f, 0.07568f);
vec3        defaultSpecular = vec3(0.633f, 0.7278f, 0.633f);

const int levels = 4;
const float scaleFactor = 1.f / levels;

float celShading(float intensity)
{
    if (intensity > 0.93f)
        return (1.0f);
    else if (intensity > 0.6f)
        return (0.8f);
    else if (intensity > 0.4f)
        return (0.6f);
    else if (intensity > 0.2f)
        return (0.4f);
    else if (intensity > 0.1f)
        return (0.1f);
    return (0.0f);
}

vec3	phong()
{
    vec3 posToLight = normalize(lightPosition - Position);
    vec3 posToCamera = normalize(cameraPosition - Position);

    float diffuse =  max(dot(Normal, posToLight), 0);
    //vec3 diffuseColor = defaultDifuse * floor(diffuse * levels) * scaleFactor;
    vec3 diffuseColor = defaultDifuse * celShading(diffuse);


    vec3 cameraToLight = normalize(posToLight + posToCamera);
    float specular = 0.0f;
    if(dot(posToLight, Normal) > 0.0f)
        specular = pow(max(0, dot(cameraToLight, Normal)), defaultShininess);
    float specMask = (pow(dot(cameraToLight, Normal), defaultShininess) > 0.4f) ? 1 : 0;

    vec3 color = (diffuseColor + specular * specMask);

    return (color);
}

void main()
{
    vec4 color;
    if (colorSnake != 0) {
        color = vec4((colorSnake >> 16) & 0xFF, (colorSnake >> 8) & 0xFF, (colorSnake) & 0xFF, 1.f);
        color /= 255.f;
        defaultDifuse = color.rgb;
        defaultSpecular = color.rgb;
        defaultAmbient = color.rgb * 0.1f;
        color = vec4(phong(), 1.f);
    }
    else {
        color = texture(texture_diffuse1, TexCoords);

        defaultDifuse = color.rgb;
        defaultSpecular = color.rgb;
        defaultAmbient = color.rgb * 0.1f;
        color = vec4(phong(), 1.f);
    }
    FragColor = vec4(color.rgb, 1.0f);
}