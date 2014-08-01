#include <iostream>
#include <monoOS.h>
#include <string>

using namespace std;

int main(void) {
    string configFileName = "config/system.json";

    Config cfg(configFileName);

    OS *os;
    switch(cfg.getOsType()) {
    case Config::MONO:
        os = new MonoOS(&cfg);
        break;
    defalut:
        cout << "no type selected" << endl;
        os = new MonoOS(&cfg);
    }
    os->run();

    return 0;
}
