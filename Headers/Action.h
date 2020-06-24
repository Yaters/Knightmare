#pragma once //One or the other. Not both
/*
#ifndef ACTION_H
#define ACTION_H
*/
class Character;

#include "SpriteRender.h"
#include <glm/glm.hpp>
#include "Resource_manager.h"
#include "GameObject.h"
#include "Character.h"
#include "Collider.h"

enum Action_List {
	Charge, Bomb, Throw, Stomp, Dodge, Shield,
	Fan, Square, Protect, Empty
};

class Action: public GameObject {
public: 

	Action(glm::vec2 startPos, glm::vec2 startDir, Action_List Type, glm::vec2 Psize);
	Action();
	
	

	GLboolean isActive = GL_FALSE;
	

	//Handles action rendering
	void Draw(Character* ch, SpriteRender* renderer, GLfloat dt);


	//Static


	//Handles action damage between two characters (each with own action)
	static void DamageUpdate(Character* player, Character* enemy, GLfloat dt);

	//Literally just stores the name of the texture for the box of each action. Treat like Texture dictionary
	static std::string actionBoxes[9];
	

private:
	Action_List type;

	//Data when action occurs
	glm::vec2 startPos;
	glm::vec2 startDir; // NOT Encompassed by Gameobject Dir because then it wouldn't update dir

	//Action 'life' is seconds until it is done. Subtracts dt from corresponding 'life' value every time action is used
	GLfloat life;
	GLfloat lifeMax;

	GLfloat damage = 0;

	GLfloat SPcost = 0;
	GLfloat MPcost = 0;




};

//#endif
