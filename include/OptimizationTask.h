#pragma once

#include<vector>

class OptimizationTask
{
public:
    OptimizationTask() = default;
    virtual ~OptimizationTask() = default;

    virtual double computeCost(std::vector<double> &position) = 0;
};