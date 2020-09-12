#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QMatrix4x4>
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
  ~OpenGLWidget() override;

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
  QOpenGLBuffer vbo; //  Vertex Buffer Object (顶点缓冲对象)
  QOpenGLBuffer ebo; //  Element Buffer Object(索引缓冲对象)
  GLfloat angle;

  QVector<GLfloat> vertexData;
  QVector<GLushort> vertex_indices;
  QVector<GLfloat> colorData;
  QVector<QVector3D> cubePositions;
  QVector<QVector3D> rotate_axes;

  QMatrix4x4 matrixView;
  QMatrix4x4 matrixProjection;
};

#endif // OPENGLWIDGET_H
