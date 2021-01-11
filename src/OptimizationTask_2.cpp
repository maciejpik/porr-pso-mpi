#include "../include/OptimizationTask_2.h"

#include <vector>
#include <math.h>

OptimizationTask_2::OptimizationTask_2()
    : OptimizationTask() {}

double OptimizationTask_2::computeCost(std::vector<double> &position)
{
    double costFunctionValue = 0.0;
    for (unsigned int i = 0; i < position.size() - 1; i++)
    {
        costFunctionValue = costFunctionValue + (pow((position[i + 1] -
            pow(position[i], 2)), 2) * 100 + pow((1 - position[i]), 2));
    }
    return costFunctionValue;
}