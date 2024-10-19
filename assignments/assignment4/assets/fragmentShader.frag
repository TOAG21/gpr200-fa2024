#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
uniform sampler2D texture1;

void main()
{
    vec2 aTexCoord = texCoord;
    FragColor = texture(texture1, aTexCoord);
}