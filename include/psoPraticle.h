//
// Created by Agnieszka Jurkiewicz on 10/01/2021.
//
#pragma once
#ifndef PORR_MPI_PSOPRATICLE_H
#define PORR_MPI_PSOPRATICLE_H

#include "OptimizationExercisesConfig.h"
#include "pso.h"
#include <vector>
#include <random>



class psoParticle
{
public:
    psoParticle(int dim,OptimizationExercisesConfig* config);
    virtual ~psoParticle();

    void setStartPosition(std::default_random_engine &gen);
    void setStartSpeed(std::default_random_engine &gen);
    void computeCostFunctionValue();
    void computeCostFunctionValuePbest();
    double getCostFunctionValue() const;
    double getCostFunctionValue(std::vector<double> &v_position) const;
    double getCostFunctionValuePbest() const;
    std::vector<double> getPositionVector();
    void computePosition(std::default_random_engine *gen);
    void computeSpeed(std::default_random_engine *gen);
    double *globalBestPosition ;
    double getCoefficientForBoundedPosition(std::vector<double> &newPositionVector,
                                                         std::vector<double> &tempSpeedVectors);

protected:

    int dimensions;
    double w, c1, c2;
    std::vector<double> positionVectors;
    double costFunctionValue;
    double costFunctionValuePbest;
    OptimizationExercisesConfig* config;
    std::vector<double> positionVectorsParticlePbest;
    std::vector<double> speedVectors;
    std::vector<double> tempSpeedVectors;

};

#endif //PORR_MPI_PSOPRATICLE_H
