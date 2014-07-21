#include <iostream>
#include <environment.h>

using namespace std;

Environment::Environment(int _nCores) :
    nCores(_nCores)
{
    cores = new Core[nCores];
}
