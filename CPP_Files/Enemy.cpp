#include "Enemy.h"


Enemy::Enemy(Texture2D Sprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir): 
	Character(Sprite, Speed, Pos, Color, Size, Dir) {
	//While infobox here technically has a texture, it's set to whatever enemy's is. Don't use the normal draw function or... Weird body doubles I guess? with bars?
	//If you want to change MaxHP/MaxSP... then you need to change constructor here to include these values.
	info = InfoBox(Sprite, HP, SP, MP, pos - glm::vec2(0, size.y / 3), glm::vec2(size.x, size.y / 7));
	action = new Action();
}


void Enemy::Update(SpriteRender* renderer, Player* player, GLfloat dt) {
	//Update player position
	UpdateAI(player, dt);
	pos += velocity * speed * dt;
	//collider.CircleExcludeCircle(*player, *this);
	info.pos = pos - glm::vec2(0, size.y / 3);
	
}

//Draws little bar above head as well as the sprite
void Enemy::Draw(SpriteRender* renderer, GLfloat dt) {
	info.DrawEnemy(renderer, HP);
	renderer->DrawSprite(sprite, pos, size, dir, color);

	//Action Logic
	//Regen
	if (SP <= 100) SP += 5 * dt;
	if (MP <= 100) MP += 5 * dt;
	//Draw
	action->Draw(this, renderer, dt);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* I LEAVE THE SAME WARNING AS IN PLAYER
* BEYOND LIES ONLY AN 'AI' METHODD. 
* BE WARNED, IT'S GROSS 
* LIKE GROSS TO READ. GROSS VERY MUCH
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */












//Once I start adding in MP/SP costs for real, I'll not do an action sometimes unless there is enough for it
void Enemy::UpdateAI(Player* p1, GLfloat dt) {
	
	glm::vec2 CenterDiff = (p1->pos + 0.5f * p1->size) - (pos + 0.5f * size);
	dir = glm::normalize(CenterDiff);
	GLfloat len = glm::length(CenterDiff);
	len -= size.y * 0.5f;

	timeSinceAction += dt; //Might as well do it here since it'll reset if an action is performed

	//Go towards player
	if (CenterDiff.x < 0) velocity.x = -1; else velocity.x = 1;
	if (CenterDiff.y < 0) velocity.y = -1; else velocity.y = 1;

	if (p1->action->isActive) {
		//if len is in p1 blasting zone
		if (len < p1->action->size.y * 0.5f) {
			velocity *= -1.f; //go away from player
			if (!action->isActive) {
				//If super close, protect or shield based on which costs less
				if (len < p1->action->size.y * 0.125f) {
					//Protect or Shield based on MP/SP
					if (SP <= MP) {
						delete action;
						action = new Action(pos, dir, Action_List::Protect, size);
					}
					else {
						delete action;
						action = new Action(pos, dir, Action_List::Shield, size);
					}
				}
				//Dodge out of zone if sorta close
				else if (len < p1->action->size.y * 0.25f) {
					delete action;
					action = new Action(pos, dir, Action_List::Dodge, size);
				}
				timeSinceAction = 0;
			}


		} else  if (len > p1->action->size.y * 2.f){	
			if (!action->isActive && timeSinceAction >= 1.0) {
				action = new Action(pos, dir, Action_List::Throw, size);
				timeSinceAction = 0;
			}
		}
	} 
	
	//Player is not acting
	else {
		if (SP >= MP && !action->isActive) {
			if (len >= 7 * speed && timeSinceAction >= 0.5) {
				delete action;
				action = new Action(pos, dir, Action_List::Throw, size);
				timeSinceAction = 0;
			}
			else if (len <= 5 * speed && timeSinceAction >= 0.5) {
				delete action;
				action = new Action(pos, dir, Action_List::Charge, size);
				timeSinceAction = 0;
			}
			
		}
		else {
			if (!action->isActive) {
				if (len < size.y * 2.0f) {
					delete action;
					action = new Action(pos, dir, Action_List::Fan, size);
					timeSinceAction = 0;
				}
				else if (len < size.y * 3.0f) {
					delete action;
					action = new Action(pos, dir, Action_List::Square, size);
					timeSinceAction = 0;
				}
			}
			
		}
		

	}
}