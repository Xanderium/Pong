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
	nextPosition.x = sprite->getPosition().x + velocity.x * timeDelta;
	nextPosition.y = sprite->getPosition().y + velocity.y * timeDelta;
	return nextPosition;
}

// Check for collisions between puck and left and right
sf::Vector2f Puck::detectCollision(sf::RectangleShape player1Sprite, sf::RectangleShape player2Sprite, sf::Vector2f nextPosition) {
	// Check for collision with right side of left paddle
	if(nextPosition.x - sprite->getRadius() < player1Sprite.getPosition().x + player1Sprite.getSize().x
		&& nextPosition.y + sprite->getRadius() > player1Sprite.getPosition().y
		&& nextPosition.y - sprite->getRadius() < player1Sprite.getPosition().y + player1Sprite.getSize().y) {
			float collisionTime = (player1Sprite.getPosition().x + player1Sprite.getPosition().x - (sprite->getPosition().x - sprite->getRadius())) /
								  (nextPosition.x - sprite->getRadius() - (sprite->getPosition().x - sprite->getRadius()));
			float xAtCollision = sprite->getPosition().x + ((1 - collisionTime) * sprite->getPosition().x + collisionTime * nextPosition.x);
			float xAtCompletion = xAtCollision - (nextPosition.x - xAtCollision);
			nextPosition.x = xAtCompletion;
			velocity = sf::Vector2f(-velocity.x, velocity.y);
	// Check for collision with left side of right paddle
	} else if(nextPosition.x + sprite->getRadius() > player2Sprite.getPosition().x
        && nextPosition.y + sprite->getRadius() > player2Sprite.getPosition().y
        && nextPosition.y - sprite->getRadius() < player2Sprite.getPosition().y + player2Sprite.getSize().y) {
			float collisionTime = (player2Sprite.getPosition().x - (sprite->getPosition().x + sprite->getRadius())) /
								  (nextPosition.x + sprite->getRadius() - (sprite->getPosition().x + sprite->getRadius()));
			float xAtCollision = ((1 - collisionTime) * sprite->getPosition().x + collisionTime * nextPosition.x);
			float xAtCompletion = xAtCollision - (nextPosition.x - xAtCollision);
			nextPosition.x = xAtCompletion;
			velocity = sf::Vector2f(-velocity.x, velocity.y);
	// Check for collision with top wall
    } else if(nextPosition.y - sprite->getRadius() < 0) {
		float collisionTime = -(sprite->getPosition().y - sprite->getRadius()) /
							  (nextPosition.y - sprite->getRadius() - (sprite->getPosition().y - sprite->getRadius()));
		float yAtCollision = sprite->getPosition().y + ((1 - collisionTime) * sprite->getPosition().y + collisionTime * nextPosition.y);
		float yAtCompletion = yAtCollision - (nextPosition.y - yAtCollision);
		nextPosition.y = yAtCompletion;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	// Check for collision with bottom wall
	} else if(nextPosition.y > 1080) {
		float collisionTime = (1080 - (sprite->getPosition().y + sprite->getRadius())) /
							  (nextPosition.y + sprite->getRadius() - (sprite->getPosition().y + sprite->getRadius()));
		float yAtCollision = ((1 - collisionTime) * (sprite->getPosition().y + sprite->getRadius()) + collisionTime * (nextPosition.y + sprite->getRadius()));
		float yAtCompletion = yAtCollision - (nextPosition.y - yAtCollision);
		nextPosition.y = yAtCompletion;
		velocity = sf::Vector2f(velocity.x, -velocity.y);
	}
	return nextPosition;
}