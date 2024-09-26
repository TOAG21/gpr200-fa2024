#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;

void main()
{
    vec2 aTexCoord = texCoord;
    aTexCoord.x += time / 10.0f;
    FragColor = texture(texture1, aTexCoord);
}