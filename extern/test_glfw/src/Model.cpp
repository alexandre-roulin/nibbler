#include "Model.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Model::Model(std::string const &path) :
    path_(path){
    loadModel_();
}

Model::~Model() {
    clean_();
}

void    Model::clean_() {
}

void	Model::render() const {
    for(unsigned int i = 0; i < mesh_.size(); i++)
        mesh_[i].render();
}
void	Model::render(Shader &shader) {
    for(unsigned int i = 0; i < mesh_.size(); i++)
        mesh_[i].render(shader);
}

void 					Model::loadModel_() {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path_, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    //aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return ;
    }
#ifdef __APPLE__
    directory_ = path_.substr(0, path_.find_last_of('/'));
#else
    directory_ = path_.substr(0, path_.find_last_of("\\"));
#endif
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

        /*
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
           vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]) { // Each 1st column = a texture (Jusqu'a 8)
            vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.uv = glm::vec2(0.0f, 0.0f);
        }
		vertices.push_back(vertex);*/

		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
			vertex.uv = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
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
    return textures;
}

unsigned int Texture::TextureFromFile(const char *path, const std::string &directory)
{
    std::string         filename;
    unsigned char       *data;
    unsigned int        textureID;
    GLenum              format;
    int                 width, height, nrComponents;

    filename = std::string(path);
#ifdef __APPLE__
    filename = directory + '/' + filename;
#else
    filename = directory + "\\" + filename;
#endif
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
        std::cout << "Texture : " << path << std::endl;
    }
    else
        std::cerr << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);

    return (textureID);
}

bool        Model::debug_ = true;