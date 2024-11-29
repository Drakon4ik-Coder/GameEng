//player.cpp
#include "Player.h"
using namespace sf;
using namespace std;

void Player::Update(double dt) {
  //Move in four directions based on keys
  if(Keyboard::isKeyPressed(Keyboard::W)) {
    _position.y-=_speed*dt;
  }
  if(Keyboard::isKeyPressed(Keyboard::S)) {
    _position.y+=_speed*dt;
  }
  if(Keyboard::isKeyPressed(Keyboard::A)) {
    _position.x-=_speed*dt;
  }
  if(Keyboard::isKeyPressed(Keyboard::D)) {
    _position.x+=_speed*dt;
  }
  
  Entity::Update(dt);
}

Player::Player()
    : _speed(200.0f), Entity(make_unique<CircleShape>(25.f)) {
  _shape->setFillColor(Color::Magenta);
  _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Player::Render(sf::RenderWindow &window) const {
  window.draw(*_shape);
}
