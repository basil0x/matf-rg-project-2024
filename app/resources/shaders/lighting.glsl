//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

uniform vec3 dirLightDirection;
uniform vec3 dirLightColor;
uniform float dirLightIntensity;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform float pointLightIntensity;

void main() {
    vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = 0.1 * texColor;

    vec3 dirDir = normalize(-dirLightDirection);
    float dirDiff = max(dot(norm, dirDir), 0.0);
    vec3 dirDiffuse = dirDiff * dirLightColor * dirLightIntensity * texColor;
    vec3 dirHalfway = normalize(dirDir + viewDir);
    float dirSpec = pow(max(dot(norm, dirHalfway), 0.0), 32.0);
    vec3 dirSpecular = dirSpec * dirLightColor * dirLightIntensity;

    vec3 pointDir = normalize(pointLightPos - FragPos);
    float pointDiff = max(dot(norm, pointDir), 0.0);
    float dist = length(pointLightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
    vec3 pointDiffuse = pointDiff * pointLightColor * pointLightIntensity * texColor * attenuation;
    vec3 pointHalfway = normalize(pointDir + viewDir);
    float pointSpec = pow(max(dot(norm, pointHalfway), 0.0), 32.0);
    vec3 pointSpecular = pointSpec * pointLightColor * pointLightIntensity * attenuation;

    vec3 result = ambient + dirDiffuse + dirSpecular + pointDiffuse + pointSpecular;
    FragColor = vec4(result, 1.0);
}