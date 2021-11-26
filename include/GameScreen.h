#ifndef GAMESCREEN_H
#define GAMESCREEN_H


#include <vector>
#include <string>
#include <array>
#include <fstream>


namespace TC {

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
            struct Player {
                size_t hp;
                size_t curHotbarSlot;
            };
            struct Console {
                /* Public Member Constants */
                static constexpr Size<> size[2] {{12, 55}, {0, 0}};
                /* Public Member Enums */
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
                std::vector<std::pair<std::string, std::wstring>> record;
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
                std::function<void()> genClickFunc(WinSData &winSData, int index);
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
            Player p;
            ScreenFocus focus;
            /* Private Methods */
            void consoleInput(int key);
            void drawStatBar();
            void hotbarSelect(size_t slot);
            void initConsole();
            void initOptionMenu();
            void initScreen();
            void initSubWindows();
            void moveCursor(int side, bool highlight=false);
            void sendToConsole(std::string line, const std::wstring &icon); 
            void updateConsole();
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
