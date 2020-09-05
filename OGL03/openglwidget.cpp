#include "openglwidget.h"
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), angle(0) {
  timer = new QTimer(this);
  timer->setInterval(1000 / 30);
  connect(timer, &QTimer::timeout, this, &OpenGLWidget::onTimerOut);
  timer->start();
}

void OpenGLWidget::initializeGL() {

  initializeOpenGLFunctions();

  // vertex shader
  QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  vshader->compileSourceFile("://shader/basic.vert");
  // fragment shader
  QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  fshader->compileSourceFile("://shader/basic.frag");

  // shader program
  program = new QOpenGLShaderProgram;
  program->addShader(vshader);
  program->addShader(fshader);

  program->link();
  program->bind();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {}

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // 矩形顶点位置矩阵，需要6个顶点(1个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点)
  // 以下坐标以第一象限坐标为第0个顶点，按照逆时针依次第1、2、3个顶点
  QVector<GLfloat> positionData = {
      0.8f,  0.8f,  0.0f, // Point 0
      0.8f,  -0.8f, 0.0f, // Point 1
      -0.8f, 0.8f,  0.0f, // Point 3
      0.8f,  -0.8f, 0.0f, // Point 1
      -0.8f, -0.8f, 0.0f, // Point 2
      -0.8f, 0.8f,  0.0f  // Point 3
  };
  QVector<GLfloat> colorData = {
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f  // R: Point 1 color
  };
  //      1.0f, 0.0f, 0.0f, // R: Point 1 color
  //      0.0f, 1.0f, 0.0f, // G: Point 2 color
  //      0.0f, 0.0f, 1.0f, // B: Point 3 color
  //      1.0f, 0.0f, 0.0f, // R: Point 1 color
  //      0.0f, 1.0f, 0.0f, // G: Point 2 color
  //      0.0f, 0.0f, 1.0f  // B: Point 3 color
  QVector<GLuint> indices = {
      0, 1, 3, //
      1, 2, 3  //
  };

  // vertex buffer object
  vbo.create();
  vbo.bind();
  int size = positionData.size() + colorData.size();

  // vertex positions
  vbo.allocate(positionData.data(), size * sizeof(GLfloat));
  GLuint vPosition = program->attributeLocation("VertexPosition");
  program->setAttributeBuffer(vPosition, GL_FLOAT, 0, 3, 0);
  glEnableVertexAttribArray(vPosition);

  // vertex colors
  int offset = positionData.size();
  vbo.write(offset * sizeof(GLfloat), colorData.data(), colorData.size() * sizeof(GLfloat));
  GLuint vColor = program->attributeLocation("VertexColor");
  program->setAttributeBuffer(vColor, GL_FLOAT, colorData.size() * sizeof(GLfloat), 3, 0);
  glEnableVertexAttribArray(vColor);

  // transform
  int w = width();
  int h = height();

  // perspective设置视景体，也就是空间的可视范围，
  // translate实现平移，这里是将物体向z轴负方向平移了3个单位，也就是把物体拿远了
  //（如果不拿远几个单位，是看不到物体的，因为物体是和你的眼睛处在同一个平面了）；
  // rotate物体的旋转角度，第一个参数是旋转角度，后3个参数指定旋转轴。这里指定的就是z轴正方向。

  QMatrix4x4 matrixModel;
  matrixModel.rotate(angle, 0.0f, 0.0f, 1.0f); // 局部坐标沿Z洲选择angle角度

  QMatrix4x4 matrixView;
  matrixView.translate(0.0, 0.0, -3); // 视图矩阵沿-Z轴平移3个单位

  QMatrix4x4 matrixProjection;
  matrixProjection.perspective(90.0f, (GLfloat)w / (GLfloat)h, 0.001f, 100.0f); // 投影矩阵沿垂直于平面(90°)

  program->setUniformValue("model", matrixModel);
  program->setUniformValue("view", matrixView);
  program->setUniformValue("projection", matrixProjection);

  // 绘制矩形，
  glDrawArrays(GL_TRIANGLES, 0, indices.size());
}

void OpenGLWidget::onTimerOut() {
  angle += 1.0f;
  update();
}
