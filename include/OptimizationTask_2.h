#pragma once

#include "OptimizationTask.h"

#include <vector>

class OptimizationTask_2 : public OptimizationTask
{
public:
    OptimizationTask_2();
    virtual ~OptimizationTask_2() = default;

    double computeCost(std::vector<double> &position);
};