#pragma once //One or the other. Not both
/*
#ifndef ENEMY_H
#define ENEMY_H
*/
#include "Player.h"

//Ok, need to transition enemy into character. That's tomorrow's job
//Woah wait there was that thing in modelling about common attributes. You could try that
class Enemy : public Character {
public:
	Enemy(Texture2D sprite, GLfloat speed, glm::vec2 pos, glm::vec4 color, glm::vec2 size, glm::vec2 dir);

	//void ActionDraw(SpriteRender* renderer, GLfloat dt); //Enemy AI, which will be... tough to say the least
	void Update(SpriteRender* renderer, Player* opponent, GLfloat dt);

	void Draw(SpriteRender* renderer, GLfloat dt) override; //Will draw a little bar over head for healths

private:
	//Helper methods
	void UpdateAI(Player* p1, GLfloat dt);
	void ActionDraw(SpriteRender* renderer, GLfloat dt);

	//Attributes only seen by methods
	GLfloat timeSinceAction = 0; //Time since the last action was performed, used for updatePos
	InfoBox info;



};
//#endif
