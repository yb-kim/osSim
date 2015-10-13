#include <appFactory.h>

AppFactory::AppFactory():
    appTypePointer(0), nApps(0), weightSum(0)
{
    //
}

AppFactory::AppFactory(int *weights, int nApps):
    weights(weights), nApps(nApps), weightSum(0)
{
    accumulatedWeights = new int[nApps];
    for(int i=0; i<nApps; i++) {
        weightSum += weights[i];
        accumulatedWeights[i] = weightSum;
    }
}

unsigned int AppFactory::selectAppType() {
    /*
    if(appTypePointer >= Application::getNSpecs()) appTypePointer = 0;
    return appTypePointer++;
    */

    int r = rand() % weightSum;
    for(int i=0; i<nApps; i++) {
        if(r < accumulatedWeights[i]) return i;
    }
}
