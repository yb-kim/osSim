#include <os.h>

class MonoOS : public OS {
public:
    MonoOS(Config *cfg);
    void init();
private:
    //
};
