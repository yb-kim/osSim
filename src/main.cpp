#include <iostream>
#include <monoOS.h>
#include <string>

using namespace std;

int main(void) {
    string configFileName = "config/example.json";
    string syscallSpecs = "config/mono/syscalls.json";
    string appSpecs = "config/mono/apps.json";

    Config cfg(configFileName);
    Syscall::setSyscalls(syscallSpecs);

    MonoOS os(&cfg);
    os.run();
    return 0;
}
