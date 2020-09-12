#include "openglwidget.h"
#include "geometryengine.h"

#include <QMouseEvent>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QtMath>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  //  init_plot_data();
  timer = new QTimer(this);
  timer->setInterval(1000 / 30);
  connect(timer, &QTimer::timeout, this, &OpenGLWidget::onTimerOut);
}

OpenGLWidget::~OpenGLWidget() {
  makeCurrent();
  //    delete program;
  delete geometries;
  delete texture;
  doneCurrent();
}

void OpenGLWidget::initShader() {
  QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  vshader->compileSourceFile(":/res/shader/basic.vert");
  // fragment shader
  QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  fshader->compileSourceFile(":/res/shader/basic.frag");

  // shader program
  program = new QOpenGLShaderProgram(this);
  program->addShader(vshader);
  program->addShader(fshader);

  program->link();
  program->bind();
}

void OpenGLWidget::initializeGL() {

  initializeOpenGLFunctions();

  // vertex shader
  initShader();
  initTextures();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // Enable back face culling
  glEnable(GL_CULL_FACE);

  geometries = new GeometryEngine;

  timer->start();
}

void OpenGLWidget::initTextures() {
  // Load cube.png image
  texture = new QOpenGLTexture(QImage(":/tex_cube.png").mirrored());

  // Set nearest filtering mode for texture minification
  texture->setMinificationFilter(QOpenGLTexture::Nearest);

  // Set bilinear filtering mode for texture magnification
  texture->setMagnificationFilter(QOpenGLTexture::Linear);

  // Wrap texture coordinates by repeating
  // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
  texture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    // Save mouse press position
    mousePressPosition = QVector2D(event->localPos());
    beginDrag = true;
  }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && beginDrag) {
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(event->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
    beginDrag = false;
  }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {

  //  if (event->angleDelta().y() < 0) {
  //    zoomScale /= 1.1;
  //  } else {
  //    zoomScale *= 1.1;
  //  }
  //  GLint viewport[4];
  //  glGetIntegerv(GL_VIEWPORT, viewport);
  //  resizeGL(viewport[2], viewport[3]);
  //  update();
}

void OpenGLWidget::resizeGL(int w, int h) {
  qreal aspect = qreal(w) / qreal(h ? h : 1);

  // Set near plane to 0.01, far plane to 40.0, field of view 45 degrees
  const qreal zNear = 0.01, zFar = 40.0, fov = 45.0;

  // Reset projection
  projection.setToIdentity();

  // Set perspective projection
  projection.perspective(fov, aspect, zNear, zFar);
}

void OpenGLWidget::paintGL() {

  // Clear color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  texture->bind();

  //! [6]
  // Calculate model view transformation
  QMatrix4x4 matrix;
  matrix.translate(0.0, 0.0, -5.0);
  matrix.rotate(rotation);

  //  qDebug() << view;
  // Set modelview-projection matrix
  program->setUniformValue("mvp_matrix", projection * matrix);
  //! [6]

  // Use texture unit 0 which contains cube.png
  program->setUniformValue("texture", 0);

  // Draw cube geometry
  geometries->renderCubes(program);
}

void OpenGLWidget::onTimerOut() {
  //  angle += 2.0f;
  //  update();

  // Decrease angular speed (friction)
  angularSpeed *= 0.99;

  // Stop rotation when speed goes below threshold
  if (angularSpeed < 0.01) {
    angularSpeed = 0.0;
  } else {
    // Update rotation
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

    // Request an update
    update();
  }
}

// void OpenGLWidget::init_plot_data() {
//  // 本函数初始化所有数据，
//  // 立方体顶点位置矩阵，需要36个顶点(6个面 x 每个面有2个三角形组成 x
//  // 每个三角形有3个顶点)
//  // 以下坐标以第一象限坐标为第0个顶点，按照逆时针依次第1、2、3个顶点
//  positionData = {
//      // Position(3个)   // Texture(2个)
//      -0.8f, -0.8f, -0.8f, //
//      0.8f,  -0.8f, -0.8f, //
//      0.8f,  0.8f,  -0.8f, //
//      0.8f,  0.8f,  -0.8f, //
//      -0.8f, 0.8f,  -0.8f, //
//      -0.8f, -0.8f, -0.8f, //

//      -0.8f, -0.8f, 0.8f, //
//      0.8f,  -0.8f, 0.8f, //
//      0.8f,  0.8f,  0.8f, //
//      0.8f,  0.8f,  0.8f, //
//      -0.8f, 0.8f,  0.8f, //
//      -0.8f, -0.8f, 0.8f, //

//      -0.8f, 0.8f,  0.8f,  //
//      -0.8f, 0.8f,  -0.8f, //
//      -0.8f, -0.8f, -0.8f, //
//      -0.8f, -0.8f, -0.8f, //
//      -0.8f, -0.8f, 0.8f,  //
//      -0.8f, 0.8f,  0.8f,  //

//      0.8f,  0.8f,  0.8f,  //
//      0.8f,  0.8f,  -0.8f, //
//      0.8f,  -0.8f, -0.8f, //
//      0.8f,  -0.8f, -0.8f, //
//      0.8f,  -0.8f, 0.8f,  //
//      0.8f,  0.8f,  0.8f,  //

//      -0.8f, -0.8f, -0.8f, //
//      0.8f,  -0.8f, -0.8f, //
//      0.8f,  -0.8f, 0.8f,  //
//      0.8f,  -0.8f, 0.8f,  //
//      -0.8f, -0.8f, 0.8f,  //
//      -0.8f, -0.8f, -0.8f, //

//      -0.8f, 0.8f,  -0.8f, //
//      0.8f,  0.8f,  -0.8f, //
//      0.8f,  0.8f,  0.8f,  //
//      0.8f,  0.8f,  0.8f,  //
//      -0.8f, 0.8f,  0.8f,  //
//      -0.8f, 0.8f,  -0.8f  //
//  };

//  colorData = {
//      1.0f, 0.84f, 0.0f, // R: Point 1 color
//      1.0f, 0.84f, 0.0f, // R: Point 1 color
//      1.0f, 0.84f, 0.0f, // R: Point 1 color
//      1.0f, 0.84f, 0.0f, // R: Point 1 color
//      1.0f, 0.84f, 0.0f, // R: Point 1 color
//      1.0f, 0.84f, 0.0f, // R: Point 1 color

//      0.0f, 0.84f, 0.0f, // R: Point 1 color
//      0.0f, 0.84f, 0.0f, // R: Point 1 color
//      0.0f, 0.84f, 0.0f, // R: Point 1 color
//      0.0f, 0.84f, 0.0f, // R: Point 1 color
//      0.0f, 0.84f, 0.0f, // R: Point 1 color
//      0.0f, 0.84f, 0.0f, // R: Point 1 color

//      0.0f, 0.0f,  0.84f, // R: Point 1 color
//      0.0f, 0.0f,  0.84f, // R: Point 1 color
//      0.0f, 0.0f,  0.84f, // R: Point 1 color
//      0.0f, 0.0f,  0.84f, // R: Point 1 color
//      0.0f, 0.0f,  0.84f, // R: Point 1 color
//      0.0f, 0.0f,  0.84f, // R: Point 1 color

//      1.0f, 0.0f,  0.0f, // R: Point 1 color
//      1.0f, 0.0f,  0.0f, // R: Point 1 color
//      1.0f, 0.0f,  0.0f, // R: Point 1 color
//      1.0f, 0.0f,  0.0f, // R: Point 1 color
//      1.0f, 0.0f,  0.0f, // R: Point 1 color
//      1.0f, 0.0f,  0.0f, // R: Point 1 color

//      0.0f, 0.84f, 1.0f, // R: Point 1 color
//      0.0f, 0.84f, 1.0f, // R: Point 1 color
//      0.0f, 0.84f, 1.0f, // R: Point 1 color
//      0.0f, 0.84f, 1.0f, // R: Point 1 color
//      0.0f, 0.84f, 1.0f, // R: Point 1 color
//      0.0f, 0.84f, 1.0f, // R: Point 1 color

//      1.0f, 0.0f,  1.0f, // R: Point 1 color
//      1.0f, 0.0f,  1.0f, // R: Point 1 color
//      1.0f, 0.0f,  1.0f, // R: Point 1 color
//      1.0f, 0.0f,  1.0f, // R: Point 1 color
//      1.0f, 0.0f,  1.0f, // R: Point 1 color
//      1.0f, 0.0f,  1.0f  // R: Point 1 color
//  };

//  //  立方体在世界坐标下的位置
//  cubePositions = {QVector3D(0.0f, 0.0f, 0.0f),    QVector3D(2.0f, 5.0f, -15.0f),
//                   QVector3D(-1.5f, -2.2f, -2.5f), QVector3D(-3.8f, -2.0f, -12.3f),
//                   QVector3D(2.4f, -0.4f, -3.5f),  QVector3D(-1.7f, 3.0f, 7.5f),
//                   QVector3D(1.3f, -2.0f, -2.5f),  QVector3D(1.5f, 2.0f, -2.5f),
//                   QVector3D(1.5f, 0.2f, -1.5f),   QVector3D(-1.3f, 1.0f, -1.5f)};

//  for (int idx = 0; idx < cubePositions.size(); ++idx) {
//    QColor c = QColor::fromRgb(QRandomGenerator::global()->generate());
//    rotate_axes.append(QVector3D(c.redF(), c.greenF(), c.blueF()));
//  }
//}
