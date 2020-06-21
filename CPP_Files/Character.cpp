#include "Character.h"


Character::Character(Texture2D PlaySprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir) : 
	GameObject(Pos, Size, Dir), sprite(PlaySprite), speed(Speed), color(Color) {
	//It's literally just copying all the values in
}

void Character::SetDir(glm::vec2 direction) {
	dir = direction;
}
void Character::Draw(SpriteRender* renderer, GLfloat dt) {
	renderer->DrawSprite(sprite, pos, size, dir, color);

}