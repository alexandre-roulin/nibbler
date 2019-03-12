#pragma once

#include <exception>
#include <glm/glm.hpp>
#include <boost/filesystem.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>

#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "Glfw.hpp"
#include "Shader.hpp"
#include "Glfw.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "ActModel.hpp"
#include "MutantGrid.tpp"
#include "Particle.hpp"
#include "Material.hpp"
#include "Light.hpp"

#define FLAG_LINE 2

class DisplayGlfw : public Glfw, public IDisplay {
public:
    class GlfwConstructorException : public std::exception {
    public:
        GlfwConstructorException() noexcept;
		explicit GlfwConstructorException(std::string const &s) noexcept;
		const char *what() const noexcept override;
        ~GlfwConstructorException() noexcept override;
	private:
        GlfwConstructorException &
        operator=(GlfwConstructorException const &rhs) noexcept;
        std::string error_;
    };

	DisplayGlfw() = delete;
    DisplayGlfw(int width, int height, char const *windowName);
	~DisplayGlfw() override;
	DisplayGlfw &operator=(DisplayGlfw const &rhs) = delete;
	DisplayGlfw(DisplayGlfw const &src) = delete;

	void render() override;
    void update() override;
	bool        exit() const override;
	eDirection getDirection() const override;
	void		drawGrid(MutantGrid< eSprite > const &grid) override;
	void		setBackground(MutantGrid< eSprite > const &grid) override;
	void		registerCallbackAction(std::function<void(eAction)>) override;


private:
	std::bitset<16>					flag_;
	boost::filesystem::path			pathRoot_;
    eDirection          			direction_;
	Vector2D<int> const 			winTileSize_;

	MutantGrid< eSprite >			tileBackground_;
    MutantGrid< ActModel >			background_;
	MutantGrid< eSprite >			tileGrid_;
	MutantGrid< ActModel >			grid_;

	float							deltaTime_;

	eSprite 						yourSnakeSprite;
	int 							yourSnakeX;
	int 							yourSnakeY;
	Shader							shader_;
	Shader							shaderMultiple_;
	Model							modelGrass_;
	Model							modelWall_;
	Model							appleModel_;
	Model							modelSphere_;
	Model							modelHead_;
	Particle						*particuleBackground_;
	Particle						*particuleBackgroundOutline_;
	Light							light_;
	std::vector<Camera>				camera_;
	int								indexActiveCamera_;
    ActModel						actBlock_;

	glm::mat4						projection_;
	glm::mat4						view_;
	glm::mat4						model_;

	std::function<void(eAction)>	callback_;


	void            	    error_(std::string const &s = std::string("Error"));
    void            	    clean_();
	void					activeNextCamera_();
	Camera					&getActiveCamera_();

	void					drawGridCase_(eSprite sprite, int x, int y);
	void					drawGridCaseBody_(int x, int y);

	void					renderLine_(ActModel const &model);

	void					drawHelpLineSnake_();


	std::map< eSprite, Material >		materialMap_;
	void constructMaterialMap_();
};

extern "C" {
IDisplay *newInstance(int width,
                     int height,
                     char const *windowName);
void deleteInstance(IDisplay *display);
}