#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

class QOpenGLShaderProgram;
#include <QOpenGLBuffer>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  OpenGLWidget(QWidget *parent = nullptr);

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

protected:
  void onTimerOut();

private:
  QTimer *timer;
  QOpenGLShaderProgram *program;
  QOpenGLBuffer vbo;
  GLfloat angle;
};

#endif // OPENGLWIDGET_H
