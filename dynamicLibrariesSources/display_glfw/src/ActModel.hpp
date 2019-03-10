#pragma once

#include "Model.hpp"
#include "Transform.hpp"

class ActModel : public Transform {
public:

	ActModel();
	ActModel(Model const *model);
	virtual ~ActModel() = default;
	ActModel(ActModel const &src);
	ActModel &operator=(ActModel const &src) = delete;

	Model const		*getModel() const;
	void			assign(Model const *model);

	void			render(Shader &shader, GLenum typeOfDraw = GL_TRIANGLES);

private:
	std::bitset<16>		flag_;
	Model const			*model_;
};
