// GameEng.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;
class Ship;

constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;
constexpr uint16_t invaders_rows = 5;
constexpr uint16_t invaders_columns = 12;

extern sf::Texture spritesheet;
extern std::vector<Ship *> ships;

const Keyboard::Key controls[2] = {
    Keyboard::Left,   // Player Left
    Keyboard::Right   // Player Right
};