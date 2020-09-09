#include "openglwidget.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QtMath>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), angle(0) {
  init_plot_data();
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
  program = new QOpenGLShaderProgram(this);
  program->addShader(vshader);
  program->addShader(fshader);

  program->link();
  program->bind();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {
  qreal aspect = qreal(w) / qreal(h ? h : 1);

  // Set near plane to 0.01, far plane to 40.0, field of view 45 degrees
  const qreal zNear = 0.01, zFar = 40.0, fov = 45.0;

  // Reset projection
  matrixProjection.setToIdentity();

  // Set perspective projection
  matrixProjection.perspective(fov, aspect, zNear, zFar);
}

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  vbo.write(offset * sizeof(GLfloat), colorData.data(),
            colorData.size() * sizeof(GLfloat));
  GLuint vColor = program->attributeLocation("VertexColor");
  program->setAttributeBuffer(vColor, GL_FLOAT,
                              colorData.size() * sizeof(GLfloat), 3, 0);
  glEnableVertexAttribArray(vColor);

  // transform
  int w = width();
  int h = height();

  // perspective设置视景体，也就是空间的可视范围，
  // translate实现平移，这里是将物体向z轴负方向平移了3个单位，也就是把物体拿远了
  // rotate物体的旋转角度，第一个参数是旋转角度，后3个参数指定旋转轴。这里指定的就是z轴正方向。

  // 对象本身沿Z轴逆时针旋转(angle<0)
  //  QMatrix4x4 matrixModel;
  //  matrixModel.rotate(-angle, 0.0f, 0.0f, 1.0f); //
  //  局部坐标沿Z洲选择angle角度

  // camera 在Y=radius处，以radius为半径顺时针旋转
  GLfloat radius = 25.0f;
  GLfloat camX = radius * qSin(angle * M_PI / 180.0f);
  GLfloat camZ = radius * qCos(angle * M_PI / 180.0f);

  // 视图变换矩阵，即可视空间矩阵，也就是camera空间矩阵
  // lookAt函数表明从camera坐标(参数1)，将camera的朝向指向目标对象(参数2),必须提供up向量(参数3)
  // 这里 camera 从相机位置处指向坐标原点，上向量是(0,1,0)即Y轴

  //  matrixView.lookAt(QVector3D(camX, radius, camZ), QVector3D(0, 0, 0),
  //  QVector3D(0, 1, 0));
  matrixView.setToIdentity();
  matrixView.lookAt(QVector3D(camX, radius, camZ), QVector3D(0, 0, 0),
                    QVector3D(0, 1, 0));

  // 使用透视变换，以窗口宽高比为比例系数，并设定远近平面
  //  QMatrix4x4 matrixProjection;
  //  matrixProjection.setToIdentity();
  //  matrixProjection.perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.001f,
  //                               100.0f); // 投影矩阵 倾斜45°

  // 绘制多个物体，在不同的位置上绘制
  for (int idx = 0; idx < cubePositions.size(); ++idx) {

    // 对象本身沿Z轴逆时针旋转(angle<0)
    QMatrix4x4 matrixModel;
    matrixModel.translate(cubePositions.at(idx));
    //    matrixModel.rotate(-angle, 0.0f,0.0f,1.0f);
    matrixModel.rotate(-angle, rotate_axes.at(idx));

    program->setUniformValue("model", matrixModel);
    program->setUniformValue("view", matrixView);
    program->setUniformValue("projection", matrixProjection);

    // 绘制矩形，
    glDrawArrays(GL_TRIANGLES, 0, positionData.size());
  }
}

void OpenGLWidget::onTimerOut() {
  angle += 2.0f;
  update();
}

void OpenGLWidget::init_plot_data() {
  // 本函数初始化所有数据，
  // 立方体顶点位置矩阵，需要36个顶点(6个面 x 每个面有2个三角形组成 x
  // 每个三角形有3个顶点)
  // 以下坐标以第一象限坐标为第0个顶点，按照逆时针依次第1、2、3个顶点
  positionData = {
      // Position(3个)   // Texture(2个)
      -0.8f, -0.8f, -0.8f, //
      0.8f,  -0.8f, -0.8f, //
      0.8f,  0.8f,  -0.8f, //
      0.8f,  0.8f,  -0.8f, //
      -0.8f, 0.8f,  -0.8f, //
      -0.8f, -0.8f, -0.8f, //

      -0.8f, -0.8f, 0.8f, //
      0.8f,  -0.8f, 0.8f, //
      0.8f,  0.8f,  0.8f, //
      0.8f,  0.8f,  0.8f, //
      -0.8f, 0.8f,  0.8f, //
      -0.8f, -0.8f, 0.8f, //

      -0.8f, 0.8f,  0.8f,  //
      -0.8f, 0.8f,  -0.8f, //
      -0.8f, -0.8f, -0.8f, //
      -0.8f, -0.8f, -0.8f, //
      -0.8f, -0.8f, 0.8f,  //
      -0.8f, 0.8f,  0.8f,  //

      0.8f,  0.8f,  0.8f,  //
      0.8f,  0.8f,  -0.8f, //
      0.8f,  -0.8f, -0.8f, //
      0.8f,  -0.8f, -0.8f, //
      0.8f,  -0.8f, 0.8f,  //
      0.8f,  0.8f,  0.8f,  //

      -0.8f, -0.8f, -0.8f, //
      0.8f,  -0.8f, -0.8f, //
      0.8f,  -0.8f, 0.8f,  //
      0.8f,  -0.8f, 0.8f,  //
      -0.8f, -0.8f, 0.8f,  //
      -0.8f, -0.8f, -0.8f, //

      -0.8f, 0.8f,  -0.8f, //
      0.8f,  0.8f,  -0.8f, //
      0.8f,  0.8f,  0.8f,  //
      0.8f,  0.8f,  0.8f,  //
      -0.8f, 0.8f,  0.8f,  //
      -0.8f, 0.8f,  -0.8f  //
  };

  colorData = {
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color
      1.0f, 0.84f, 0.0f, // R: Point 1 color

      0.0f, 0.84f, 0.0f, // R: Point 1 color
      0.0f, 0.84f, 0.0f, // R: Point 1 color
      0.0f, 0.84f, 0.0f, // R: Point 1 color
      0.0f, 0.84f, 0.0f, // R: Point 1 color
      0.0f, 0.84f, 0.0f, // R: Point 1 color
      0.0f, 0.84f, 0.0f, // R: Point 1 color

      0.0f, 0.0f,  0.84f, // R: Point 1 color
      0.0f, 0.0f,  0.84f, // R: Point 1 color
      0.0f, 0.0f,  0.84f, // R: Point 1 color
      0.0f, 0.0f,  0.84f, // R: Point 1 color
      0.0f, 0.0f,  0.84f, // R: Point 1 color
      0.0f, 0.0f,  0.84f, // R: Point 1 color

      1.0f, 0.0f,  0.0f, // R: Point 1 color
      1.0f, 0.0f,  0.0f, // R: Point 1 color
      1.0f, 0.0f,  0.0f, // R: Point 1 color
      1.0f, 0.0f,  0.0f, // R: Point 1 color
      1.0f, 0.0f,  0.0f, // R: Point 1 color
      1.0f, 0.0f,  0.0f, // R: Point 1 color

      0.0f, 0.84f, 1.0f, // R: Point 1 color
      0.0f, 0.84f, 1.0f, // R: Point 1 color
      0.0f, 0.84f, 1.0f, // R: Point 1 color
      0.0f, 0.84f, 1.0f, // R: Point 1 color
      0.0f, 0.84f, 1.0f, // R: Point 1 color
      0.0f, 0.84f, 1.0f, // R: Point 1 color

      1.0f, 0.0f,  1.0f, // R: Point 1 color
      1.0f, 0.0f,  1.0f, // R: Point 1 color
      1.0f, 0.0f,  1.0f, // R: Point 1 color
      1.0f, 0.0f,  1.0f, // R: Point 1 color
      1.0f, 0.0f,  1.0f, // R: Point 1 color
      1.0f, 0.0f,  1.0f  // R: Point 1 color
  };

  //  立方体在世界坐标下的位置
  cubePositions = {
      QVector3D(0.0f, 0.0f, 0.0f),    QVector3D(2.0f, 5.0f, -15.0f),
      QVector3D(-1.5f, -2.2f, -2.5f), QVector3D(-3.8f, -2.0f, -12.3f),
      QVector3D(2.4f, -0.4f, -3.5f),  QVector3D(-1.7f, 3.0f, 7.5f),
      QVector3D(1.3f, -2.0f, -2.5f),  QVector3D(1.5f, 2.0f, -2.5f),
      QVector3D(1.5f, 0.2f, -1.5f),   QVector3D(-1.3f, 1.0f, -1.5f)};

  for (int idx = 0; idx < cubePositions.size(); ++idx) {
    QColor c = QColor::fromRgb(QRandomGenerator::global()->generate());
    rotate_axes.append(QVector3D(c.redF(), c.greenF(), c.blueF()));
  }
}
