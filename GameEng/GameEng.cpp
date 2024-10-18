// GameEng.cpp : Defines the entry point for the application.
//

#include "GameEng.h"

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};
Vector2f ballVelocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float velocityMultiplier = 1.1f;
bool aiPlayer = false;
int score[2] = {0,0};

Text text;
Font font;
CircleShape ball;
RectangleShape paddles[2];

void Reset() {
    // reset paddle position
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));
    // reset Ball Position
    ball.setPosition(gameWidth/2, gameHeight/2);
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
    // Update Score Text
    text.setString(std::to_string(score[0])+":"+std::to_string(score[1]));
    
}

void Load() {
    // Set size and origin of paddles
    for (auto &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius/2,ballRadius/2); //Should be half the ball width and height
    // reset paddle position
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));
    // reset Ball Position
    ball.setPosition(gameWidth/2, gameHeight/2);
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
    // Load font-face from res dir
    font.loadFromFile("../../../../res/fonts/RobotoMono-Regular.ttf");
    // Set text element to use font
    text.setFont(font);
    // set the character size to 24 pixels
    text.setCharacterSize(24);
    text.setString(std::to_string(score[0])+":"+std::to_string(score[1]));
    // Keep Score Text Centered
    text.setPosition((gameWidth * .5f) - (text.getLocalBounds().width * .5f), 0);
}

void Update(RenderWindow &window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        aiPlayer = !aiPlayer;
    }

    // handle paddle movement
    float direction[2] = {0.0f,0.0f};
    if (Keyboard::isKeyPressed(controls[0])) {
        direction[0] = -1;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction[0] = 1;
    }
    if (aiPlayer) {
        direction[1] = ball.getPosition().y > paddles[1].getPosition().y ? 1 : -1;
    }
    else {
        if (Keyboard::isKeyPressed(controls[2])) {
            direction[1] = -1;
        }
        if (Keyboard::isKeyPressed(controls[3])) {
            direction[1] = 1;
        }      
    }
    if ((paddles[0].getPosition().y >= paddleSize.y / 2 || direction[0] > 0) && (paddles[0].getPosition().y <= gameHeight - paddleSize.y / 2 || direction[0] < 0)) {
        paddles[0].move(Vector2f(0.f, direction[0] * paddleSpeed * dt));
    }
    if ((paddles[1].getPosition().y >= paddleSize.y / 2 || direction[1] > 0) && (paddles[1].getPosition().y <= gameHeight - paddleSize.y / 2 || direction[1] < 0)) {
        paddles[1].move(Vector2f(0.f, direction[1] * paddleSpeed * dt));
    }
    ball.move(ballVelocity * dt);
    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) { //bottom wall
        // bottom wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, -10.f));
    } else if (by < 0) { //top wall
        // top wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, 10.f));
    } else if (bx > gameWidth) {
        // right wall
        score[0]++;
        Reset();
    } else if (bx < 0) {
        // left wall
        score[1]++;
        Reset();
    } else if (
	  //ball is inline or behind paddle AND
	  bx < paddleSize.x + paddleOffsetWall && 
	  //ball is below top edge of paddle AND
	  by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
	  //ball is above bottom edge of paddle
	  by < paddles[0].getPosition().y + (paddleSize.y * 0.5)) {
        // bounce off left paddle
        ballVelocity.x *= -velocityMultiplier;
        ballVelocity.y *= velocityMultiplier;
        ball.move(Vector2f(10.f, 0.f));
    } else if (
      //ball is inline or behind paddle AND
	  bx > gameWidth - paddleSize.x - paddleOffsetWall && 
	  //ball is below top edge of paddle AND
	  by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
	  //ball is above bottom edge of paddle
	  by < paddles[1].getPosition().y + (paddleSize.y * 0.5)) {
        // bounce off right paddle
        ballVelocity.x *= -velocityMultiplier;
        ballVelocity.y *= velocityMultiplier;
        ball.move(Vector2f(-10.f, 0.f));
    }   

}


void Render(RenderWindow &window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}

