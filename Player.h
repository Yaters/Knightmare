#pragma once //One or the other. Not both
/*
#ifndef PLAYER_H
#define PLAYER_H
*/

#include "GameObject.h"
#include "InfoBox.h"
#include "Action.h"
#include "Collider.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Character.h"
#include "Resource_manager.h"



class Player: public Character {
public:

	Player(Texture2D sprite, GLfloat speed, glm::vec2 pos, glm::vec4 color, glm::vec2 size, glm::vec2 dir);
	Player(Texture2D PlaySprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir, InfoBox Info);
	//9 = size of Action_List enum. This array carries whether an action is happening or not

	
	
	
	void Draw(SpriteRender* renderer, GLfloat dt) override;

	
	void Update(SpriteRender* renderer, Character* enemy, GLfloat dt, GLboolean(&keys)[1024], GLboolean(&clicks)[1024]);

	void setMoves(Action_List moves[5]);
	
	InfoBox info;

private: 

	Action_List moves[5];

	//Functions used only by player
	void UpdateVel(GLboolean(&keys)[1024], GLboolean(&clicks)[1024]);

	//A dictionary of placements for the action box drawing in lower left
	glm::vec2 boxesPos[5];
	glm::vec2 boxesSize;


	void initBoxPlacement(GLint WIDTH, GLint HEIGHT);

};
//#endif

