//
// Created by Agnieszka Jurkiewicz on 10/01/2021.
//
#pragma once
#ifndef PORR_MPI_PSOPRATICLE_H
#define PORR_MPI_PSOPRATICLE_H

#include "OptimizationExercisesConfig.h"
#include <vector>
#include <random>

class psoParticle
{
public:
    psoParticle();
    virtual ~psoParticle();

    void setStartPosition(std::default_random_engine &gen);
    void computeCostFunctionValue();
    double getCostFunctionValue() const;
    double getCostFunctionValue(std::vector<double> &v_position) const;
    std::vector<double> getPositionVector();
    void computePosition(float w, float speedConstant1, float speedConstant2,
                         std::default_random_engine *gen);
    void computeSpeed(float w, float speedConstant1, float speedConstant2,
                      std::default_random_engine *gen);

protected:

    int vectorDim;
    std::vector<double> positionVectors;
    double costFunctionValue;
    OptimizationExercisesConfig* config;
};

#endif //PORR_MPI_PSOPRATICLE_H
