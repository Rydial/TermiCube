#ifndef TRANSFERRABLEDATA_H
#define TRANSFERRABLEDATA_H


#include <vector>
#include <memory>

/////////////////////// Forward Declarations ///////////////////////

class Screen;

/////////////////////// Transfer Data Wrappers ///////////////////////

struct TCWindowData {
    /* Public Member Variables */
    bool exit;
    size_t screen;
    std::vector<std::unique_ptr<Screen>> screenList;
};

class TCWindowSharedData {
    private:
        /* Weak Ptr instead of Shared Ptr to prevent self-referencing */
        /* Private Member Variables */
        std::weak_ptr<TCWindowData> data;
    public:
        /* Public Constructor */
        TCWindowSharedData(std::shared_ptr<TCWindowData> &winData);
        /* Public Methods */
        void switchScreen(size_t index);
        void terminate();
};


#endif // TRANSFERRABLEDATA_H