#include <monoOS.h>
#include <string>

using namespace std;

int main(void) {
    string configFileName = "config/example.json";
    Config cfg(configFileName);

    MonoOS os(&cfg);
    os.run();
    return 0;
}
