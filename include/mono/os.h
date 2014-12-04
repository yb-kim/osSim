#include <os.h>
#include <mono/environment.h>

class MonoEnvironment;

class MonoOS : public OS {
public:
    MonoOS(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
private:
    //
};
