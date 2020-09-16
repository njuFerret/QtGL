#ifndef MESH_H
#define MESH_H

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "Shader.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector>

struct Vertex {
  QVector3D Position;
  QVector3D Normal;
  QVector2D TexCoords;
  // glm::vec3 Tangent;
  // glm::vec3 Bitangent;
};

struct Texture {
  GLuint id;
  QString type;
  QString path;
};

class Mesh {
public:
  Mesh(const QVector<Vertex> &newVertices, const QVector<GLuint> &newIndices, QVector<Texture> &newTextures);
  void renderMesh();

public:
  QVector<Vertex> vertices;
  QVector<GLuint> indices;
  QVector<Texture> textures;

private:
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;
  QOpenGLBuffer ebo; //渲染环境

  void setupMesh();
};
#endif // MESH_H
