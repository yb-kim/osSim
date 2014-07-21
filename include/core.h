#include <application.h>

class Core {
public:
    Core();
    void loadApp(Application *app);
private:
    Application *appRunning;
};
