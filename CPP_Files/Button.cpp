#include "Button.h"

Button::Button() {
	size = glm::vec2(0, 0);
	pos = glm::vec2(0, 0);
	charSize = 0;
	stringSize = glm::vec2(0,0);
	boxTitle = "Button";
}

//Basic constructor sets values
Button::Button(std::string text, glm::vec2 size, glm::vec2 pos, GLfloat charSize): text(text) {
	this->size = size;
	this->pos = pos;
	this->charSize = charSize;
	StringSize();
	boxTitle = "Button";
}
Button::Button(std::string text, glm::vec2 size, glm::vec2 pos, GLfloat charSize, TextMode mode) : text(text) {
	this->mode = mode;
	this->size = size;
	this->pos = pos;
	this->charSize = charSize;
	StringSize();
	boxTitle = "Button";
}

void Button::setTextColor(glm::vec3 mainColor) {
	this->textColor = mainColor;
}
void Button::setTextColor(glm::vec3 mainColor, glm::vec3 outsideColor) {
	this->textColor = mainColor;
	this->textOutsideColor = outsideColor;
}
void Button::setTextOutsideColor(glm::vec3 outsideColor) {
	this->textOutsideColor = outsideColor;
}

void Button::StringSize() {
	std::string::const_iterator c;
	FontGlyph ch;
	GLfloat sizeX = 0;
	GLfloat sizeY = 0;
	GLfloat scale = charSize / 10.f;

	GLint num = 0;
	GLfloat avgHeight = 0;
	// / 10.f is from random downsize in string draw method
	for (c = this->text.begin(); c != this->text.end(); c++, num++) {
		ch = Resource_manager::GetFont("Arial").getGlyph((unsigned char) *c);
		sizeX += ch.Advance * scale; //TODO: Make the size (here and in draw string) dynamic w/ button size
		
		//Alright, here's the deal. X works perfectly. For y, I have no clue what to do.
		//This is not exact. In fact, it's probaby mathematical nonsense. But it gets the 
		//ballpark, so I'm happy. I'm sorry all you OCD people out there
		GLfloat test = ch.Size.y * scale + ch.Bearing.y * scale;
		test *= -1;
		avgHeight += test;
	}
	avgHeight /= (float)num;
	sizeY = avgHeight;

	this->stringSize = glm::vec2(sizeX, sizeY);
}

//Basic draw function, draws the button texture with the given size and pos
//If the mouse is in the button, changes the button color and updates isSelected
//Will need to update once text feature is up and running
void Button::Draw(glm::vec2 cursorPos, SpriteRender* renderer) {
	//This pos is top left, string pos is bottom left
	// - - - - - - - - - - - - - - - - - - - - -
	//|	                 LABEL                  |
	// - - - - - - - - - - - - - - - - - - - - -
	glm::vec2 stringPos = this->pos;
	stringPos.y += (this->size.y + this->stringSize.y) / 2.f; 
	stringPos.x += (this->size.x - this->stringSize.x) / 2.f;

	if (!Collider::checkPointRecCol(cursorPos, *this)) {


		//Normal button blue words if not selected
		renderer->DrawSprite(Resource_manager::GetTexture(boxTitle), pos, size, glm::vec2(0, -1), glm::vec4(1, 1, 1, 1));
		//renderer->DrawString(text, "Arial", stringPos, this->charSize, glm::vec3(0.0, 0.0, 1.0)); //Draw blue if selected
		renderer->DrawString(text, "Arial", stringPos, this->charSize, this->textColor, this->textOutsideColor, this->mode);
		isSelected = GL_FALSE;
	} else {
		//Blue button Red words if not selected
		renderer->DrawSprite(Resource_manager::GetTexture(boxTitle), pos, size, glm::vec2(0, -1), glm::vec4(0.5, 0.5, 1.0, 1.0));
		//renderer->DrawString(text, "Arial", stringPos, this->charSize, glm::vec3(0.540, 0.062, 0.109)); //Draw blue if selected
		renderer->DrawString(text, "Arial", stringPos, this->charSize, 1.0f - this->textColor, 1.0f - this->textOutsideColor, this->mode);
		isSelected = GL_TRUE;
	}
}
