#version 330 core
uniform vec4 aColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(aColor);
}
