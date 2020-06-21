#pragma once //One or the other. Not both
/**
#ifndef COLLIDER_H
#define COLLIDER_H
*/

#include "GameObject.h"
#include <glm/glm.hpp>


//Collider singleton is essentially just a wrapper for the methods
class Collider {
public:
	static GLboolean checkRectRectCol(GameObject rect1, GameObject rect2);
	static GLboolean checkRectCircleCol(GameObject rect, GameObject circle) {
		return checkCircleRectCol(circle, rect);
	}
	static GLboolean checkCircleRectCol(GameObject circle, GameObject rect);
	static GLboolean checkCircleCircleCol(GameObject circle1, GameObject circle2);
	static GLboolean checkCircleTriangleCol(GameObject circle, GameObject tri);
	static void CircleContainCircle(GameObject outsideCircle, GameObject& insidecircle);
	static void CircleExcludeCircle(GameObject circleStay, GameObject& circleKeepOut);

	static GLboolean checkPointRecCol(glm::vec2 point, GameObject rect);
	static GLboolean checkPointCircleCol(glm::vec2 point, GameObject circle);

private:
	static inline GLfloat LSquare(glm::vec2 vector) {
		return vector.x * vector.x + vector.y * vector.y;
	}
	
	Collider() { }
};

//#endif

