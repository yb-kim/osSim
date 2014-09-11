#include <os.h>

class MicroOS : public OS {
public:
    MicroOS(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    enum Services {
        NS, //name service
        FM,
        PM
    };
private:
    //
};
