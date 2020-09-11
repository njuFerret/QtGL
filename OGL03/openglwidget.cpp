#include "openglwidget.h"
#include <QMatrix4x4>
//#include <QOpenGLShader>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), angle(0) {

  // 分别设置两个缓冲区为顶点缓冲区和索引缓冲区
  // 默认为顶点缓冲区
  vbo.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
  ibo.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));

  timer = new QTimer(this);
  timer->setInterval(1000 / 30);
  connect(timer, &QTimer::timeout, this, &OpenGLWidget::onTimerOut);
  timer->start();
}

OpenGLWidget::~OpenGLWidget() {
  makeCurrent();
  vbo->destroy();
  vbo.reset();
  ibo->destroy();
  ibo.reset();
  doneCurrent();
}

void OpenGLWidget::initializeGL() {

  initializeOpenGLFunctions();

  // shader program
  //  两种添加shader的方法：之一
  program.addShaderFromSourceFile(QOpenGLShader::Vertex, "://shader/basic.vert");
  program.addShaderFromSourceFile(QOpenGLShader::Fragment, "://shader/basic.frag");

  /**************************************************************************************
   * 两种添加shader的方法：之二
  // vertex shader
  QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  vshader->compileSourceFile("://shader/basic.vert");

  QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  fshader->compileSourceFile("://shader/basic.frag");
  program.addShader(vshader);
  program.addShader(fshader);
  **************************************************************************************/

  program.link();
  program.bind();

  // vertex buffer object
  vbo->create();

  // index buffer object
  ibo->create();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {}

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixView.setToIdentity();
  matrixView.lookAt(QVector3D(0, 0, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
  //  matrixView.lookAt(QVector3D(1, 1, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

  matrixProjection.setToIdentity();
  matrixProjection.perspective(45.0f, (GLfloat)width() / (GLfloat)height(), 0.001f,
                               100.0f); // 投影矩阵沿垂直于平面(90°)
  matrixModel.setToIdentity();
  matrixModel.rotate(angle, 0.0f, 0.0f, 1.0f); // 局部坐标沿Z洲选择angle角度
  plotRect();

  matrixModel.setToIdentity();
  matrixModel.rotate(-angle, 0.0f, 0.0f, 1.0f); // 局部坐标沿Z洲选择angle角度
  plotRect2();
}

void OpenGLWidget::plotRect() {
  // 绘制矩形方法一：指定各个顶点
  // 矩形顶点位置矩阵，需要6个顶点(1个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点)
  // 以下坐标以第一象限坐标为第0个顶点，按照逆时针依次第1、2、3个顶点
  QVector<GLfloat> positionData = {
      0.4f,  0.4f,  1.0f, // Point 0
      0.4f,  -0.4f, 1.0f, // Point 1
      -0.4f, 0.4f,  1.0f, // Point 3
      0.4f,  -0.4f, 1.0f, // Point 1
      -0.4f, -0.4f, 1.0f, // Point 2
      -0.4f, 0.4f,  1.0f  // Point 3
  };
  QVector<GLfloat> colorData = {
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f  // R: Point 1 color
  };

  QVector<GLuint> indices = {
      0, 1, 3, //
      1, 2, 3  //
  };

  int size = positionData.size() + colorData.size();

  vbo->bind();
  // vertex positions
  vbo->allocate(positionData.data(), size * sizeof(GLfloat));
  GLuint vPosition = program.attributeLocation("VertexPosition");
  program.setAttributeBuffer(vPosition, GL_FLOAT, 0, 3, 0);
  glEnableVertexAttribArray(vPosition);

  // vertex colors
  int offset = positionData.size();
  vbo->write(offset * sizeof(GLfloat), colorData.data(), colorData.size() * sizeof(GLfloat));
  GLuint vColor = program.attributeLocation("VertexColor");
  program.setAttributeBuffer(vColor, GL_FLOAT, colorData.size() * sizeof(GLfloat), 3, 0);
  glEnableVertexAttribArray(vColor);

  // perspective设置视景体，也就是空间的可视范围，
  // translate实现平移，这里是将物体向z轴负方向平移了3个单位，也就是把物体拿远了
  //（如果不拿远几个单位，是看不到物体的，因为物体是和你的眼睛处在同一个平面了）；
  // rotate物体的旋转角度，第一个参数是旋转角度，后3个参数指定旋转轴。这里指定的就是z轴正方向。

  program.setUniformValue("model", matrixModel);
  program.setUniformValue("view", matrixView);
  program.setUniformValue("projection", matrixProjection);

  // 绘制矩形，
  glDrawArrays(GL_TRIANGLES, 0, indices.size());
}

void OpenGLWidget::plotRect2() {
  // 绘制矩形方法二:顶点 + 索引
  // 以下来自qt中的OpenGL例程：cube (geometryengine.cpp)

  // 三角形可通过重复索引进行连接。如果两个相邻三角形的顶点顺序相反，
  // 则第第1个顶点和最后一个顶点的索引需要重复,
  // 如果两个相邻三角形具有相同的顶点顺序，则只需要重复最后一个顶点的索引
  // 例如 0,1,2,3，中括号内为顶点列表
  //        顶点顺序相反时，索引为 0,[0,1,2,3],3
  //        顶点顺序相同时，索引为 [0,1,2,3],3

  // 用于使用三角形绘制立方体的索引
  QVector<GLfloat> positionData = {
      -0.8f, -0.8f, 0.0f, // Point 0
      0.8f,  -0.8f, 0.0f, // Point 1
      -0.8f, 0.8f,  0.0f, // Point 2
      0.8f,  0.8f,  0.0f  // Point 3
  };

  QVector<GLfloat> colorData = {
      1.0f, 0.0f,  0.0f, // R: Point 0 color
      1.0f, 1.0f,  0.0f, // Y: Point 1 color
      0.0f, 0.0f,  1.0f, // B: Point 2 color
      0.0f, 0.84f, 0.0f  // G: Point 3 color
  };

  QVector<GLushort> indices = {
      0, 1, 2, 3, 3 //
  };

  int size = positionData.size() + colorData.size();
  vbo->bind();
  ibo->bind();
  // vertex positions
  vbo->allocate(positionData.data(), size * sizeof(GLfloat));
  GLuint vPosition = program.attributeLocation("VertexPosition");
  glEnableVertexAttribArray(vPosition);
  program.setAttributeBuffer(vPosition, GL_FLOAT, 0, 3, 0);

  // vertex colors
  int offset = positionData.size();
  vbo->write(offset * sizeof(GLfloat), colorData.data(), colorData.size() * sizeof(GLfloat));
  GLuint vColor = program.attributeLocation("VertexColor");
  glEnableVertexAttribArray(vColor);
  program.setAttributeBuffer(vColor, GL_FLOAT, colorData.size() * sizeof(GLfloat), 3, 0);

  ibo->allocate(indices.data(), indices.size() * sizeof(GLushort));

  // perspective设置视景体，也就是空间的可视范围，
  // translate实现平移，这里是将物体向z轴负方向平移了3个单位，也就是把物体拿远了
  //（如果不拿远几个单位，是看不到物体的，因为物体是和你的眼睛处在同一个平面了）；
  // rotate物体的旋转角度，第一个参数是旋转角度，后3个参数指定旋转轴。这里指定的就是z轴正方向。

  program.setUniformValue("model", matrixModel);
  program.setUniformValue("view", matrixView);
  program.setUniformValue("projection", matrixProjection);

  // 绘制矩形，
  //  glDrawArrays(GL_TRIANGLES, 0, indices.size());
  glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_SHORT, nullptr);
  //  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, nullptr);
}

void OpenGLWidget::onTimerOut() {
  angle += 1.0f;
  update();
}
