#pragma once
#include "SFML/Graphics.hpp"

// Puck class for handling puck movement
class Puck {
public:
	Puck(sf::CircleShape *sprite, sf::Vector2f initialVelocity);
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity();
	sf::Vector2f getNextPosition(float timeDelta);
	sf::Vector2f detectCollision(sf::RectangleShape player1Sprite, sf::RectangleShape player2Sprite, sf::Vector2f newPosition);

private:
	sf::CircleShape* sprite;
	sf::Vector2f velocity;
};
