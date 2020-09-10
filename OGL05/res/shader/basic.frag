#version 440
// 片段着色器

//in vec3 Color;          // 输入变量为顶点着色器输出变量
//out vec4 FragColor;     // 输出片段颜色

//void main()
//{
//    // 第四维设定透明度，如果开启blend模式
//    FragColor = vec4( Color, 1.0);
////    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//}

// 纹理
uniform sampler2D texture;
// 由顶点着色器获得的纹理坐标
varying vec2 v_texcoord;


void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord);
}

