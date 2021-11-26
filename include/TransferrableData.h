#ifndef TRANSFERRABLEDATA_H
#define TRANSFERRABLEDATA_H


#include <vector>
#include <memory>


namespace TC {

    /* Forward Declarations */
    class Screen;


    /* Type Aliases */
    using Scr = Screen;


    struct WindowData {
        /* Public Member Variables */
        bool exit;
        size_t screen;
        std::vector<std::unique_ptr<Scr>> screenList;
    };
    /* Type Alias */
    using WinData = WindowData;


    class WindowSharedData {
        private:
            /* Weak Ptr instead of Shared Ptr to prevent self-referencing */
            /* Private Member Variables */
            std::weak_ptr<WinData> data;
        public:
            /* Public Constructor */
            WindowSharedData(std::shared_ptr<WinData> &winData);
            /* Public Methods */
            void switchScreen(size_t index);
            void terminate();
    };
    /* Type Alias */
    using WinSData = WindowSharedData;

}


#endif // TRANSFERRABLEDATA_H