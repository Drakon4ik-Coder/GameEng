// bullet.cpp
#include "Bullet.h"
#include "Ship.h"
#include "GameEng.h"
using namespace sf;
using namespace std;

unsigned char Bullet::bulletPointer = 0; // Define static member
Bullet Bullet::bullets[256];            // Define the static array
Bullet::Bullet() : _mode(false) {}

void Bullet::Update(const float &dt)
{
    for(int i = 0; i< 256; i++)
    {
        bullets[i]._Update(dt);
    }
}

void Bullet::Render(sf::RenderWindow &window)
{
    for(int i = 0; i< 256; i++)
    {
        window.draw(bullets[i]);
    }
}
void Bullet::Fire(const sf::Vector2f &pos, const bool mode)
{
    Bullet* bullet = &bullets[bulletPointer];
    bullet->setPosition(pos);
    bullet->_mode = mode;
    IntRect _sprite;
    if (mode)
    {
        _sprite = IntRect(Vector2i(32, 32), Vector2i(32, 32));
    }
    else
    {
        _sprite = IntRect(Vector2i(64, 32), Vector2i(32, 32));
    }
    bullet->setTextureRect(_sprite);
    bulletPointer++;
    if(bulletPointer == 256) {
        bulletPointer=0;
    }
}
void Bullet::Init()
{
    bulletPointer = 0;
    for (int i = 0; i < 256; i++)
    {
        Bullet* bullet = &bullets[i];
        bullet->setTexture(spritesheet);
        bullet->setOrigin(16.f, 16.f);
        bullet->setPosition(Vector2f{-100, -100});
    }
}

void Bullet::_Update(const float &dt) {
    if (getPosition().y < -32 || getPosition().y > gameHeight + 32) {
        //off screen - do nothing
        return;
    } else {
        move(Vector2f(0, dt * 200.0f * (_mode ? 1.0f : -1.0f)));
        const FloatRect boundingBox = getGlobalBounds();
        
        for (auto s : ships) {
            if (!_mode && typeid(*s) != typeid(Invader)) {
                //player bulelts don't collide with player
                continue;
            }
            if (_mode && typeid(*s) != typeid(Player)) {
                //invader bullets don't collide with other invaders
                continue;
            }
            if (!s->is_exploded() && 
                s->getGlobalBounds().intersects(boundingBox)) {
                  //Explode the ship
                  s->Explode();
                  //warp bullet off-screen
				  setPosition(Vector2f(-100, -100));
                  return;
            }
        }
    }
};