#pragma once

#ifndef PSOCONFIG
#define PSOCONFIG

#include <vector>
#include <istream>

class OptimizationExercisesConfig
{
public:
    OptimizationExercisesConfig();
    virtual ~OptimizationExercisesConfig();
    virtual double computeCostFunctionValue(std::vector<double> positionVector) = 0;
    virtual bool isXInRange(double position);
    virtual bool isPositionInRange(std::vector<double> positionVector);

    double lowerLimitPositionVector;
    double upperLimitPositionVector;
};

#endif //PSOCONFIG
