#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <string>
#include <vector>
#include <map>
#include <functional>


/* Forward Declarations */


/* Type Aliases */
using Commands = std::map<std::string, std::function<void()>>;


/* Enumerated Types */
enum class GameState : int {
    END, START
};


/* Structures */
struct Coords {
    int x;
    int y;
    /* Methods */
    Coords(int x=0, int y=0);
    Coords &operator=(const Coords &a);
    Coords &operator+=(const Coords &a);
    Coords operator+(const Coords &a) const;
};

struct Map {
    std::vector<std::vector<int>> floor;
    size_t size;
};

namespace ent { // Entities

    struct Entity {
        std::string emoji;  // Emoji
        Coords pos;         // Position
        /* Methods */
        Entity(std::string emoji, int x, int y);
    };

    struct Player : Entity {
        Coords vel; // Velocity
        /* Methods */
        Player(int x, int y);
    };

    struct Tree : Entity {
        
    };

}


/* Classes */
class Game {
    private:
        Commands cmds;      // Commands
        GameState state;    // Game State
        Map map;            // Map
        std::string msg;    // Message 
        ent::Player p1;     // Player       
        /* Methods */
        void generateCommands();
        void generateMap();
    public:
        Game(size_t mapSize=255);
        void drawGraphics();
        GameState getState() {return state;}
        void updateGame();
        void userInput();
};

#endif // TERMICUBE_H