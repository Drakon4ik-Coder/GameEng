// ship.cpp
#include "Ship.h"
#include "Bullet.h"
#include "GameEng.h"
using namespace sf;
using namespace std;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite()
{
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
};

void Ship::Update(const float &dt) {}

void Ship::Explode() {
	setTextureRect(IntRect(Vector2i(128, 32), Vector2i(32, 32)));
    _exploded = true;
}
bool Ship::is_exploded() const {
    return _exploded;
}

// Define the ship deconstructor.
// Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;

bool Invader::direction;
float Invader::speed;
Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir)
{
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
    speed = 10;
}

void Invader::Update(const float &dt)
{
    Ship::Update(dt);

    move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));
    if ((direction && getPosition().x > gameWidth - 16) ||
        (!direction && getPosition().x < 16))
    {
        direction = !direction;
        for (int i = 0; i < ships.size(); ++i)
        {
            if (typeid(*ships[i]) == typeid(Invader))
            {
                ships[i]->move(Vector2f(0.0f, 24.0f));
            }
        }
    }
    static float firetime = 0.0f;
    firetime -= dt;
    if (firetime <= 0 && rand() % 100 == 0) {
        Bullet::Fire(getOrigin()+getPosition(), true);
        firetime = 4.0f + (rand() % 60);
    }
}

Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32)))
{
    setPosition({gameWidth * .5f, gameHeight - 32.f});
    speed = 50;
    direction = 0;
    countdown=0;
}

void Player::Update(const float &dt)
{
    Ship::Update(dt);
    countdown+=dt;

    move(Vector2f(dt * direction * speed, 0.0f));
    direction = 0;
    if (Keyboard::isKeyPressed(controls[0]))
    {
        direction = -1;
    }
    if (Keyboard::isKeyPressed(controls[1]))
    {
        direction = 1;
    }

    speed = 50;
    if ((getPosition().x >= gameWidth - 16 && direction == 1) || (getPosition().x <= 16 && direction == -1))
    {
        speed = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::Space) && countdown > 1)
    {
        Bullet::Fire(getOrigin()+getPosition(), false);
        countdown = 0;
    }
}
