GLSL 三种变量类型（uniform，attribute和varying）

1.uniform变量

uniform变量是外部程序传递给（vertex和fragment）shader的变量。因此它是application通过函数glUniform**()函数赋值的。在（vertex和fragment）shader程序内部，uniform变量就像是C语言里面的常量(const)，它不能被shader程序修改。（shader只能用，不能改）

如果uniform变量在vertex和fragment两者之间声明方式完全一样，则它可以在vertex和fragment共享使用。（相当于一个被vertex和fragment shader共享的全局变量）

uniform变量一般用来表示：变换矩阵，材质，光照参数和颜色等信息。

以下是例子：
```glsl
uniform mat4 viewProjMatrix; //投影+视图矩阵
uniform mat4 viewMatrix;        //视图矩阵
uniform vec3 lightPosition;     //光源位置
uniform float lumaThreshold;
uniform float chromaThreshold;
uniform sampler2D SamplerY;
uniform sampler2D SamplerUV;
uniform mat3 colorConversionMatrix;
```

2.attribute变量

attribute变量是只能在vertex shader中使用的变量。（它不能在fragment shader中声明attribute变量，也不能被fragment shader中使用）

一般用attribute变量来表示一些顶点的数据，如：顶点坐标，法线，纹理坐标，顶点颜色等。

在application中，一般用函数glBindAttribLocation()来绑定每个attribute变量的位置，然后用函数glVertexAttribPointer()为每个attribute变量赋值。

以下是例子：
```glsl
attribute vec4 position;
attribute vec2 texCoord;
uniform float preferredRotation;
varying vec2 texCoordVarying;

void main()
{
    mat4 rotationMatrix = mat4( cos(preferredRotation), -sin(preferredRotation), 0.0, 0.0,
                                sin(preferredRotation),  cos(preferredRotation), 0.0, 0.0,
                                0.0,     0.0, 1.0, 0.0,
                                0.0,     0.0, 0.0, 1.0);
    gl_Position = position * rotationMatrix;
    texCoordVarying = texCoord;

}
```

3.varying变量

varying变量是vertex和fragment shader之间做数据传递用的。一般vertex shader修改varying变量的值，然后fragment shader使用该varying变量的值。因此varying变量在vertex和fragment shader二者之间的声明必须是一致的。application不能使用此变量。

以下是例子：



```glsl
// Vertex shader  
attribute vec4 position;
attribute vec2 texCoord;
uniform float preferredRotation;
varying vec2 texCoordVarying;

void main()
{
    mat4 rotationMatrix = mat4( cos(preferredRotation), -sin(preferredRotation), 0.0, 0.0,
                                sin(preferredRotation),  cos(preferredRotation), 0.0, 0.0,
                                0.0,     0.0, 1.0, 0.0,
                                0.0,     0.0, 0.0, 1.0);
    gl_Position = position * rotationMatrix;
    texCoordVarying = texCoord;

}
```

// Fragment shader
```glsl
varying highp vec2 texCoordVarying;  // Varying in fragment shader
precision mediump float;
uniform float lumaThreshold;
uniform float chromaThreshold;
uniform sampler2D SamplerY;
uniform sampler2D SamplerUV;
uniform mat3 colorConversionMatrix;

void main()
{
  mediump vec3 yuv;
  lowp vec3 rgb;
  
  // Subtract constants to map the video range start at 0
  yuv.x = (texture2D(SamplerY, texCoordVarying).r - (16.0/255.0))* lumaThreshold;
  yuv.yz = (texture2D(SamplerUV, texCoordVarying).rg - vec2(0.5, 0.5))* chromaThreshold;
  rgb = colorConversionMatrix * yuv;
  
  gl_FragColor = vec4(rgb,1);  
}
```

参考： https://www.jianshu.com/p/eed3ebdad4fb