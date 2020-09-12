// 限定符in/out定义变量的类型为输入/输出变量。
// 顶点着色器

// 本段着色器代码定义了2个输入变量，分别是vec3类型的顶点位置（VertexPosition）和颜色（VertexColor），
// 以及1个vec3类型的输出变量，该变量用于输出顶点的颜色（Color），
// 输出变量会被传递到下一个着色器中，作为输入变量

#version 440

in vec3 VertexPosition;
in vec3 VertexColor;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    // 用输入变量初始化输出变量的值
    Color = VertexColor;
    // 初始化一个预定义的vec4类型的变量gl_Position，设定该顶点的位置
    gl_Position = projection * view * model *vec4( VertexPosition, 1.0 );

}

//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;

//out vec2 TexCoords;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//void main()
//{
//    TexCoords = aTexCoords;
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//}
