#pragma once
#include "GameObject.h"
#include "SpriteRender.h"
#include "Collider.h"
#include "Selectable.h"

#include <functional>



class Button: public GameObject, public Selectable {
public:
	Button();
	Button(std::string text, glm::vec2 size, glm::vec2 pos, GLfloat charSize);
	Button(std::string text, glm::vec2 size, glm::vec2 pos, GLfloat charSize, TextMode mode);

	void Draw(glm::vec2 cursorPos, SpriteRender* renderer) override;

	void setTextColor(glm::vec3 mainColor);
	void setTextColor(glm::vec3 mainColor, glm::vec3 outsideColor);
	void setTextOutsideColor(glm::vec3 outsideColor);


private:
	std::string text = "";
	
	void StringSize();

	GLfloat charSize;
	glm::vec2 stringSize;

	TextMode mode = TextMode::GLOW;

	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec3 textOutsideColor = glm::vec3(1, 0, 0);

};

