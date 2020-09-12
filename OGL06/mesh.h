#ifndef MESH_H
#define MESH_H

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "Shader.h"
#include <QOpenGLBuffer>
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
  Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
       std::vector<Texture> &textures);
  //  void renderMesh(Shader shader);

public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

private:
  QOpenGLBuffer VAO;
  QOpenGLBuffer VBO;
  QOpenGLBuffer EBO; //渲染环境

  void setupMesh();
};
#endif // MESH_H
