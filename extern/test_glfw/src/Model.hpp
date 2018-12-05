#pragma  once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"

class Shader;


class Model {
public:

	Model(std::string const &path);
	~Model();

	void	render() const;
	void	render(Shader &shader);

	std::vector<Mesh *>	mesh_;
	std::string			path_;
	std::string			directory_;

private:

    void					clean_();
	void 					loadModel_();

	void					processNode_(aiNode *node, const aiScene *scene);
	Mesh					*processMesh_(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture>	loadMaterialTextures_(aiMaterial *mat, aiTextureType type, Texture::eType eType);

    static bool				debug_;

	Model() = delete;
	Model(Model const &Model) = delete;
    Model &operator=(Model const &Model) = delete;
};