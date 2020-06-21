#include "ActionBox.h"

//Init static vars
GLboolean ActionBox::filledBottom[5] = { GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE };
glm::vec2 ActionBox::bottomPos[5] = { glm::vec2(0.f,0.f), glm::vec2(0.f,0.f), glm::vec2(0.f,0.f), glm::vec2(0.f,0.f), glm::vec2(0.f,0.f) };
GLfloat ActionBox::scaleChange  = 1.f;

ActionBox::ActionBox() : firstPos(glm::vec2(0, 0)), bottomNum(-1) {
	size = glm::vec2(0, 0);
	pos = glm::vec2(0, 0);
	
	boxTitle = "";
}
ActionBox::ActionBox(std::string BoxTitle, glm::vec2 size, glm::vec2 pos, Action_List action): firstPos(pos), bottomNum(-1), action(action) {
	this->size = size;
	this->pos = pos;
	this->boxTitle = BoxTitle;
}

void ActionBox::setBottomStats(GLfloat scaleChange, glm::vec2 xyChange, glm::vec2 offset) {
	scaleChange = scaleChange;
	for (int i = 0; i < sizeof(bottomPos) / sizeof(glm::vec2); i++) {
		bottomPos[i] = xyChange * (float)i + offset;
	}
}
GLint ActionBox::getActionIndex() {
	Action_List* actions = new Action_List[5];
	GLfloat threshold = 1.0f; //Account for floating point error
	if (this->pos == this->firstPos) {
		return -1;
	} else {
		for (int i = 0; i < sizeof(bottomPos) / sizeof(glm::vec2); i++) {
			
			glm::vec2 diff = this->pos - bottomPos[i];
			if (std::abs(diff.x) < threshold && std::abs(diff.y) < threshold) {
				return i;
			}
		}
		return -1;
	}

}
Action_List ActionBox::getAction() {
	return action;
}



void ActionBox::Draw(glm::vec2 cursorPos, SpriteRender* renderer) {
	if (!Collider::checkPointRecCol(cursorPos, *this) ) {
		//Not Hovering over it
		renderer->DrawSprite(Resource_manager::GetTexture(boxTitle), pos, size, glm::vec4(1.f, 1.f, 1.f, 1.f));
		isSelected = GL_FALSE;
	} else {
		//Hovering over it
		//To check, does this assume direction like I want? Or is it assuming something else?
		
		renderer->DrawQuad(pos - size * 0.05f, size * 1.1f, glm::vec2(0, -1), glm::vec4(0.4f, 0.4f, 0.4f, 0.5f));
		renderer->DrawSprite(Resource_manager::GetTexture(boxTitle), pos, size, glm::vec4(1.f, 1.f, 1.f, 1.f));
		isSelected = GL_TRUE;
	}
	
}



void ActionBox::initClick() {
	this->onClick = [this]() {

		if (this->bottomNum == -1) {
			//For each, if they aren't already filled, put them down there
			for (int i = 0; i < 5; i++) {
				if (!filledBottom[i]) {

					this->pos = bottomPos[i];
					this->filledBottom[i] = GL_TRUE;
					this->bottomNum = i;
					this->size *= scaleChange;

					break;
				}

			}

		} else {
			this->size /= this->scaleChange;
			this->pos = this->firstPos;

			this->filledBottom[this->bottomNum] = GL_FALSE;
			this->bottomNum = -1;

		}
	};
}