#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform float time;

void main()
{
    vec2 aTexCoord = texCoord;
    //aTexCoord.x += time / 10.0f;
    //aTexCoord.y += sin(aTexCoord.x * 10 + time) / 15;
    FragColor = texture(texture1, aTexCoord);
}