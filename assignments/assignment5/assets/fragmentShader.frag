#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
in vec3 Normal;  
in vec3 FragPos; 
uniform sampler2D texture1;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec2 aTexCoord = texCoord;

    vec4 lighting = vec4((ambient + diffuse + specular), 1.0);
    //vec4 result = lighting * texture(texture1, aTexCoord);
    vec4 result = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
    FragColor = result;
}