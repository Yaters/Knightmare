#pragma once
#include "SpriteRender.h"
#include <functional>

class Selectable {
public:
	std::function<void()> onClick;

	virtual void Draw(glm::vec2 cursorPos, SpriteRender* renderer) = 0;
	GLboolean isSelected = GL_FALSE;

protected:
	
	std::string boxTitle;
};

