#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget() override;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void plotRect();
  void plotRect2();

protected:
  void onTimerOut();

private:
  QTimer *timer;
  QOpenGLShaderProgram program;

  QScopedPointer<QOpenGLBuffer> vbo;
  QScopedPointer<QOpenGLBuffer> ibo;

  GLfloat angle;

  QMatrix4x4 matrixProjection;
  QMatrix4x4 matrixView;
  QMatrix4x4 matrixModel;
};

#endif // OPENGLWIDGET_H
