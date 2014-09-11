#include <application.h>
#include <micro/os.h>

class MicroApplication : public Application {
public:
    MicroApplication(unsigned int appSpecIndex);
    void run(unsigned int unitTick);
    void freeLock();

private:
    void ipc(MicroOS::Services service);
};
