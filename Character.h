#pragma once


class Action;

#include "GameObject.h"
#include "SpriteRender.h"
#include "Resource_manager.h"
#include "Action.h"

//Ok the idea for this came from the update method.
//By using this as a superclass for both, we can define an Update method that calls the Updatevel (or the one overloaded by subclass)
//Followed by the rest of the stuff. collider stuff will probably have to be handled by the game after each functions update method
//keep out for everything involved
//The two Player/Enemy classes also share many characteristics, which is why Enemy extended player in the first place


class Character: public GameObject {

public:
	//Constructors
	Character(Texture2D PlaySprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir);

	//Methods
	void SetDir(glm::vec2 direction);
	virtual void Draw(SpriteRender* renderer, GLfloat dt) = 0;

	//Attributes
	GLfloat speed = 300.;
	glm::vec2 velocity = glm::vec2(0, 0);
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	Texture2D sprite;

	//We'll assume all characters can use actions and take damage (/be killed)
	//Need pointer because we havent defined what an action is yet
	Action* action;

	GLfloat HP = 100.;
	GLfloat SP = 100.;
	GLfloat MP = 100.;

	//~Character() { delete action; };
};




/*
virtual void UpdateVel() = 0; //Update vel is abstract: set by subclass
virtual void Update() = 0; //Update is abstract too: will probably rely on Updatevel (I couldn't make it use updatevel cause unknown parameter needed )
*/ 
//^ I don't have an easy way of doing this wihout knowing the parameters the subclass will need. Like it's possible, but not worth the time spent

