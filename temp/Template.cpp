#include <iostream>
#include "Template.h"


/* Comstants */
constexpr int screenSize {17};
constexpr int windowLength {(screenSize * 2) + 1};


/*########################## ENTITIES ##########################*/

namespace ent {

    Entity::Entity(std::string emoji, int x, int y) :
        emoji{emoji},   pos{x, y}
    {
        
    }

    Player::Player(int x, int y) :
        Entity("\U00002B99", x, y), vel{0, 0}
    {
        std::cout << x << " " << y << '\n';
    }

}


/*########################## STRUCT [COORDS] ##########################*/

/* Public Methods */
Coords::Coords(int x, int y) : x{x}, y{y}
{
    
}

Coords & Coords::operator=(const Coords &a)
{
    x = a.x;
    y = a.y;
    return *this;
}

Coords & Coords::operator+=(const Coords &a)
{
    x += a.x;
    y += a.y;
    return *this;
}

Coords Coords::operator+(const Coords &a) const
{
    return Coords{x + a.x, y + a.y};
}


/*########################## CLASS [GAME] ##########################*/

/* Private Methods */
void Game::generateCommands()
{
    cmds.emplace("exit", [this] () {state = GameState::END;});
    cmds.emplace("w", [this] () {p1.vel.y = 1;});
    cmds.emplace("s", [this] () {p1.vel.y = -1;});
    cmds.emplace("a", [this] () {p1.vel.x = -1;});
    cmds.emplace("d", [this] () {p1.vel.x = 1;});
}

void Game::generateMap()
{
    // Add Player First
    for (size_t i {0}; i < map.size; ++i) {
        map.floor.emplace_back();

        for (size_t j {0}; j < map.size; ++j)
            map.floor[i].emplace_back(0);
    } 
}

/* Public Methods */
Game::Game(size_t mapSize) :
    cmds{},             state{GameState::START},
    map{{}, mapSize},   msg{},
    p1{static_cast<int>(mapSize) / 2, static_cast<int>(mapSize) / 2}
{
    generateCommands();
    generateMap();
    // Graphics need to be called at least once
    drawGraphics();
}

void Game::drawGraphics()
{
    // Clear Game Screen
    system("clear");

    // Print Any Game Messages
    std::cerr << msg;

    /* Draw Top of Game Window */
    std::cout << "\U00002554";
    
    for (size_t i {0}; i < windowLength; i++)
        std::cout << "\U00002550";
    
    std::cout << "\U00002557\n";

    /* Draw Middle of Game Window */
    for (size_t i {0}; i < screenSize; i++) {
        std::cout << "\U00002551";

        for (size_t j {0}; j < screenSize; j++) {
            if (map.floor[i][j] == 0)
                std::cout << "  ";
            else // Non-empty squares
                std::cout << "  ";
        }

        std::cout << " \U00002551\n";
    }

    /* Draw Bottom of Game Window */
    std::cout << "\U0000255A";
    
    for (size_t i {0}; i < windowLength; i++)
        std::cout << "\U00002550";
    
    std::cout << "\U0000255D\n";
}

void Game::updateGame()
{
    // Reset Player Velocity
    p1.vel = {0, 0};
}

void Game::userInput()
{     
    std::cout << "\nCommand: ";
    std::string input;
    std::getline(std::cin, input);

    try {
        cmds[input]();
        msg.clear();
    } catch(const std::bad_function_call& e) {
        msg = "\nInvalid Command.\n\n";
    }
}
