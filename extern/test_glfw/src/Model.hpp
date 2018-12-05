#pragma  once

#include <vector>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh;
class Shader;

class Model {
public:

	Model(std::string const &path);
	~Model();

	void	render() const;
	void	render(Shader &shader);

private:
	std::vector<Mesh>	meshe_;
	std::string			path_;

    void					clean_();
	void 					loadModel_();

	//void					processNode_(aiNode *node, const aiScene *scene);
	//Mesh					processMesh_(aiMesh *mesh, const aiScene *scene);
	//std::vector<Texture>	loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    static bool				debug_;

	Model() = delete;
	Model(Model const &Model) = delete;
    Model &operator=(Model const &Model) = delete;
};