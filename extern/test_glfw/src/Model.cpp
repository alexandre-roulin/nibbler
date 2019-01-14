#include "Model.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include "DisplayGlfw.hpp"


Model::Model() :
		positionMax_(0.f),
		positionMin_(0.f),
		positionCenter_(0.f),
		interScaling_(1.f) {
}

Model::Model(std::string const &path) :
		positionMax_(0.f),
		positionMin_(0.f),
		positionCenter_(0.f),
		interScaling_(1.f) {
	setModel(path);
}

void	Model::setModel(std::string const &path) {
	clean_();
	path_ = path;
	loadModel_();
	setupScaling_();
}

Model::~Model() {
    clean_();
}

void    Model::clean_() {
	mesh_.clear();
}

float	Model::getInterScaling() const {
	return (interScaling_);
}
glm::vec3	Model::getPositionCenter() const {
	return (positionCenter_);
}

void	Model::render() const {
    for(unsigned int i = 0; i < mesh_.size(); i++)
        mesh_[i].render();
}
void	Model::render(Shader &shader) const {
    for(unsigned int i = 0; i < mesh_.size(); i++)
        mesh_[i].render(shader);
}

void 					Model::loadModel_() {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path_, aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return ;
    }
	std::cout << "[path_] : " << path_ << std::endl;
    directory_ = path_.substr(0, path_.find_last_of(DISPLAY_GLFW_SLASH));
	std::cout << "[directory_] : " << directory_ << std::endl;
    processNode_(scene->mRootNode, scene);

}

void					Model::processNode_(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)  {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh_(mesh, scene);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode_(node->mChildren[i], scene);
    }

    //for(unsigned int i = 0; i < scene->mMeshes.size(); i++) {
	//    processMesh_(mesh, scene);
	//}
}


void					Model::processMesh_(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;
    std::vector<Texture>        textures;

	std::cout << "node NumMeshes : " << mesh->mNumVertices << std::endl;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)  {
        Vertex vertex;

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0])// Each 1st column = a texture (Jusqu'a 8)
            vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.uv = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

        if (!i)
        	positionMax_ = positionMin_ = vertex.position;
        else {
        	if (vertex.position.x > positionMax_.x)
        		positionMax_.x = vertex.position.x;
			if (vertex.position.y > positionMax_.y)
				positionMax_.y = vertex.position.y;
			if (vertex.position.z > positionMax_.z)
				positionMax_.z = vertex.position.z;

			if (vertex.position.x < positionMin_.x)
				positionMin_.x = vertex.position.x;
			if (vertex.position.y < positionMin_.y)
				positionMin_.y = vertex.position.y;
			if (vertex.position.z < positionMin_.z)
				positionMin_.z = vertex.position.z;
        }
    }


    for(unsigned int i = 0; i < mesh->mNumFaces; i++)  {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) //Always 3 : I make Triangular Flag Option
            indices.push_back(face.mIndices[j]);
    }

    // Traitement des matériaux
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures_(material, aiTextureType_DIFFUSE, Texture::eType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures_(material, aiTextureType_SPECULAR, Texture::eType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    mesh_.emplace_back(vertices, indices, textures);
}

void			Model::setupScaling_()  {
	glm::vec3	diff;
	float		scaling;

	diff = glm::abs(positionMin_ - positionMax_);
	if (diff.x > diff.y && diff.x > diff.z)
		scaling = 1.f / diff.x;
	else if (diff.y > diff.x && diff.y > diff.z)
		scaling = 1.f / diff.y;
	else
		scaling = 1.f / diff.z;
	positionMin_ = -positionMin_;
	diff = positionMax_ - positionMin_;
	diff.x = positionMax_.x - positionMin_.x;
	diff.y = positionMax_.y - positionMin_.y;
	diff.z = positionMax_.z - positionMin_.z;
	interScaling_ = scaling;
	diff = diff * 0.5f;
	positionCenter_ = diff;
}

std::vector<Texture>    Model::loadMaterialTextures_(aiMaterial *mat, aiTextureType type, Texture::eType eType)
{
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)  {
        aiString str;
        mat->GetTexture(type, i, &str);

        Texture texture;
        texture.id = Texture::TextureFromFile(str.C_Str(), directory_);
        texture.type = eType;
        texture.path = std::string(str.C_Str());
        textures.push_back(texture);
    }
    return (textures);
}

unsigned int Texture::TextureFromFile(const char *path, const std::string &directory)
{
    std::string         filename;
    unsigned char       *data;
    unsigned int        textureID;
    GLenum              format;
    int                 width, height, nrComponents;

    filename = std::string(path);

    filename = directory + DISPLAY_GLFW_SLASH + filename;

    glGenTextures(1, &textureID);

    data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data && nrComponents == 2) {
        std::cerr << "Texture cannot be GREY_ALPHA at path: " << path << std::endl;
    }
    else if (data) {

        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = 0;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Texture : " << filename << std::endl;
    }
    else
        std::cerr << "Texture failed to load at path: " << filename << std::endl;
    stbi_image_free(data);

    return (textureID);
}

bool        Model::debug_ = true;