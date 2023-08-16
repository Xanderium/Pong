#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Player.h"
#include "Puck.h"

sf::Vector2f findIntercept(sf::Vector2f line1Point1, sf::Vector2f line1Point2, sf::Vector2f line2Point1, sf::Vector2f line2Point2) {
    float denominator = ((line2Point2.y - line2Point1.y) * (line1Point2.x - line1Point1.x)) - ((line2Point2.x - line2Point1.x) * (line1Point2.y - line1Point1.y));
    if (denominator != 0) {
        float ua = (((line2Point2.x - line2Point1.x) * (line1Point1.y - line2Point1.y)) - ((line2Point2.y - line2Point1.y) * (line1Point1.x - line2Point1.x))) / denominator;
        if (ua >= 0 && ua <= 1) {
            float ub = (((line1Point2.x - line1Point1.x) * (line1Point1.y - line2Point1.y)) - ((line1Point2.y - line1Point1.y) * (line1Point1.x - line2Point1.x))) / denominator;
            if (ub >= 0 && ub <= 1) {
                float x = line1Point1.x + (ua * (line1Point2.x - line1Point1.x));
                float y = line1Point1.y + (ua * (line1Point2.y - line1Point1.y));
                return sf::Vector2f(x, y);
            }
        }
    }
    return sf::Vector2f(NULL, NULL);
}

sf::Vector3f findPuckPaddleIntercept(sf::CircleShape puck, sf::RectangleShape paddle, float deltaX, float deltaY) {
    sf::Vector2f intercept = sf::Vector2f(NULL, NULL);
    int interceptSide = -1;
    sf::Vector2f puckPos = puck.getPosition();
    sf::Vector2f newPuckPos = sf::Vector2f(puckPos.x + deltaX, puckPos.y + deltaY);
    if(deltaX < 0) {
        interceptSide = 1;
        intercept = findIntercept(puckPos, newPuckPos,
            sf::Vector2f(paddle.getPosition().x + paddle.getSize().x + puck.getRadius(), paddle.getPosition().y - puck.getRadius()),
            sf::Vector2f(paddle.getPosition().x + paddle.getSize().x + puck.getRadius(), paddle.getPosition().y + paddle.getSize().y + puck.getRadius()));
    }
    else if(deltaX > 0) {
        interceptSide = 3;
        intercept = findIntercept(puckPos, newPuckPos,
            sf::Vector2f(paddle.getPosition().x - puck.getRadius(), paddle.getPosition().y - puck.getRadius()),
            sf::Vector2f(paddle.getPosition().x - puck.getRadius(), paddle.getPosition().y + paddle.getSize().y + puck.getRadius()));
    }
    if(intercept.x == NULL) {
        if(deltaY < 0) {
            interceptSide = 2;
            intercept = findIntercept(puckPos, newPuckPos,
                sf::Vector2f(paddle.getPosition().x - puck.getRadius(), paddle.getPosition().y + paddle.getSize().y + puck.getRadius()),
                sf::Vector2f(paddle.getPosition().x + paddle.getSize().x + puck.getRadius(), paddle.getPosition().y + paddle.getSize().y + puck.getRadius()));
        }
        else if(deltaY > 0) {
            interceptSide = 0;
            intercept = findIntercept(puckPos, newPuckPos,
                sf::Vector2f(paddle.getPosition().x - puck.getRadius(), paddle.getPosition().y + puck.getRadius()),
                sf::Vector2f(paddle.getPosition().x + paddle.getSize().x + puck.getRadius(), paddle.getPosition().y + puck.getRadius()));
        }
    }
    sf::Vector3f puckPaddleIntercept = sf::Vector3f(intercept.x, intercept.y, interceptSide);
    return puckPaddleIntercept;
}

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

    // Create player 3
    sf::RectangleShape player3Sprite(sf::Vector2f(20, 200));
    player3Sprite.setPosition(sf::Vector2f(50, windowSize.y * 0.5 - player3Sprite.getSize().y * 0.5));
    Player player3(&player3Sprite, true);

    // Create player 4
    sf::RectangleShape player4Sprite(sf::Vector2f(20, 200));
    player4Sprite.setPosition(sf::Vector2f(windowSize.x - 75, windowSize.y * 0.5 - player4Sprite.getSize().y * 0.5));
    Player player4(&player4Sprite, false);

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
    Puck puck2(&puckSprite, sf::Vector2f(xVelocity, yVelocity));

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

        // Player 3 controls
        if(player3.isControllable()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player3.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player3.setVelocity(sf::Vector2f(1.0f, 270));
            }
        } else {
            if(puckSprite.getPosition().y < player3Sprite.getPosition().y + player3Sprite.getSize().y * 0.5) {
                player3.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(puckSprite.getPosition().y > player3Sprite.getPosition().y + player3Sprite.getSize().y * 0.5) {
                player3.setVelocity(sf::Vector2f(1.0f, 270));
            }
        }

        // Player 4 controls
        // Make sure player 4 is controllable before checking for player controls
        if(player4.isControllable()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                player4.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                player4.setVelocity(sf::Vector2f(1.0f, 270));
            }
        } else {
            if(puckSprite.getPosition().y < player4Sprite.getPosition().y - puckSprite.getRadius()) {
                player4.setVelocity(sf::Vector2f(1.0f, 90));
            } else if(puckSprite.getPosition().y > player4Sprite.getPosition().y + player4Sprite.getSize().y + puckSprite.getRadius()) {
                player4.setVelocity(sf::Vector2f(1.0f, 270));
            }
        }

        // Update players and puck sprites
        
        // Update paddles
        player3.update(timeElapsed.asMilliseconds());
        player4.update(timeElapsed.asMilliseconds());
        // Get puck's next position
        sf::Vector2f newPuckPosition;
        newPuckPosition.x = puckSprite.getPosition().x + puck2.getVelocity().x * timeElapsed.asMilliseconds();
        newPuckPosition.y = puckSprite.getPosition().y + puck2.getVelocity().y * timeElapsed.asMilliseconds();
        // Check for puck collisions
        // Check for collision with right side of left paddle
        if(newPuckPosition.x - puckSprite.getRadius() < player3Sprite.getPosition().x + player3Sprite.getSize().x
            && newPuckPosition.y + puckSprite.getRadius() > player3Sprite.getPosition().y
            && newPuckPosition.y - puckSprite.getRadius() < player3Sprite.getPosition().y + player3Sprite.getSize().y) {
            float collisionTime = (player3Sprite.getPosition().x + player3Sprite.getPosition().x - (puckSprite.getPosition().x - puckSprite.getRadius())) /
                (newPuckPosition.x - puckSprite.getRadius() - (puckSprite.getPosition().x - puckSprite.getRadius()));
            float xAtCollision = puckSprite.getPosition().x + ((1 - collisionTime) * puckSprite.getPosition().x + collisionTime * newPuckPosition.x);
            float xAtCompletion = xAtCollision + (newPuckPosition.x - xAtCollision);
            newPuckPosition.x = xAtCompletion;
            puck2.setVelocity(sf::Vector2f(-puck2.getVelocity().x, puck2.getVelocity().y));

            // Play puck collision sound
            puckCollisionSoundPitch = ((float) (rand() % 10)) / 10 + 0.5f;
            puckCollisionSound.setPitch(puckCollisionSoundPitch);
            puckCollisionSound.play();
        // Check for collision with left side of right paddle
        } else if(newPuckPosition.x + puckSprite.getRadius() > player4Sprite.getPosition().x
            && newPuckPosition.y + puckSprite.getRadius() > player4Sprite.getPosition().y
            && newPuckPosition.y - puckSprite.getRadius() < player4Sprite.getPosition().y + player4Sprite.getSize().y) {
            float collisionTime = (player4Sprite.getPosition().x - (puckSprite.getPosition().x + puckSprite.getRadius())) /
                (newPuckPosition.x + puckSprite.getRadius() - (puckSprite.getPosition().x + puckSprite.getRadius()));
            float xAtCollision = ((1 - collisionTime) * puckSprite.getPosition().x + collisionTime * newPuckPosition.x);
            float xAtCompletion = xAtCollision - (newPuckPosition.x - xAtCollision);
            newPuckPosition.x = xAtCompletion;
            std::cout << std::endl << "Pre-collision Velocity: (" << puck2.getVelocity().x << ", " << puck2.getVelocity().y << ")" << std::endl;
            puck2.setVelocity(sf::Vector2f(-puck2.getVelocity().x, puck2.getVelocity().y));
            std::cout << "Post-collision Velocity: (" << puck2.getVelocity().x << ", " << puck2.getVelocity().y << ")" << std::endl;

            // Play puck collision sound
            puckCollisionSoundPitch = ((float) (rand() % 10)) / 10 + 0.5f;
            puckCollisionSound.setPitch(puckCollisionSoundPitch);
            puckCollisionSound.play();
        }
        // Check for collision with top wall
        if(newPuckPosition.y - puckSprite.getRadius() < 0) {
            float collisionTime = -(puckSprite.getPosition().y - puckSprite.getRadius()) /
                (newPuckPosition.y - puckSprite.getRadius() - (puckSprite.getPosition().y - puckSprite.getRadius()));
            float yAtCollision = puckSprite.getPosition().y + ((1 - collisionTime) * puckSprite.getPosition().y + collisionTime * newPuckPosition.y);
            float yAtCompletion = yAtCollision + (newPuckPosition.y - yAtCollision);
            newPuckPosition.y = yAtCompletion;
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));

            // Play puck collision sound
            puckCollisionSoundPitch = ((float) (rand() % 10)) / 10 + 0.5f;
            puckCollisionSound.setPitch(puckCollisionSoundPitch);
            puckCollisionSound.play();
        // Check for collision with bottom wall
        } else if(newPuckPosition.y > windowSize.y) {
            float collisionTime = (windowSize.y - (puckSprite.getPosition().y + puckSprite.getRadius())) /
                (newPuckPosition.y + puckSprite.getRadius() - (puckSprite.getPosition().y + puckSprite.getRadius()));
            float yAtCollision = ((1 - collisionTime) * (puckSprite.getPosition().y + puckSprite.getRadius()) + collisionTime * (newPuckPosition.y + puckSprite.getRadius()));
            float yAtCompletion = yAtCollision - (newPuckPosition.y - yAtCollision);
            newPuckPosition.y = yAtCompletion;
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));

            // Play puck collision sound
            puckCollisionSoundPitch = ((float) (rand() % 10)) / 10 + 0.5f;
            puckCollisionSound.setPitch(puckCollisionSoundPitch);
            puckCollisionSound.play();
        }
        // c = sqrt(a^2 + b^2 - 2ab * cos(theta))
        // c^2 = a^2 + b^2 - 2ab * cos(theta)
        // c^2 - a^2 - b^2 = 2ab * cos(theta)
        // (c^2 - a^2 - b^2) / 2ab = cos(theta)
        // arccos(c^2 - a^2 - b^2) = theta
        puckSprite.setPosition(newPuckPosition);
        float magnitude = sqrt((long double) (puck2.getVelocity().x * puck2.getVelocity().x + puck2.getVelocity().y * puck2.getVelocity().y));
        magnitude += 0.00005f * timeElapsed.asMilliseconds();
        float direction = acos(puck2.getVelocity().y / magnitude);
        float newVelocityX = magnitude * sin(direction) * (puck2.getVelocity().x > 0 ? 1 : -1);
        float newVelocityY = magnitude * cos(direction);
        puck2.setVelocity(sf::Vector2f(newVelocityX, newVelocityY));
        // Check for collision with left or right wall
        if(newPuckPosition.x < 0 || newPuckPosition.x > windowSize.x) {
            float direction = rand() % 60;
            if(puck2.getVelocity().x < 0) {
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
            puck2.setVelocity(sf::Vector2f(xVelocity, yVelocity));
        }

        // Update players and puck sprites
        sf::Vector2f player3Pos = player3Sprite.getPosition();
        sf::Vector2f player4Pos = player4Sprite.getPosition();
        sf::Vector2f puckPos = puckSprite.getPosition();
        // player3.update(timeElapsed.asMilliseconds());
        // player4.update(timeElapsed.asMilliseconds());
        // sf::Vector2f puckDelta = puck2.update(timeElapsed.asMilliseconds());

        // Clear window and redraw all items after updates

        window.clear();

        window.draw(player1ScoreText);
        window.draw(player2ScoreText);
        window.draw(player3Sprite);
        window.draw(player4Sprite);
        window.draw(puckSprite);

        window.display();
    }

}