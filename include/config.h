class Config {
public:
    Config();
    unsigned int getMaxTick() { return maxTick; }
    unsigned int getUnitTick() { return unitTick; }
    unsigned int getNCores() { return nCores; }
private:
    unsigned int maxTick;
    unsigned int unitTick;
    unsigned int nCores;
};
