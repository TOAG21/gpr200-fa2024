#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
in vec3 Normal;  
in vec3 FragPos; 
uniform sampler2D texture1;
uniform float ambientStrength; 
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float specularStrength; 
uniform float diffuseStrength; 
uniform float shininess; 

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec2 aTexCoord = texCoord;
    vec4 tex = texture(texture1, aTexCoord);

    //vec3 norm = normalize(Normal + (tex.xyz * 5 * Normal));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 lighting = vec4((ambient + diffuse + specular), 1.0);
    vec4 result = lighting * tex;
    FragColor = result;
}