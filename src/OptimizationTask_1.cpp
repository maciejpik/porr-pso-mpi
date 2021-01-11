#include "../include/OptimizationTask_1.h"

#include <vector>
#include <math.h>

OptimizationTask_1::OptimizationTask_1()
    : OptimizationTask()
{
    taskId = 1;
}

double OptimizationTask_1::computeCost(std::vector<double> &position)
{
    double sum = 0;
    double product = 1;
    for (int i = 0; i < position.size(); i++)
    {
        sum = sum + position[i]*position[i];
        product = product * cos(position[i] / (i + 1));
    }
    return 1.0 / 40 * sum + 1 - product;
}