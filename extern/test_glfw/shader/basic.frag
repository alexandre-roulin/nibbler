#version 330 core
out vec4 FragColor;

struct tMaterial
{
    float	shininess;
	vec3	ambient;
	vec3	diffuse;
	vec3	specular;
};

struct tLight
{
	vec3	position;
	float	intensity;
};

smooth in vec2 TexCoords;
smooth in vec3 Normal;
smooth in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform int uColorSnake;
uniform vec3 uCameraPosition;
uniform tMaterial		uMaterial;

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
        return (1.f);
    else if (intensity > 0.6f)
        return (0.8f);
    else if (intensity > 0.4f)
        return (0.6f);
    else if (intensity > 0.2f)
        return (0.4f);
    else if (intensity > 0.1f)
        return (0.1f);
    return (0.f);
}

vec3	phong()
{
    vec3 posToLight = normalize(lightPosition - Position);
    vec3 posToCamera = normalize(uCameraPosition - Position);

    float diffuse =  max(dot(Normal, posToLight), 0);
    //vec3 diffuseColor = defaultDifuse * floor(diffuse * levels) * scaleFactor;
    vec3 diffuseColor = uMaterial.diffuse * celShading(diffuse);


    vec3 cameraToLight = normalize(posToLight + posToCamera);
    float specular = 0.f;
    if(dot(posToLight, Normal) > 0.f)
        specular = pow(max(0, dot(cameraToLight, Normal)), uMaterial.shininess);
    float specMask = (pow(dot(cameraToLight, Normal), uMaterial.shininess) > 0.4f) ? 1.f : 0.f;

    vec3 color = (uMaterial.ambient + diffuseColor + (uMaterial.specular * specular * specMask));

    return (color);
}

void main()
{
    vec4 color;
    if (uColorSnake != 0) {
        color = vec4((uColorSnake >> 16) & 0xFF, (uColorSnake >> 8) & 0xFF, (uColorSnake) & 0xFF, 1.f);
        color /= 255.f;
        //defaultDifuse = color.rgb;
        //defaultSpecular = color.rgb;
        //defaultAmbient = color.rgb * 0.1f;
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