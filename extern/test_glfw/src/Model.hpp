#pragma  once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"
#include <deque>
#include <bitset>

class Shader;


class Model {
public:

	enum eFlag { SAME_SCALING };

	Model(std::string const &path);
	~Model();

	glm::mat4	getTransform() const;

	void		rotate(glm::vec3 const &rotate);

	void	render() const;
	void	render(Shader &shader);

private:
	std::bitset<16>		flag_;

	std::deque<Mesh>	mesh_;
	std::string			path_;
	std::string			directory_;

	glm::mat4			transform_;

	glm::vec3			positionMax_;
	glm::vec3			positionMin_;
	glm::vec3			positionCenter_;

	float				interScaling_;
	float				sameScaling_;
	glm::vec3			scaling_;
	glm::vec3			rotation_;
	glm::vec3			position_;

	float				angle_;


    void					clean_();
	void 					loadModel_();
	void					setupScaling_();
	glm::vec3				getTranslation_();
	void					updateTransform_();

	void					processNode_(aiNode *node, const aiScene *scene);
	void					processMesh_(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture>	loadMaterialTextures_(aiMaterial *mat, aiTextureType type, Texture::eType eType);

    static bool				debug_;

	Model() = delete;
	Model(Model const &Model) = delete;
    Model &operator=(Model const &Model) = delete;
};
