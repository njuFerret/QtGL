#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "Mesh.h"

class Model {
public:
  Model(const char *path, bool gamma = false);

  //  void Draw(Shader shader);

private:
  //  std::vector<Mesh> meshes;
  //  std::string directory;
  //  std::vector<Texture> textures_loaded;
  bool gammaCorrection;

  //  void loadModel(std::string path);
  //  void processNode(aiNode *node, const aiScene *scene);
  //  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  //  std::vector<Texture> loadMaterialtextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H
