//
// Created by Agnieszka Jurkiewicz on 01/11/2020.
//
#pragma once
#include "OptimizationExercisesConfig.h"

#include <vector>

class ConfigEx2 : public OptimizationExercisesConfig
{
public:
    ConfigEx2();
    virtual ~ConfigEx2();
    double computeCostFunctionValue(std::vector<double> positionVector);
};


