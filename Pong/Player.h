#pragma once
#include "SFML/Graphics.hpp"

class Player {
	public:
		Player(sf::RectangleShape *sprite, bool isControllable);
		bool isControllable();
		void setVelocity(sf::Vector2f velocity);
		void update(float timeDelta);

	private:
		sf::RectangleShape* sprite;
		bool controllable;
		sf::Vector2f velocity;
};

