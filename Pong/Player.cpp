#include "Player.h"

Player::Player(sf::RectangleShape *sprite, bool controllable) {
	this->sprite = sprite;
	this->controllable = controllable;
	this->velocity = sf::Vector2f(0, 0);
}

bool Player::isControllable() {
	return controllable;
}

void Player::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}

void Player::update(float timeDelta) {
	float moveDelta = 0;
	if(velocity.y == 90) {
		if(sprite->getGlobalBounds().top - velocity.x * timeDelta < 0) {
			moveDelta = -sprite->getGlobalBounds().top;
		} else {
			moveDelta = -timeDelta * velocity.x;
		}
	} else if(velocity.y == 270) {
		if(sprite->getPosition().y + sprite->getSize().y + velocity.x * timeDelta > 1080) {
			moveDelta = 1080 - sprite->getPosition().y - sprite->getSize().y;
		} else {
			moveDelta = timeDelta * velocity.x;
		}
	}
	sprite->move(sf::Vector2f(0, moveDelta));
	velocity = sf::Vector2f(0, 0);
}