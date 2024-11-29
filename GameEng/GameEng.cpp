// GameEng.cpp : Defines the entry point for the application.
//

#include "Player.h"
#include "GameEng.h"
#include "LevelSystem.h"

std::unique_ptr<Player> player;

void Load()
{
    player = std::make_unique<Player>();
    ls::loadLevelFile("res/levels/maze.txt");

    // Print the level to the console
    for (size_t y = 0; y < ls::getHeight(); ++y) {
        for (size_t x = 0; x < ls::getWidth(); ++x) {
        cout << ls::getTile({x, y});
        }
        cout << endl;
    }
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

    (*player).Update(dt);
}
void Render(RenderWindow &window)
{
    ls::Render(window);
    (*player).Render(window);
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
