#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector3D>

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
  void init_plot_data();

private:
  QTimer *timer;
  QOpenGLShaderProgram *program;
  QOpenGLBuffer vbo;
  GLfloat angle;

  QVector<GLfloat> positionData;
  QVector<GLfloat> colorData;
  QVector<QVector3D> cubePositions;
  QVector<QVector3D> rotate_axes;
};

#endif // OPENGLWIDGET_H
