#ifndef GAMESCREEN_H
#define GAMESCREEN_H


#include <vector>
#include <string>
#include <array>
#include <fstream>

/////////////////////////////* Forward Declarations */////////////////////////////

namespace entt {

    enum class entity : uint32_t;

}

///////////////////////////////////////////////////////////////////////////////////

namespace TC {

    /* Forward Declarations */
    namespace EC {

        struct Position;
        struct Velocity;

    }

    /* Type Alias */
    using EID = entt::entity;
    using Pos = EC::Position;
    using Vel = EC::Velocity;
    using Map = std::vector<std::vector<std::vector<EID>>>;


    class GameScreen : public Screen {
        private:
            /* Private Member Constants */
            static constexpr size_t spriteWidth {2};
            static constexpr Size<> mainSize {27, 82};
            static constexpr Size<> statBarSize {1, 26};
            static constexpr Size<> hotbarSize {10, 26};
            static constexpr Size<> consoleSize {12, 55};
            /* Private Member Enums */
            enum class SubWindowType {
                MAIN, STATBAR, HOTBAR, CONSOLE, COUNT
            };
            enum class ScreenFocus {
                MAIN, OPTIONS, CONSOLE
            };
            enum class CursorMove : int {
                LEFT = -1, RIGHT = 1
            };
            /* Private Member Structs */
            struct GameInfo {
                Size<> mapSize;
                size_t numOfLvls;
                size_t hp;
                size_t hotbarSlot;
                size_t curLvl;
            };

            struct Console {
                /* Public Member Constants */
                static constexpr Size<> size[2] {{12, 55}, {0, 0}};
                /* Public Member Enums */
                enum class Format {
                    NORMAL, TERMINAL
                } fmt;

                enum class Mode {
                    INTEGRATED, POPUP, COUNT
                } mode;
                /* Public Member Structs */
                struct Input {
                    std::string line;
                    size_t cursIndex; // Relative to line
                    size_t cursPos; // Relative to console
                    int highlight;
                } input;
                /* Public Member Variables */
                std::vector<std::string> record;
                std::ofstream file;
            };

            struct OptionMenu {
                /* Public Member Enums */
                enum class ButtonType {
                    RESUME, SETTINGS, MAINMENU, COUNT
                };
                /* Public Constants */
                static constexpr Size<> size {27, 82};
                static constexpr Size<> btnSize {5, 48};
                static constexpr Point<> btnStartPos {
                    ((maxRows - Console::size[0].y - 3) - (static_cast<size_t>(
                        ButtonType::COUNT) * btnSize.y)) / (static_cast<size_t>(
                            ButtonType::COUNT) + 1) + 1,
                    (maxCols - btnSize.x) / 2};
                /* Public Methods */
                std::function<void()> genClickFunc(WinSData &winSData, size_t index);
                /* Public Member Variables */
                std::unique_ptr<PANEL, PanelDel> panel;
                std::function<void()> resetFocus;
                BtnMgr btns;
            };
            /* Private Member Variables */
            std::vector<std::unique_ptr<WINDOW, WinDel>> subWins;
            std::array<std::string, 10> hotbar;
            Console cnsl;
            OptionMenu optMenu;
            ScreenFocus focus;
            GameInfo info;
            EID player;
            Map map;
            /* Private Constructor */
            GameScreen(PANEL *panel, WinSData &winSData);
            /* Private Methods */
            void consoleInput(int key);
            void drawMap();
            void drawStatBar();
            void gameInput(int key);
            void hotbarSelect(size_t slot);
            void initConsole();
            void initEntities();
            void initOptionMenu();
            void initScreen();
            void initSubWindows();
            void moveCursor(int side, bool highlight=false);
            void sendToConsole(std::string line); 
            void updateConsole();
            bool withinBoundary(const Pos &pos, const Vel &vel);
        public:
            /* Public Constructor */
            GameScreen(WinSData &winSData);
            /* Public Methods */
            void drawGraphics();
            void updateScreen();
            void userInput(int key);
    };
    /* Type Alias */
    using GScr = GameScreen;

}


#endif // GAMESCREEN_H
