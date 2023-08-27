#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Player.h"
#include "Puck.h"

int main() {
    std::cout << "Hello World!\n";

    srand(time(NULL));

    // Import sounds
    sf::SoundBuffer soundBuffer;

    soundBuffer.loadFromFile("Sounds/puckcollision.wav");
    sf::Sound puckCollisionSound = sf::Sound(soundBuffer);
    float puckCollisionSoundPitch;
    int paddleHitSoundNum;

    sf::RenderWindow window(sf::VideoMode(600, 600), "Pong", sf::Style::Fullscreen);
    sf::Vector2u windowSize = window.getSize();
    sf::View view(sf::FloatRect(0, 0, (float) windowSize.x, (float) windowSize.y));
    window.setView(view);
    window.setFramerateLimit(140);

    // Create clock for keeping game time independent of frame rate
    sf::Clock clock;

    // Get font
    sf::Font font;
    if (!font.loadFromFile("pongfont.ttf")) {
        std::cout << "Error loading \"pongfont.ttf\"" << std::endl;
    }
    
    // Write title and score text
    int player1Score = 0;
    sf::Text player1ScoreText;
    player1ScoreText.setFont(font);
    player1ScoreText.setPosition(sf::Vector2f(windowSize.x * 0.5 - 250, 75));
    player1ScoreText.setString(std::to_wstring(player1Score));
    player1ScoreText.setCharacterSize(100);
    int player2Score = 0;
    sf::Text player2ScoreText;
    player2ScoreText.setFont(font);
    player2ScoreText.setPosition(sf::Vector2f(windowSize.x * 0.5 + 200, 75));
    player2ScoreText.setString(std::to_wstring(player2Score));
    player2ScoreText.setCharacterSize(100);

    // Create player 1
    sf::RectangleShape player1Sprite(sf::Vector2f(20, 200));
    player1Sprite.setPosition(sf::Vector2f(50, windowSize.y * 0.5 - player1Sprite.getSize().y * 0.5));
    Player player1(&player1Sprite, true);

    // Create player 2
    sf::RectangleShape player2Sprite(sf::Vector2f(20, 200));
    player2Sprite.setPosition(sf::Vector2f(windowSize.x - 75, windowSize.y * 0.5 - player2Sprite.getSize().y * 0.5));
    Player player2(&player2Sprite, true);

    // Create puck
    sf::CircleShape puckSprite(10);
    puckSprite.setPosition(windowSize.x * 0.5 - puckSprite.getRadius(), windowSize.y * 0.5 - puckSprite.getRadius());
    int randNum = rand() % 100;
    float direction = rand() % 600;
    if(randNum < 50) {
        direction += 1500;
    } else {
        direction = ((int) direction + 3300) % 3600;
    }
    direction /= 10;
    float xVelocity = 1.0f * cos(direction * M_PI / 180);
    float yVelocity = -1.0f * sin(direction * M_PI / 180);
    Puck puck(&puckSprite, sf::Vector2f(xVelocity, yVelocity));

    while(window.isOpen()) {

        // Get time elapsed
        sf::Time timeElapsed = clock.getElapsedTime();
        clock.restart().asSeconds();

        // Handle events by polling
        sf::Event event;
        while(window.pollEvent(event)) {
            switch (event.type) {
                // Close window when close button is pressed
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                        // Close window when escape button is pressed
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                    }
                    break;
            }
        }

        // Player 2 controls
        // Make sure player 2 is controllable before checking for player controls
        if(player1.isControllable()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player1.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player1.setVelocity(sf::Vector2f(1.0f, 270));
            }
        } else {
            if(puckSprite.getPosition().y < player1Sprite.getPosition().y + player1Sprite.getSize().y * 0.5 - puckSprite.getRadius()) {
                player1.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(puckSprite.getPosition().y > player1Sprite.getPosition().y + player1Sprite.getSize().y * 0.5 + puckSprite.getRadius()) {
                player1.setVelocity(sf::Vector2f(1.0f, 270));
            }
        }

        // Player 2 controls
        // Make sure player 2 is controllable before checking for player controls
        if(player2.isControllable()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                player2.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                player2.setVelocity(sf::Vector2f(1.0f, 270));
            }
        } else {
            if(puckSprite.getPosition().y < player2Sprite.getPosition().y + player2Sprite.getSize().y * 0.5 - puckSprite.getRadius()) {
                player2.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(puckSprite.getPosition().y > player2Sprite.getPosition().y + player2Sprite.getSize().y * 0.5 + puckSprite.getRadius()) {
                player2.setVelocity(sf::Vector2f(1.0f, 270));
            }
        }

        // Update players and puck sprites
        
        // Update paddles
        player1.update(timeElapsed.asMilliseconds());
        player2.update(timeElapsed.asMilliseconds());
        // Calculate puck's next position
        sf::Vector2f nextPuckPosition = puck.getNextPosition(timeElapsed.asMilliseconds());
        // Check for puck collisions
        sf::Vector2f puckPosAfterCollision = puck.detectCollision(player1Sprite, player2Sprite, nextPuckPosition);
        // Update puck's position to calculated next position
        puckSprite.setPosition(puckPosAfterCollision);
        // If there was a collision, play puck collision sound
        if(nextPuckPosition.x != puckPosAfterCollision.x || nextPuckPosition.y != puckPosAfterCollision.y) {
            puckCollisionSoundPitch = ((float) (rand() % 10)) / 10 + 0.5f;
            puckCollisionSound.setPitch(puckCollisionSoundPitch);
            puckCollisionSound.play();
        }
        // c = sqrt(a^2 + b^2 - 2ab * cos(theta))
        // c^2 = a^2 + b^2 - 2ab * cos(theta)
        // c^2 - a^2 - b^2 = 2ab * cos(theta)
        // (c^2 - a^2 - b^2) / 2ab = cos(theta)
        // arccos(c^2 - a^2 - b^2) = theta
        float magnitude = sqrt((long double) (puck.getVelocity().x * puck.getVelocity().x + puck.getVelocity().y * puck.getVelocity().y));
        magnitude += 0.00005f * timeElapsed.asMilliseconds();
        float direction = acos(puck.getVelocity().y / magnitude);
        float newVelocityX = magnitude * sin(direction) * (puck.getVelocity().x > 0 ? 1 : -1);
        float newVelocityY = magnitude * cos(direction);
        puck.setVelocity(sf::Vector2f(newVelocityX, newVelocityY));
        // Check for collision with left or right wall
        if(nextPuckPosition.x < 0 || nextPuckPosition.x > windowSize.x) {
            float direction = rand() % 60;
            if(puck.getVelocity().x < 0) {
                player2Score++;
                player2ScoreText.setString(std::to_wstring(player2Score));
                direction += 150;
            } else {
                player1Score++;
                player1ScoreText.setString(std::to_wstring(player1Score));
                direction += (int) (direction + 330) % 360;
            }
            puckSprite.setPosition(sf::Vector2f(windowSize.x * 0.5 - puckSprite.getRadius(), windowSize.y * 0.5 - puckSprite.getRadius()));
            float xVelocity = 1.0f * cos(direction * M_PI / 180);
            float yVelocity = 1.0f * sin(direction * M_PI / 180);
            puck.setVelocity(sf::Vector2f(xVelocity, yVelocity));
        }

        // Clear window and redraw all items after updates

        window.clear();

        window.draw(player1ScoreText);
        window.draw(player2ScoreText);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(puckSprite);

        window.display();
    }

}