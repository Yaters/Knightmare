#pragma once
#include "Resource_manager.h"
#include "Selectable.h"
#include "Collider.h"
#include "Action.h"

class ActionBox : public GameObject, public Selectable {
public:
	ActionBox();
	ActionBox(std::string BoxTitle, glm::vec2 size, glm::vec2 pos, Action_List action);
	void Draw(glm::vec2 cursorPos, SpriteRender* renderer) override;
	void initClick();
	GLint getActionIndex();
	Action_List getAction();

	static void setBottomStats(GLfloat scaleChange, glm::vec2 xyChange, glm::vec2 offset);
	

private:

	glm::vec2 firstPos;

	int bottomNum = -1;
	Action_List action;

	static GLfloat scaleChange;
	
	static glm::vec2 bottomPos[5];
	static GLboolean filledBottom[5];
};

