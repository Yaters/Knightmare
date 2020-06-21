#include "InfoBox.h"


InfoBox::InfoBox() : MaxHP(100.), MaxSP(100.), MaxMP(100.) {
	this->size = glm::vec2(350, 150);
	this->pos = glm::vec2(1245, 1045);
	this->dir = glm::vec2(0, -1);
}
InfoBox::InfoBox(Texture2D sprite, GLfloat MaxHP, GLfloat MaxSP, GLfloat MaxMP, glm::vec2 pos, glm::vec2 size): MaxHP(MaxHP), MaxSP(MaxSP), MaxMP(MaxMP) {
	this->sprite = sprite;
	this->pos = pos;
	this->size = size;
	this->dir = glm::vec2(0, -1);
}

void InfoBox::Draw(SpriteRender* renderer, GLfloat HP, GLfloat SP, GLfloat MP) {

	renderer->DrawSprite(sprite, pos, size, dir);

	//All the ratios (like 27/35 or 9.5/15) can be used because specific dimensions of box are known at creation. 
	//Box is og 150 by 350 pixels

	GLfloat HPbarsize = static_cast<GLfloat>( (size.x * 27. / 35.) * (HP / MaxHP) );
	GLfloat SPbarsize = static_cast<GLfloat>( (size.x * 27. / 35.) * (SP / MaxSP) );
	GLfloat MPbarsize = static_cast<GLfloat>( (size.x * 27. / 35.) * (MP / MaxMP) );
	GLfloat barHeight = static_cast<GLfloat>( size.y * 1.7 / 15. );

	//Red
	renderer->DrawQuad(this->pos + glm::vec2(this->size.x / 5.0, this->size.y * 6.5/ 15.), glm::vec2(HPbarsize, barHeight), dir, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//Yellow
	renderer->DrawQuad(this->pos + glm::vec2(this->size.x / 5.0, this->size.y * 9.5 / 15.), glm::vec2(SPbarsize, barHeight), dir, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//Blue
	renderer->DrawQuad(this->pos + glm::vec2(this->size.x / 5.0, this->size.y * 12.5 / 15.), glm::vec2(MPbarsize, barHeight), dir, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

}

//When constructing this, the pos 
void InfoBox::DrawEnemy(SpriteRender* renderer, GLfloat HP) {
	
	//renderer->DrawSprite(sprite, pos, size, dir);
	renderer->DrawQuad(pos, size, dir, glm::vec4(0, 0, 0, 0.9));
	renderer->DrawQuad(pos, glm::vec2((HP / MaxHP) * size.x, size.y), dir, glm::vec4(1., 0, 0, 0.9));

	//Maybe try at one point so constructor only needs to specify player pos and size

}