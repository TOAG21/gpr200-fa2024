#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
  
out vec4 ourColor;
uniform float uTime;

void main()
{
    vec3 pos = aPos;
    gl_Position = vec4(pos, 1.0);
    ourColor = aColor;
}