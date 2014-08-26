#include <iostream>
#include <mono/os.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "usage: osSim <config file root directory>" << endl;
        exit(0);
    }

    string configFileRoot(argv[1]);
    if(configFileRoot[configFileRoot.size()-1] != '/') configFileRoot += '/';
    //string configFileName = "config/example/system.json";

    Config cfg(configFileRoot);

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
