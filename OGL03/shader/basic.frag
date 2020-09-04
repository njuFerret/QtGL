#version 440
// 片段着色器

in vec3 Color;          // 输入变量为顶点着色器输出变量
out vec4 FragColor;     // 输出片段颜色

void main()
{
    // 第四维设定透明度，如果开启blend模式
    FragColor = vec4( Color, 1.0);
}
