#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLFunctions>
//#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QTimer>
#include <QVector2D>
#include <QVector3D>

class GeometryEngine;
class QOpenGLShaderProgram;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget() override;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

protected:
  void onTimerOut();
  //  void init_plot_data();

  void initShader();
  void initTextures();

private:
  QTimer *timer;
  QOpenGLShaderProgram *program;
  //  QOpenGLBuffer vbo;
  //  GLfloat angle;

  //  QVector<GLfloat> positionData;
  //  QVector<GLfloat> colorData;
  //  QVector<QVector3D> cubePositions;
  //  QVector<QVector3D> rotate_axes;

  GeometryEngine *geometries = nullptr;
  QOpenGLTexture *texture = nullptr;

  QMatrix4x4 projection;

  QVector2D mousePressPosition;
  QVector3D rotationAxis;
  qreal angularSpeed = 0;
  QQuaternion rotation;
};

#endif // OPENGLWIDGET_H
