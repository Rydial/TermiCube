

#include <string>
#include <vector>
#include <map>
#include <functional>


enum class ScreenType {
    MAINMENU, SETTINGS, GAME, INVENTORY
};


class Screen {

};

class GameWindow {
    private:
        std::vector<Screen> screenList;
        ScreenType screen;
    public:
        GameWindow() :
            screenList{},   screen{ScreenType::MAINMENU}
        {

        }

        void drawGraphics();
        void updateGame();
        void userInput();
};