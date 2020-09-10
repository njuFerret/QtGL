// 限定符in/out定义变量的类型为输入/输出变量。
// 顶点着色器

// 本段着色器代码定义了2个输入变量，分别是vec3类型的顶点位置（VertexPosition）和颜色（VertexColor），
// 以及1个vec3类型的输出变量，该变量用于输出顶点的颜色（Color），
// 输出变量会被传递到下一个着色器中，作为输入变量

#version 440

//in vec3 VertexPosition;
//in vec3 VertexColor;

//out vec3 Color;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;


//void main()
//{
//    // 用输入变量初始化输出变量的值
//    Color = VertexColor;
//    // 初始化一个预定义的vec4类型的变量gl_Position，设定该顶点的位置
//    gl_Position = projection * view * model *vec4( VertexPosition, 1.0 );

//}

uniform mat4 mvp_matrix;
// 两个属性，位置和纹理坐标
attribute vec4 a_position;
attribute vec2 a_texcoord;
// 输出至片段着色器问题坐标
varying vec2 v_texcoord;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]

