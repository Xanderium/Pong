#define _USE_MATH_DEFINES

#include "Puck.h"
#include <cmath>>

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

sf::Vector2f Puck::update(float timeDelta) {
	// SOH CAH TOA
	// x - Adjacent
	// y - Opposite
	// cos(theta) = x / hyp
	// x = hyp * cos(theta)
	// sin(theta) = y / hyp
	// y = hyp * sin(theta)
	float deltaX = velocity.x * timeDelta * cos(velocity.y * M_PI / 180);
	float deltaY = -velocity.x * timeDelta * sin(velocity.y * M_PI / 180);
	sf::Vector2f moveDelta = sf::Vector2f(deltaX, deltaY);
	sprite->move(moveDelta);
	velocity.x += timeDelta * 0.00005f;
	sf::Color spriteColor = sprite->getFillColor();
	// Change color based on the puck's speed
	sprite->setFillColor(sf::Color(255 - abs(255 - (((int) (velocity.x * 100 + 85)) % 512)), 255 - abs(255 - (((int) (velocity.x * 100)) % 512)), 255 - abs(255 - (((int) (velocity.x * 100 + 170)) % 512))));
	return moveDelta;
}