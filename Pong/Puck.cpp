#define _USE_MATH_DEFINES

#include "Puck.h"
#include <iostream>
#include <cmath>

Puck::Puck(sf::CircleShape *sprite, sf::Vector2f initialVelocity) {
	this->sprite = sprite;
	this->velocity = initialVelocity;
}

void Puck::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}

sf::Vector2f Puck::getVelocity() {
	return velocity;
}

sf::Vector2f Puck::getNextPosition(float timeDelta) {
	sf::Vector2f nextPosition;
	sf::Vector2f curPosition = sprite->getPosition();
	nextPosition.x = curPosition.x + velocity.x * timeDelta;
	nextPosition.y = curPosition.y + velocity.y * timeDelta;
	return nextPosition;
}

sf::Vector2f findIntersection(sf::Vector2f linePoint1, sf::Vector2f linePoint2, sf::Vector2f wallPoint1, sf::Vector2f wallPoint2) {
	// 1 = Vertical
	// 0 = Horizontal
	int directionOfWall = (wallPoint1.x != wallPoint2.x ? 0 : 1);

	float linePoint1Value1 = (directionOfWall > 0 ? linePoint1.x : linePoint1.y);
	float linePoint2Value1 = (directionOfWall > 0 ? linePoint2.x : linePoint2.y);
	float linePoint1Value2 = (directionOfWall > 0 ? linePoint1.y : linePoint1.x);
	float linePoint2Value2 = (directionOfWall > 0 ? linePoint2.y : linePoint2.x);
	float wallValue1 = (directionOfWall > 0 ? wallPoint1.y : wallPoint1.x);
	float wallValue2 = (directionOfWall > 0 ? wallPoint2.y : wallPoint2.x);
	float wallPosition = (directionOfWall > 0 ? wallPoint1.x : wallPoint1.y);

	float timeAtCross = (wallPosition - linePoint1Value1) / (linePoint2Value1 - linePoint1Value1);
	float valueAtCross = (1 - timeAtCross) * linePoint1Value2 + timeAtCross * linePoint2Value2;

	float distancePoint1ToWall = wallPosition - linePoint1Value1;
	float distancePoint2ToWall = wallPosition - linePoint2Value1;
	bool opposingSidesOfWall = distancePoint1ToWall * distancePoint2ToWall < 0;
	if(opposingSidesOfWall &&  valueAtCross > wallValue1 && valueAtCross < wallValue2) {
		float intersectionX = (directionOfWall > 0 ? wallPosition : valueAtCross);
		float intersectionY = (directionOfWall > 0 ? valueAtCross : wallPosition);
		return sf::Vector2f(intersectionX, intersectionY);
	} else return sf::Vector2f(NULL, NULL);
}

// Check for collisions between puck and left and right
sf::Vector2f Puck::detectCollision(sf::RectangleShape player1Sprite, sf::RectangleShape player2Sprite, sf::Vector2f nextPosition) {

	sf::Vector2f wallPoint1;
	sf::Vector2f wallPoint2;
	sf::Vector2f intersection = sf::Vector2f(NULL, NULL);

	sf::Vector2f puckPosition = sprite->getPosition();
	float puckRadius = sprite->getRadius();

	// Check for collisions with left paddle
	sf::Vector2f player1Position = player1Sprite.getPosition();
	sf::Vector2f player1Size = player1Sprite.getSize();

	// Check left paddle right wall

	// Get line for left paddle's right wall
	wallPoint1 = sf::Vector2f(player1Position.x + player1Size.x, player1Position.y);
	wallPoint2 = sf::Vector2f(player1Position.x + player1Size.x, player1Position.y + player1Size.y);
	
	// Find intersection between both lines
	intersection = findIntersection(sf::Vector2f(puckPosition.x - puckRadius, puckPosition.y), sf::Vector2f(nextPosition.x - puckRadius, nextPosition.y), wallPoint1, wallPoint2);
	
	if(intersection.x != NULL) {
		nextPosition.x = intersection.x - (nextPosition.x - puckRadius - intersection.x) + puckRadius;
		velocity = sf::Vector2f(-velocity.x, velocity.y);
	}

	// * Repeat as such for all other walls the puck can collide with

	// Check left paddle bottom wall
	wallPoint1 = sf::Vector2f(player1Position.x, player1Position.y + player1Size.y);
	wallPoint2 = sf::Vector2f(player1Position.x + player1Size.x, player1Position.y + player1Size.y);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y - puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y - puckRadius), wallPoint1, wallPoint2);
	if(intersection.x != NULL) {
		std::cout << "asdf" << std::endl;
		nextPosition.y = intersection.y - (nextPosition.y - puckRadius - intersection.y) + puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	// Check left paddle top wall
	wallPoint1 = sf::Vector2f(player1Position.x, player1Position.y);
	wallPoint2 = sf::Vector2f(player1Position.x + player1Size.x, player1Position.y);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y + puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y + puckRadius), wallPoint1, wallPoint2);
	if(intersection.x != NULL) {
		std::cout << "fdsa" << std::endl;
		nextPosition.y = intersection.y - (nextPosition.y + puckRadius - intersection.y) - puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	// Check for collisions with right paddle
	sf::Vector2f player2Position = player2Sprite.getPosition();
	sf::Vector2f player2Size = player2Sprite.getSize();

	// Check right paddle left wall
	wallPoint1 = sf::Vector2f(player2Position.x, player2Position.y);
	wallPoint2 = sf::Vector2f(player2Position.x, player2Position.y + player2Size.y);
	intersection = findIntersection(sf::Vector2f(puckPosition.x + puckRadius, puckPosition.y), sf::Vector2f(nextPosition.x + puckRadius, nextPosition.y), wallPoint1, wallPoint2);
	if(intersection.x != NULL) {
		nextPosition.x = intersection.x - (nextPosition.x + puckRadius - intersection.x) - puckRadius;
		velocity = sf::Vector2f(-velocity.x, velocity.y);
	}
	
	// Check right paddle bottom wall
	wallPoint1 = sf::Vector2f(player2Position.x, player2Position.y + player1Size.y);
	wallPoint2 = sf::Vector2f(player2Position.x + player2Size.x, player2Position.y + player1Size.y);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y - puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y - puckRadius), wallPoint1, wallPoint2);
	if (intersection.x != NULL) {
		std::cout << "asdf" << std::endl;
		nextPosition.y = intersection.y - (nextPosition.y - puckRadius - intersection.y) + puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	// Check right paddle top wall
	wallPoint1 = sf::Vector2f(player2Position.x, player2Position.y);
	wallPoint2 = sf::Vector2f(player2Position.x + player2Size.x, player2Position.y);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y + puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y + puckRadius), wallPoint1, wallPoint2);
	if (intersection.x != NULL) {
		std::cout << "fdsa" << std::endl;
		nextPosition.y = intersection.y - (nextPosition.y + puckRadius - intersection.y) - puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	// Check for collisions with top and bottom walls

	// Check top wall
	wallPoint1 = sf::Vector2f(0, 0);
	wallPoint2 = sf::Vector2f(2160, 0);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y - puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y - puckRadius), wallPoint1, wallPoint2);
	if (intersection.x != NULL) {
		nextPosition.y = intersection.y - (nextPosition.y - puckRadius - intersection.y) + puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	// Check bottom wall
	wallPoint1 = sf::Vector2f(0, 1080);
	wallPoint2 = sf::Vector2f(2160, 1080);
	intersection = findIntersection(sf::Vector2f(puckPosition.x, puckPosition.y + puckRadius), sf::Vector2f(nextPosition.x, nextPosition.y + puckRadius), wallPoint1, wallPoint2);
	if(intersection.x != NULL) {
		nextPosition.y = intersection.y - (nextPosition.y + puckRadius - intersection.y) - puckRadius;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}

	return nextPosition;
}