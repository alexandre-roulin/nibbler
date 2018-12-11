#pragma once

#include "Model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <bitset>

class ActModel {
public:

	enum eFlag { SAME_SCALING };

	ActModel();
	ActModel(Model const *model);
	~ActModel();
	ActModel(ActModel const &src);
	ActModel &operator=(ActModel const &src);

	Model const		*getModel() const;
	void			assign(Model const *model);
	void			resetTransform();
	glm::mat4		getTransform() const;

	void	translate(glm::vec3 const &axis, float deltaTime = 1.f);
	void	rotate(glm::vec3 const &axis, float angle, float deltaTime = 1.f);
	void	scale(glm::vec3 const &axis, float deltaTime = 1.f);
	void	scale(float, float deltaTime = 1.f);

private:
	Model const			*model_;

	std::bitset<16>		flag_;
	glm::mat4			transform_;
	glm::mat4			rotate_;
	float				sameScaling_;
	glm::vec3			scaling_;
	glm::vec3			rotation_;
	glm::vec3			position_;
	float				speed_;

	void				clean_();
	void				updateTransform_();

	static bool			debug_;
};
