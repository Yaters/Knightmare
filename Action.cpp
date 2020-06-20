#include "Action.h"

//Normal  Player speed = 300. Maybe in future make action speed a multiple of player speed
//Draw Sprite(Texture, Position, size, direction, color)
//Draw Quad(Position, Size, Direction, Color


std::string Action::actionBoxes[9] = {
		"ChargeBox", "BombBox", "ThrowBox", "StompBox", "DodgeBox", "ShieldBox",
		"FanBox", "SquareBox", "ProtectBox"
};

Action::Action(): MPcost(0), SPcost(0), life(0), lifeMax(0), type(Empty) {
	startPos = glm::vec2(0, 0);
	startDir = glm::vec2(0, -1);
	dir = glm::vec2(0, -1);
	isActive = GL_FALSE;
}

//Sets variables then makes the action active
Action::Action(glm::vec2 StartPos, glm::vec2 Dir, Action_List Type, glm::vec2 Psize) : startPos(StartPos), startDir(Dir), type(Type) {
	//Basically it doesnt't like superclass stuff in constructor
	this->dir = Dir;
	switch (type) {
		case Action_List::Charge:
			size = Psize;
			life = 0.4f;
			damage = 15.;
			SPcost = 20.;
			MPcost = 0.;
			break;
		case Action_List::Bomb:
			size = 5.f * Psize; 
			life = 2.;
			damage = 25.;
			SPcost = 30.;
			MPcost = 10.;
			break;
		case Action_List::Throw:
			size = Psize * 0.5f;
			life = 0.5;
			damage = 2.5;
			SPcost = 10.;
			MPcost = 0.;
			break;
		case Action_List::Stomp:
			size = 4.f * Psize;
			life = 2.;
			damage = 15.;
			SPcost = 20.;
			MPcost = 20.;
			break;
		case Action_List::Dodge:
			size = glm::vec2(0, 0);
			life = 0.1f;
			damage = 0.;
			SPcost = 10.;
			MPcost = 0.;
			break;
		case Action_List::Shield:
			size = Psize;
			life = 1.;
			damage = 0.;
			SPcost = 20.;
			MPcost = 0.;
			break;
		case Action_List::Fan:
			size = 9.f * Psize; 
			life = 2.;
			damage = 25.;
			SPcost = 20.;
			MPcost = 30.;
			break;
		case Action_List::Square:
			size = Psize * 6.f;
			life = 2.;
			damage = 20.;
			SPcost = 0.;
			MPcost = 40.;
			break;
		case Action_List::Protect:
			size = Psize;
			life = 1.5;
			damage = 0.;
			SPcost = 0.;
			MPcost = 30.;
			break;
		default:
			size = glm::vec2(0, 0);
			pos = glm::vec2(0, 0);
			life = 0.;
			damage = 0.;
			SPcost = 0.;
			MPcost = 0.;
	}
	lifeMax = life;
	isActive = GL_TRUE;
}



void Action::Draw(Character* ch,  SpriteRender* renderer, GLfloat dt) {

	if (!this->isActive) return;

	if (ch->SP < 0 || ch->MP < 0) this->isActive = GL_FALSE; //Stop this whole thing if either is true
	else {
		//Otherwise take the cost out
		ch->MP -= this->MPcost * dt / this->lifeMax;
		ch->SP -= this->SPcost * dt / this->lifeMax;
	}


	

	//Action sizes are mostly set by Action.cpp
	this->dir = ch->dir; //Bit of preemptive setting (I change it in switch if it's different)


	//Can't make a draw function in Action, because then you'd need Player, which creates a mess of knowing what player has before player exists
	switch (this->type) {

		//Charge
	case Action_List::Charge:
		this->pos = ch->pos;
		ch->pos += 5.f * ch->speed * dt * this->startDir;
		this->life -= dt;
		break;

		//Bomb
	case Action_List::Bomb:

		if (this->life > 1.0) {
			this->pos = this->startPos + 2.f * ch->speed * (this->lifeMax - this->life) * this->startDir + ch->size * 0.25f; //start + change + adjust to size
			renderer->DrawSprite(Resource_manager::GetTexture("Bomb"), this->pos, 0.5f * ch->size, this->startDir);
			this->life -= dt; //Stuff is missing here because there are no collisions for bomb at this time- pls don't overcomplicate
		}
		else if (this->life > 0.0) {
			this->pos = this->startPos + 2.f * ch->speed * this->startDir - 2.f * ch->size; //start + bomb move + delta square
			renderer->DrawQuad(this->pos, this->size, this->startDir, glm::vec4(1, 0, 0, 0.5));
			this->life -= dt;
			this->dir = this->startDir;
		}
		break;

		//Throw
	case Action_List::Throw:

		this->pos = this->startPos + 2400.f * (this->lifeMax - this->life) * this->startDir + ch->size * 0.25f;
		renderer->DrawSprite(Resource_manager::GetTexture("Throw"), this->pos, this->size, this->startDir);
		this->life -= dt;
		this->dir = this->startDir;
		break;

		//Stomp
	case Action_List::Stomp:
		this->pos = ch->pos - 1.5f * ch->size;
		renderer->DrawQuad(this->pos, this->size, ch->dir, glm::vec4(1, 0, 0, 0.5));
		this->life -= dt;
		break;

		//Dodge
	case Action_List::Dodge:
		ch->pos += 8.f * ch->speed * dt * ch->velocity;
		this->life -= dt;
		break;

		//Shield
	case Action_List::Shield:
		renderer->DrawSprite(Resource_manager::GetTexture("Shield"), ch->pos, ch->size, ch->dir);
		this->life -= dt;
		break;

		//Fan
	case Action_List::Fan:
		//Note to self: Fan texture is made so it's actually a square around the player
		this->pos = ch->pos - 4.f * ch->size;
		if (this->life >= 1.0) {
			renderer->DrawSprite(Resource_manager::GetTexture("FanWarn"), this->pos, this->size, ch->dir, glm::vec4(1, 0, 0, 0.5));
			this->life -= dt;
		}
		else if (this->life >= 0.0) {
			renderer->DrawSprite(Resource_manager::GetTexture("Fan"), this->pos, this->size, ch->dir, glm::vec4(1, 1, 1, 0.5));
			this->life -= dt;
		}
		break;

		//Square
	case Action_List::Square:
		//Center quad on player
		this->pos = ch->pos - 2.5f * ch->size;
		renderer->DrawQuad(this->pos, this->size, ch->dir, glm::vec4(1, 0, 0, 0.5));
		this->life -= dt;
		break;

		//Protect
	case Action_List::Protect:
		renderer->DrawSprite(Resource_manager::GetTexture("Protect"), ch->pos, ch->size, ch->dir, glm::vec4(1, 1, 1, 0.4));
		this->life -= dt;
		break;

	default: //Empty probably
		break;

	}
	if (this->life <= 0.0) this->isActive = GL_FALSE; //Always true

}



void Action::DamageUpdate(Character* P, Character* E, GLfloat dt) {

	//Stuff I'll need in case either has got a prot action going
	GLfloat lastPlayerHP = P->HP;
	GLfloat lastEnemyHP = E->HP;
	GLboolean playerProt = GL_FALSE;

	//Deal damage to enemy
	if (P->action->isActive) {
		switch (P->action->type) {
		case Action_List::Charge:
			if (Collider::checkCircleCircleCol(*P, *E)) {
				E->HP -= P->action->damage; //Do damage and stop the charge
				P->action->isActive = GL_FALSE;
				P->SP -= P->action->SPcost * (1 - (P->action->life / P->action->lifeMax));
			}
			break;
		case Action_List::Bomb:
			if (P->action->life < 1.0 && Collider::checkCircleRectCol(*E, * P->action)) {
				E->HP -= P->action->damage * dt; //if in square, for full 1sec, full damage is done
			}
			break;
		case Action_List::Throw:
			if (Collider::checkCircleRectCol(*E, * P->action)) {
				E->HP -= P->action->damage;
				P->action->isActive = GL_FALSE;
			}
			break;
		case Action_List::Stomp:
			if (Collider::checkCircleRectCol(*E, * P->action)) {
				E->HP -= P->action->damage * dt / P->action->lifeMax; //Like bomb, but life != 1sec
			}
			break;
		case Action_List::Dodge:
			if (Collider::checkCircleCircleCol(*P, *E)) {
				P->action->isActive = GL_FALSE; //Might be unecessary with playerplayercol working - to be removed
			}
			break;
		case Action_List::Shield:
			playerProt = GL_TRUE; //Now I can update HP back to regular at very end. Can't do here because need to check enemy actions for stop
			break;
		case Action_List::Fan:
			if (P->action->life < 1.0 && Collider::checkCircleTriangleCol(*E, * P->action)) {
				E->HP -= P->action->damage * dt; //Like bomb
			}
			break;
		case Action_List::Square:
			if (Collider::checkCircleRectCol(*E, * P->action)) {
				E->HP -= P->action->damage * dt / P->action->lifeMax; //Basically Stomp
			}
			break;
		case Action_List::Protect:
			playerProt = GL_TRUE; //Now I can update HP back to regular at very end
			break;
		default: //Empty Probably
			break;
		}
	}

	//Deal damage to player
	if (E->action->isActive) {
		switch (E->action->type) {
		case Action_List::Charge:
			if (Collider::checkCircleCircleCol(*E, *P)) {
				P->HP -= E->action->damage;
				E->action->isActive = GL_FALSE;
				E->SP -= E->action->SPcost * (1 - (E->action->life / E->action->lifeMax));//Do damage and stop the charge
			}
			break;
		case Action_List::Bomb:
			if (E->action->life < 1.0 && Collider::checkCircleRectCol(*P, * E->action)) {
				P->HP -= E->action->damage * dt; //if in square, for full 1sec, full damage is done
			}
			break;
		case Action_List::Throw:
			if (Collider::checkCircleRectCol(*P, * E->action)) {
				P->HP -= E->action->damage;
				E->action->isActive = GL_FALSE;
			}
			break;
		case Action_List::Stomp:
			if (Collider::checkCircleRectCol(*P, * E->action)) {
				P->HP -= E->action->damage * dt / E->action->lifeMax; //Like bomb, but life != 1sec
			}
			break;
		case Action_List::Dodge:
			if (Collider::checkCircleCircleCol(*E, *P)) {
				E->action->isActive = GL_FALSE; //Doesn't do damage, but don't want collisions. May be unecessary if future player col update works
			}
			break;
		case Action_List::Shield:
			E->HP = lastEnemyHP; //Undo damage done by player
			break;
		case Action_List::Fan:
			if (E->action->life < 1.0 && Collider::checkCircleTriangleCol(*P, * E->action)) {
				P->HP -= E->action->damage * dt; //Like bomb
			}
			break;
		case Action_List::Square:
			if (Collider::checkCircleRectCol(*P, * E->action)) {
				P->HP -= E->action->damage * dt / E->action->lifeMax; //Basically Stomp
			}
			break;
		case Action_List::Protect:
			E->HP = lastEnemyHP; //Undo damage done by player
			break;
		default: //Empty
			break;
		}
	}

	if (playerProt)P->HP = lastPlayerHP; //Couldn't do this in switch because damage wasn't dealt yet
}



