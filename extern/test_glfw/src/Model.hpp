#pragma  once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"
#include <deque>
#include <bitset>
#include <glm/gtc/matrix_transform.hpp>

class Shader;


class Model {
public:

	enum eFlag { SAME_SCALING };

	Model();
	Model(std::string const &path);
	~Model();

	void	setModel(std::string const &path);

	void	render() const;
	void	render(Shader &shader);

	void		resetTransform();
	glm::mat4	getTransform() const;

	void	translate(glm::vec3 const &axis, float deltaTime = 1.f);
	void	rotate(glm::vec3 const &axis, float angle, float deltaTime = 1.f);
	void	scale(glm::vec3 const &axis, float deltaTime = 1.f);
	void	scale(float, float deltaTime = 1.f);


private:
	std::bitset<16>		flag_;

	std::deque<Mesh>	mesh_;
	std::string			path_;
	std::string			directory_;

	glm::mat4			transform_;
	glm::mat4			rotate_;

	glm::vec3			positionMax_;
	glm::vec3			positionMin_;
	glm::vec3			positionCenter_;

	float				interScaling_;
	float				sameScaling_;
	glm::vec3			scaling_;
	glm::vec3			rotation_;
	glm::vec3			position_;

	float				speed_;


    void					clean_();
	void 					loadModel_();
	void					setupScaling_();
	void					updateTransform_();

	void					processNode_(aiNode *node, const aiScene *scene);
	void					processMesh_(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture>	loadMaterialTextures_(aiMaterial *mat, aiTextureType type, Texture::eType eType);

    static bool				debug_;

	Model(Model const &Model) = delete;
    Model &operator=(Model const &Model) = delete;
};
