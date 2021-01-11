#pragma once

#include "OptimizationTask.h"

#include <random>

enum communicationType 
{
    BEST_SOLUTION,
    STOP
};

struct boxConstraints
{
    double min;
    double max;
};

struct mcConstants
{
    double sigma;
    double T;
};

class Settings
{
public:
    Settings(int argc, char *argv[]);
    virtual ~Settings() = default;

    int getRoot() { return root; }
    int getParticlesNumber() { return particlesNumber; }
    int getDimensions() { return dimensions; }
    int getProcessRank() { return processRank; }
    int getNumberOfProcesses() { return numberOfProcesses; }
    int getLocalParticlesNumber() { return localParticlesNumber; }
    boxConstraints getInitializationConstraints() {return initializationConstraints;}
    boxConstraints getSolutionConstraints() {return solutionConstraints;}
    std::default_random_engine* getRandomEngine() {return &randomEngine;}
    OptimizationTask* getTask() {return task;}
    mcConstants getMcConstants() {return mc;}
    double getStopCriterion() {return stopCriterion;}
    bool getVerbose() {return verbose;}

    void setVerbose(bool v) {verbose = v;}

private:
    int root;
    int particlesNumber;
    int dimensions;
    int processRank;
    int numberOfProcesses;
    int localParticlesNumber;
    boxConstraints initializationConstraints;
    boxConstraints solutionConstraints;
    std::default_random_engine randomEngine;
    OptimizationTask* task;
    mcConstants mc;
    double stopCriterion;
    bool verbose;

    void computeLocalParticlesNumber();
};