#include <monoOS.h>

int main(void) {
    Config cfg;
    MonoOS os(&cfg);
    os.run();
    return 0;
}
