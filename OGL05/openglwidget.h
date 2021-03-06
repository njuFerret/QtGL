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

  GeometryEngine *geometries = nullptr;
  QOpenGLTexture *texture = nullptr;

  QMatrix4x4 projection;
  //  QMatrix4x4 view;
  //  GLfloat zoomScale = 1.0f;

  QVector2D mousePressPosition;
  QVector3D rotationAxis;
  qreal angularSpeed = 0;
  QQuaternion rotation;

  bool beginDrag = false;

  // QWidget interface
protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
};

#endif // OPENGLWIDGET_H
