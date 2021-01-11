#pragma once

#include "OptimizationTask.h"

#include <vector>

class OptimizationTask_1 : public OptimizationTask
{
public:
    OptimizationTask_1();
    virtual ~OptimizationTask_1() = default;

    double computeCost(std::vector<double> &position);
};