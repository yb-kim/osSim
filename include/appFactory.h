#include <application.h>

class AppFactory {
public:
    AppFactory();
    virtual Application* createApp();
    //
private:
    unsigned int selectAppType();

    unsigned int appTypePointer;
};
