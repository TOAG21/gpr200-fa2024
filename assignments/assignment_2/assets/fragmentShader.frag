#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
uniform float colorMultiplier;

void main()
{
    vec3 fadedColor = ourColor.rgb;
    fadedColor *= colorMultiplier;
    FragColor = vec4(fadedColor, 1.0f);
}