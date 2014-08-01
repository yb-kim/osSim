#include <application.h>

class AppFactory {
public:
    AppFactory();
    virtual Application* createApp();
private:
    //
};
