// GameEng.cpp : Defines the entry point for the application.
//

#include "Ship.h"
#include "Bullet.h"
#include "GameEng.h"

sf::Texture spritesheet;
std::vector<Ship *> ships;

void Load()
{
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png"))
    {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    int offsetX = (gameWidth-32*invaders_columns)/2;
    for (int r = 0; r < invaders_rows; ++r)
    {
        auto rect = IntRect(Vector2i(0, 0), Vector2i(32, 32));
        for (int c = 0; c < invaders_columns; ++c)
        {
            Vector2f position = {c * 32 + offsetX, r * 32 + 16};
            auto inv = new Invader(rect, position);
            ships.push_back(inv);
        }
    }
    auto player = new Player();
    ships.push_back(player);
    Bullet::Init();
}
void Update(RenderWindow &window)
{
    static Clock clock;
    float dt = clock.restart().asSeconds();
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
            return;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    for (auto &s : ships)
    {
        s->Update(dt);
    }
    Bullet::Update(dt);
}
void Render(RenderWindow &window)
{
    for (const auto s : ships)
    {
        window.draw(*s);
    }
    Bullet::Render(window);
}

int main()
{
    RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
    Load();
    while (window.isOpen())
    {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
