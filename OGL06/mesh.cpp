#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
           std::vector<Texture> &textures) {
  (this->vertices).insert((this->vertices).end(), vertices.begin(), vertices.end());
  (this->indices).insert((this->indices).end(), indices.begin(), indices.end());
  (this->textures).insert((this->textures).end(), textures.begin(), textures.end());

  setupMesh();
}

void Mesh::setupMesh() {
  /*
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

//顶点位置
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

//顶点法线
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

//纹理坐标
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

//恢复默认的VAO环境
glBindVertexArray(0);
*/
}
/*
void Mesh::renderMesh(Shader shader) {

   GLuint diffuseNr = 1;
   GLuint specularNr = 1;
   for (GLuint i = 0; i < textures.size(); ++i) {
     glActiveTexture(GL_TEXTURE0 + i);

     std::string number;
     std::string name = textures[i].type;
     if (name == "texture_diffuse")
       number = std::to_string(diffuseNr++);
     else if (name == "texture_specular")
       number = std::to_string(specularNr++);

     shader.setFloat(("material." + name + number).c_str(), i);
     glBindTexture(GL_TEXTURE_2D, textures[i].id);
   }

   //渲染
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   glActiveTexture(GL_TEXTURE0);

}*/
