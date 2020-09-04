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

  glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {}

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // vertices array
  QVector<GLfloat> positionData = {
      -0.8f, -0.8f, 0.0f, // Point 1
      0.8f,  -0.8f, 0.0f, // Point 2
      0.0f,  0.8f,  0.0f  // Point 3
  };
  QVector<GLfloat> colorData = {
      1.0f, 0.0f, 0.0f, // R: Point 1 color
      0.0f, 1.0f, 0.0f, // G: Point 2 color
      0.0f, 0.0f, 1.0f  // B: Point 3 color
  };
  //  QVector<GLuint> indices = {
  //      0, 1, 3, //
  //      1, 2, 3  //
  //  };

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

  QMatrix4x4 matrix;
  matrix.perspective(90.0f, (GLfloat)w / (GLfloat)h, 0.001f, 100.0f);
  matrix.translate(0.0, 0.0, -3);
  matrix.rotate(angle, 0.0f, 0.0f, 1.0f);

  program->setUniformValue("RotationMatrix", matrix);

  // draw with mode Triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGLWidget::onTimerOut() {
  angle += 1.0;
  update();
}
