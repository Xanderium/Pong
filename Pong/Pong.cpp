#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "SFML/Graphics.hpp"
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
    float direction = rand() % 60;
    if(randNum < 50) {
        direction += 150;
    } else {
        direction += (int) (direction + 330) % 360;
    }
    float xVelocity = 1.0f * cos(direction * M_PI / 180);
    float yVelocity = 1.0f * sin(direction * M_PI / 180);
    Puck puck2(&puckSprite, sf::Vector2f(xVelocity, yVelocity));

    // Create Player 1
    sf::RectangleShape player1(sf::Vector2f(20, 200));
    player1.setFillColor(sf::Color::Black);
    player1.setPosition(sf::Vector2f(50, windowSize.y * 0.5 - player1.getSize().y * 0.5));

    // Create Player 2
    sf::RectangleShape player2(sf::Vector2f(20, 200));
    player2.setFillColor(sf::Color::Black);
    player2.setPosition(sf::Vector2f(windowSize.x - 75, windowSize.y * 0.5 - player2.getSize().y * 0.5));

    // Create puck
    sf::CircleShape puck(10);
    puck.setFillColor(sf::Color::Black);
    puck.setPointCount(50);
    puck.setPosition(sf::Vector2f(windowSize.x * 0.5 - 25, windowSize.y * 0.5 - puck.getRadius() * 0.5));
    sf::Vector2f puckVelocity = sf::Vector2f(-0.75f, -0.1f);

    while (window.isOpen()) {

        // Get time elapsed
        sf::Time timeElapsed = clock.getElapsedTime();
        clock.restart().asSeconds();

        // Move puck
        sf::Vector2f puckMoveDelta = sf::Vector2f(puckVelocity.x * timeElapsed.asMilliseconds(), puckVelocity.y * timeElapsed.asMilliseconds());
        puck.move(puckMoveDelta);
        puckVelocity = sf::Vector2f(puckVelocity.x + (puckVelocity.x < 0 ? -1 : 1) * 0.00005f * timeElapsed.asMilliseconds(), puckVelocity.y + (puckVelocity.y < 0 ? -1 : 1) * 0.00005f * timeElapsed.asMilliseconds());
        // std::cout << puckVelocity.x << std::endl;
        // Detect collisions between the puck and the paddle
        // sf::Vector3f puckPaddleIntercept = findPuckPaddleIntercept(puck, (puckMoveDelta.x < 0 ? player1 : player2), puckMoveDelta.x, puckMoveDelta.y);
        sf::Vector3f puckPaddleIntercept;
        if(puckPaddleIntercept.x != NULL) {
            switch ((int) puckPaddleIntercept.z) {
                case 0:
                case 2:
                    puck.setPosition(sf::Vector2f(puck.getPosition().x, puckPaddleIntercept.y));
                    puckVelocity.y *= -1;
                    break;
                case 3:
                case 1:
                    puck.setPosition(sf::Vector2f(puckPaddleIntercept.x, puck.getPosition().y));
                    puckVelocity.x *= -1;
                    break;
            }
        }
        /* if (puck.getGlobalBounds().left < player1.getGlobalBounds().left + player1.getGlobalBounds().width) {
            puckVelocity.x *= -1;
            if(puck.getPosition().y + puck.getRadius() * 0.5 < player1.getPosition().y + player1.getSize().y * 0.5 && puckVelocity.y > 0)
            puckVelocity.y *= -1;
        } */
        // Detect if puck goes out of bounds
        if(puck.getPosition().x - puck.getRadius() < -25 || puck.getPosition().x + puck.getRadius() > window.getSize().x - 25) {
            puck.setPosition(sf::Vector2f(windowSize.x * 0.5 - puck.getRadius() * 0.5 - 25, windowSize.y * 0.5 - puck.getRadius() * 0.5));
            if(puckVelocity.x < 0) {
                // player2Score++;
                player2ScoreText.setString(std::to_wstring(player2Score));
                puckVelocity.x = -0.75f;
            }
            else {
                player1Score++;
                player1ScoreText.setString(std::to_wstring(player1Score));
                puckVelocity.x = 0.75f;
            }
            puckVelocity.y = -0.1f;
        // Detect collisions between the puck and the walls
        } else if(puck.getPosition().y - puck.getRadius() < 0 || puck.getPosition().y + puck.getRadius() > window.getSize().y) {
            puckVelocity.y *= -1;
        }

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
                        // Update player 1's velocity if any control keys are pressed
                        /* case sf::Keyboard::W:
                        case sf::Keyboard::A:
                            player3.setVelocity(sf::Vector2f(1.0f, 90));
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::D:
                            player3.setVelocity(sf::Vector2f(1.0f, 270));
                            break;
                        // Update player 2's velocity if any control keys are pressed and player is controllable
                            case sf::Keyboard::Up:
                            case sf::Keyboard::Left:
                                break;
                            case sf::Keyboard::Down:
                            case sf::Keyboard::Right:
                                break; */
                    }
                    break;
                /* case sf::Event::KeyReleased:
                    switch(event.key.code) {
                        // Stop player 1's movements if any if any control keys are released
                        case sf::Keyboard::W:
                        case sf::Keyboard::A:
                            player3.setVelocity(sf::Vector2f(0, 0));
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::D:
                            player3.setVelocity(sf::Vector2f(0, 0));
                            break;
                    }
                    break; */
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

        // Player 1 controls
        /* sf::Vector2f player1Pos = player1.getPosition();
        // If 'W' or 'A' are being pressed, move player one upward
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            float moveDelta = -timeElapsed.asMilliseconds();
            // If player reaches top of window view, prevent movement beyond
            if(player1Pos.y + moveDelta < 0) {
                moveDelta = player1Pos.y;
            }
            player1.move(sf::Vector2f(0, moveDelta));
            if (puckPaddleIntercept.x != NULL && puckVelocity.x < 0) {
                puckVelocity.y *= puckVelocity.y < 0 ? 0.5 : 1.5;
            }
        // If 'S' or 'D' are being pressed, move player one downward
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            float moveDelta = timeElapsed.asMilliseconds();
            // If player reaches bottom of window view, prevent movement beyond
            if (player1Pos.y + player1.getSize().y + moveDelta > windowSize.y) {
                moveDelta = windowSize.y - player1Pos.y - player1.getSize().y;
            }
            player1.move(sf::Vector2f(0, moveDelta));
            if(puckPaddleIntercept.x != NULL && puckVelocity.x < 0) {
                puckVelocity.y *= puckVelocity.y > 0 ? 0.5 : 1.5;
            }
        } */

        // Player 2 controls
        /* sf::Vector2f player2Pos = player2.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            float moveDelta = -timeElapsed.asMilliseconds();
            // If player reaches top of window view, prevent movement beyond
            if (player2Pos.y + moveDelta < 0) {
                moveDelta = player2Pos.y;
            }
            player2.move(sf::Vector2f(0, moveDelta));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            float moveDelta = timeElapsed.asMilliseconds();
            // If player reaches bottom of window view, prevent movement beyond
            if (player2Pos.y + player2.getSize().y + moveDelta > windowSize.y) {
                moveDelta = windowSize.y - player2Pos.y - player2.getSize().y;
            }
            player2.move(sf::Vector2f(0, moveDelta));
        } */

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
        // Check for collision with left side of right paddle
        } else if(newPuckPosition.x + puckSprite.getRadius() > player4Sprite.getPosition().x
            && newPuckPosition.y + puckSprite.getRadius() > player4Sprite.getPosition().y
            && newPuckPosition.y - puckSprite.getRadius() < player4Sprite.getPosition().y + player4Sprite.getSize().y) {
            float collisionTime = (player4Sprite.getPosition().x - (puckSprite.getPosition().x + puckSprite.getRadius())) /
                (newPuckPosition.x + puckSprite.getRadius() - (puckSprite.getPosition().x + puckSprite.getRadius()));
            float xAtCollision = ((1 - collisionTime) * puckSprite.getPosition().x + collisionTime * newPuckPosition.x);
            float xAtCompletion = xAtCollision - (newPuckPosition.x - xAtCollision);
            newPuckPosition.x = xAtCompletion;
            puck2.setVelocity(sf::Vector2f(-puck2.getVelocity().x, puck2.getVelocity().y));
        }
        // Check for collision with top wall
        if(newPuckPosition.y - puckSprite.getRadius() < 0) {
            float collisionTime = -(puckSprite.getPosition().y - puckSprite.getRadius()) /
                (newPuckPosition.y - puckSprite.getRadius() - (puckSprite.getPosition().y - puckSprite.getRadius()));
            float yAtCollision = puckSprite.getPosition().y + ((1 - collisionTime) * puckSprite.getPosition().y + collisionTime * newPuckPosition.y);
            float yAtCompletion = yAtCollision + (newPuckPosition.y - yAtCollision);
            newPuckPosition.y = yAtCompletion;
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));
        // Check for collision with bottom wall
        } else if(newPuckPosition.y + puckSprite.getRadius() > windowSize.y) {
            float collisionTime = (windowSize.y - (puckSprite.getPosition().y + puckSprite.getRadius())) /
                (newPuckPosition.y + puckSprite.getRadius() - (puckSprite.getPosition().y + puckSprite.getRadius()));
            float yAtCollision = ((1 - collisionTime) * (puckSprite.getPosition().y + puckSprite.getRadius()) + collisionTime * (newPuckPosition.y + puckSprite.getRadius()));
            float yAtCompletion = yAtCollision - (newPuckPosition.y - yAtCollision);
            newPuckPosition.y = yAtCompletion;
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));
        }
        puckSprite.setPosition(newPuckPosition);
        float magnitude = sqrt(puck2.getVelocity().x * puck2.getVelocity().x + puck2.getVelocity().y * puck2.getVelocity().y);
        std::cout << "Magnitude: " << magnitude << std::endl;
        magnitude += 0.00005f * timeElapsed.asMilliseconds();
        float direction = acos(puck2.getVelocity().y / magnitude);
        std::cout << "Direction: " << direction << std::endl;
        float newVelocityX = magnitude * sin(direction);
        float newVelocityY = magnitude * cos(direction);
        std::cout << "New Velocity: (" << newVelocityX << ", " << newVelocityY << ")";
        // puck2.setVelocity(sf::Vector2f(newVelocityX, newVelocityY));
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

        // Detect collission between puck and top and bottom walls
        /* if (puckSprite.getPosition().y - puckSprite.getRadius() < 0) {
            puckSprite.setPosition(sf::Vector2f(puckSprite.getPosition().x, puckSprite.getRadius()));
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));
        } else if(puckSprite.getPosition().y + puckSprite.getRadius() > 1080) {
            puckSprite.setPosition(sf::Vector2f(puckSprite.getPosition().x, 1080 - puckSprite.getRadius()));
            puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, puck2.getVelocity().y - 2 * (puck2.getVelocity().y - 180)));
        } */
        
        // Detect collission between puck and player paddles
        /* sf::Vector3f intercept;
        if(puck2.getVelocity().y > 90 && puck2.getVelocity().y < 270) {
            // intercept = findPuckPaddleIntercept(puckSprite, player3Sprite, puckDelta.x, puckDelta.y);
        } else {
            // intercept = findPuckPaddleIntercept(puckSprite, player4Sprite, puckDelta.x, puckDelta.y);
        }
        if(intercept.x != NULL) {
            puckSprite.setPosition(sf::Vector2f(intercept.x, intercept.y));
            switch((int) intercept.z) {
                case 0:
                    puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, puck2.getVelocity().y - 2 * (puck2.getVelocity().y - 180)));
                    break;
                case 1:
                    puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, puck2.getVelocity().y - 2 * (puck2.getVelocity().y - 90)));
                    break;
                case 2:
                    puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, -puck2.getVelocity().y));
                    break;
                case 3:
                    puck2.setVelocity(sf::Vector2f(puck2.getVelocity().x, puck2.getVelocity().y - 2 * (puck2.getVelocity().y - 270)));
                    break;
            }
        } */

        // Detect if a player scores a point
        /* if (puckSprite.getPosition().x - puckSprite.getRadius() < 0 || puckSprite.getPosition().x + puckSprite.getRadius() > windowSize.x) {
            puckSprite.setPosition(sf::Vector2f(windowSize.x * 0.5 - puckSprite.getRadius(), windowSize.y * 0.5 - puckSprite.getRadius()));
            direction = rand() % 30;
            if(puck2.getVelocity().y < 90 || puck2.getVelocity().y > 270) {
                player1Score++;
                player1ScoreText.setString(std::to_wstring(player1Score));
                direction += 165;
            } else {
                player2Score++;
                player2ScoreText.setString(std::to_wstring(player2Score));
                direction = (int) (direction + 345) % 360;
            }
            puck2.setVelocity(sf::Vector2f(0.75f, direction));
        } */

        // Clear window and redraw all items after updates

        window.clear();

        window.draw(player1ScoreText);
        window.draw(player2ScoreText);
        window.draw(player1);
        window.draw(player2);
        window.draw(player3Sprite);
        window.draw(player4Sprite);
        window.draw(puckSprite);
        window.draw(puck);

        window.display();
    }

}