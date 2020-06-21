#include "Collider.h"


/*Well Fudge. I've learned my lesson. Don't trust online 'optimizations' without checking.
* Square Root apparently isn't as 'costly' as some articles made it out to be. I'm not going back to change
* all my work using LSquare and radSquare instead of gl::length, but that's annoying. Real annoying
*/

//These were made for cursor cols (though PointCircle will probably never be used
GLboolean Collider::checkPointRecCol(glm::vec2 point, GameObject rect) {
	GLboolean colx = (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x); //Check that x is between x values
	GLboolean coly = (point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
	return colx && coly;
}

GLboolean Collider::checkPointCircleCol(glm::vec2 point, GameObject circle) {
	glm::vec2 Diff = (circle.pos + 0.5f * circle.size) - point;
	return glm::length(Diff) <= circle.size.y * 0.5f; //Basically is point within rad of center
}



//This isn't adjusted to factor in dir. THIS IS BROKEN DON'T USE IT UNLESS YOU'RE WILLING TO FIX IT
GLboolean Collider::checkRectRectCol(GameObject rect1, GameObject rect2) {
	GLboolean colx = (rect1.pos.x + rect1.size.x >= rect2.pos.x && rect2.pos.x + rect2.size.x >= rect1.pos.x);
	GLboolean coly = (rect1.pos.y + rect1.size.y >= rect2.pos.y && rect2.pos.y + rect2.size.y >= rect1.pos.y);
	return colx && coly;
}




GLboolean Collider::checkCircleRectCol(GameObject circle, GameObject rect) {
	//Basically ctrlc ctrlv from SpriteRender.cpp
	glm::mat2 rotate = glm::mat2(rect.dir.y, rect.dir.x,
						-rect.dir.x, rect.dir.y);

	glm::vec2 circleCenter = circle.pos + 0.5f * circle.size;
	glm::vec2 diff = circleCenter - (rect.pos + 0.5f * rect.size); //Vec2 going from rect center to circle center

	diff = rotate * diff; //rotate it to follow rectangle? 
	circleCenter = diff; //Set circleCenter to rotate later
	glm::vec2 closest = glm::clamp(diff, -0.5f * rect.size, 0.5f * rect.size); //Clamps Diff to be within rectangle- closest = closest point to circle
	diff = closest - circleCenter;
	return (LSquare(diff)) <= (float)(0.25 * circle.size.y * circle.size.y); //If point->circle center > r, it's colliding
}

//Ok we're assuming triangle texture on quad, with center and top corners as the points.
GLboolean Collider::checkCircleTriangleCol(GameObject circle, GameObject tri) {
	//This is the inverse of normal rotation matrix, rotates clockwise instead of counterclockwise (reason is because the dir.y is now opposite dir.y wanted
	glm::mat2 rotate = glm::mat2(tri.dir.y, -tri.dir.x,
		tri.dir.x, tri.dir.y);


	//LSquare returns length squared: -radSquare means instead of checking for <radSquare, we can check <0

	//List of stuff we'll need
	//I tried -y on CircCenter to adjust to normal graph, but it broke it. This works
	glm::vec2 CircCenter = circle.pos + 0.5f * circle.size - (tri.pos + 0.5f * tri.size);
	GLfloat radSquare = circle.size.y * circle.size.y * 0.25f;

	//Points (P0 is 0,0)
	glm::vec2 P1 = rotate * glm::vec2(tri.size.x / 2, tri.size.y / 2);
	glm::vec2 P2 = rotate * glm::vec2(-tri.size.x / 2, tri.size.y / 2);
	// Points to circle Center from vertices (C0 was replaced by CircCenter)
	glm::vec2 C1 = CircCenter - P1;
	glm::vec2 C2 = CircCenter - P2;
	//length of previous vector squared
	GLfloat C0Squared = LSquare(CircCenter) - radSquare;
	GLfloat C1Squared = LSquare(C1) - radSquare;
	GLfloat C2Squared = LSquare(C2) - radSquare;
	//Edge vectors (e0 was = P1 and e2 was = -P2, both were replaced)
	glm::vec2 e1 = P2 - P1;

	/* Ok here's the deal. Right now I'm using a quad with a triangle texture to draw Triangles.
	"wtf man? Opengl is like made for triangles!" i know, i know. But making a triangle draw
	means making a new Draw in SpriteRender, two new shaders, and a much more annoying system for
	finding vertices in here. Then I need that TWICE to get textured triangles. So sorry */

	//CASE #1: CORNERS IN CIRCLE
	if (C0Squared <= 0 || C1Squared <= 0 || C2Squared <= 0) return GL_TRUE;

	//CASE #2: CIRCLE INSIDE TRIANGLE

	//I might be able to optimize this more, but I won't for readability. Normal = (y2-y1, x1-x2)
	glm::vec2 Norm0(P1.y, -P1.x);
	glm::vec2 Norm1(P2.y - P1.y, P1.x - P2.x);
	glm::vec2 Norm2(-P2.y, P2.x);

	//If in triangle side of edge, dot is negative(I did counterclock order for norms)
	if (glm::dot(Norm0, CircCenter) < 0 && glm::dot(Norm1, C1) < 0 && glm::dot(Norm2, C2) < 0) return GL_TRUE;


	// CASE #3: EDGE IN CIRCLE

	//k is just a temp value to store dot product. technically the length along triangle to where circle is nearest to edge
	GLfloat k = glm::dot(CircCenter, P1);
	//If dot product is positive(not past p1)
	if (k > 0) {
		GLfloat sideLenSquare = LSquare(P1);
		k *= k / sideLenSquare;
		//Checks if it's past P0 (bigger than edge length)
		if (k < sideLenSquare) {
			if (C0Squared <= k) return GL_TRUE;
		}
	}


	k = glm::dot(C1, e1);
	if (k > 0) {
		GLfloat sideLenSquare = LSquare(e1);
		k *= k / sideLenSquare;
		if (k < sideLenSquare) {
			if (C1Squared <= k) return GL_TRUE;
		}
	}

	k = glm::dot(C2, -P2);
	if (k > 0) {
		GLfloat sideLenSquare = LSquare(-P2);
		k *= k / sideLenSquare;
		if (k < sideLenSquare) {
			if (C2Squared <= k) return GL_TRUE;
		}
	}

	return GL_FALSE;
}


//For Circles I default to using y size as radius for things like background
GLboolean Collider::checkCircleCircleCol(GameObject circle1, GameObject circle2) {
	//Find vector w/ difference between centers
	glm::vec2 temp = (circle1.pos + circle1.size * 0.5f) - (circle2.pos + circle2.size * 0.5f);
	//If length of distance <= length of radii combined, it's a collision
	return glm::length(temp) <=  (circle1.size.y * 0.5f + circle2.size.y * 0.5f);
}

//Contains a given circle within the circular GameObject
void Collider::CircleContainCircle(GameObject outsideCircle, GameObject& insideCircle) {
	//Vector between centers
	glm::vec2 diff = (insideCircle.pos + insideCircle.size * 0.5f) - (outsideCircle.pos + outsideCircle.size * 0.5f);
	GLfloat len = glm::length(diff);
	//If(centers are closer than difference in radii) <- difference because one is inside
	if (len > (0.5f * (outsideCircle.size.y - insideCircle.size.y))) {
		//insideCircle.pos = diff * (0.5f * (outsideCircle.size.y - insideCircle.size.y)) / len;  //LOL this is fun but not what I'm going for: Try it out
		insideCircle.pos = diff * 0.5f * (outsideCircle.size.y - insideCircle.size.y) / len; // First move circle center to inside 
		insideCircle.pos -= 0.5f * insideCircle.size; // then move pos to top left of circle sprite
		insideCircle.pos += outsideCircle.size * 0.5f; //then move pos to be relative to screen/big circle
	}
}

//Exclued a given circle from the circular GameObject
void Collider::CircleExcludeCircle(GameObject circleStay, GameObject& circleKeepOut) {
	//Vector between centers
	glm::vec2 diff = (circleKeepOut.pos + circleKeepOut.size * 0.5f) - (circleStay.pos + circleStay.size * 0.5f);
	GLfloat len = glm::length(diff);
	
	if (len < (0.5f * (circleStay.size.y + circleKeepOut.size.y))) { //If distance < combined radii
		diff /= len; //Normalize diff
		
		len = 0.5f * (circleStay.size.y + circleKeepOut.size.y); //Set the distance between equal to what we want
		diff *= len; //Multiply diff(gives direction) by desired length
		circleKeepOut.pos = diff + (circleStay.pos + circleStay.size * 0.5f); // Set outside circle to be Circle Stay center + new diff vector
		circleKeepOut.pos -= circleKeepOut.size * 0.5f; // Then adjust so pos = top left of circle
	}
	
	
}