#pragma once

class Settings
{
public:
    Settings(int argc, char* argv[]);
    virtual ~Settings() = default;

    int getRoot() {return root;}
    int getParticlesNumber() {return particlesNumber;}
    int getDimensions() {return dimensions;}
    int getProcessRank() {return processRank;}
    int getNumberOfProcesses() {return numberOfProcesses;}
    int getLocalParticlesNumber() {return localParticlesNumber;}

private:
    int root;
    int particlesNumber;
    int dimensions;
    int processRank;
    int numberOfProcesses;
    int localParticlesNumber;

    void computeLocalParticlesNumber();
};