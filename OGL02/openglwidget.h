#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

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

private:
  QOpenGLShaderProgram *program;
  QOpenGLBuffer vbo;
};

#endif // OPENGLWIDGET_H
