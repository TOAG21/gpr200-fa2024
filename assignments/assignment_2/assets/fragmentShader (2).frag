#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;

void main()
{
    FragColor = texture(texture2, texCoord);
}