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
  // shader program
  program = new QOpenGLShaderProgram(this);
  // vertex shader
  program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/shader/cube.vert");
  // Fragment shader
  program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/shader/cube.frag");

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

void OpenGLWidget::wheelEvent(QWheelEvent *event) {}

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
