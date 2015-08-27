#pragma once

#include <mono/os.h>

class MonoOSTypeA : public MonoOS {
public:
    MonoOSTypeA(Config *cfg);
    void checkAndDoSchedule();
    void init();
    void makeAppFactory();
    //
};
