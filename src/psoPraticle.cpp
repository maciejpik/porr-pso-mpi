//
// Created by Agnieszka Jurkiewicz on 10/01/2021.
//

#include "../include/psoPraticle.h"
#include "../include/OptimizationExercisesConfig.h"
#include "../include/PositionVectorOperator.h"
#include <random>
#include <vector>

void psoParticle::setStartPosition(std::default_random_engine &gen)
{
    std::uniform_real_distribution<double> unif(config->upperLimitPositionVector,
                                                config->lowerLimitPositionVector);
    for (int i = 0; i < vectorDim; i++)
    {
        positionVectors[i] = unif(gen);
    }
}

void psoParticle::computeCostFunctionValue()
{
    costFunctionValue = config->computeCostFunctionValue(positionVectors);
}

double psoParticle::getCostFunctionValue() const
{
    return costFunctionValue;
}


std::vector<double> psoParticle::getPositionVector()
{
    return positionVectors;
}

void psoParticle::computeSpeed(float w, float speedConstant1, float speedConstant2,
                                 std::default_random_engine *gen)
{
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    double m = sqrt(vectorDim);
    double rand_1 = unif(*generator) / m;
    double rand_2 = unif(*generator) / m;
    double tempSpeedValue =
            w * speedVectors[i] + speedConstant1 * rand_1 * (positionVectorsParticlePbest[i] - positionVectors[i]) +
            speedConstant2 + rand_2 * (swarm->GbestVector[0].positionVectorsParticlePbest[i] - positionVectors[i]);
    //  cout<<"ts: "<<tempSpeedValue<<"\n";
    tempSpeedVectors[i] = tempSpeedValue;                                               v_velocityProposition), v_velocityProposition);
}

void psoParticle::computePosition(float w, float speedConstant1, float speedConstant2,
                                    std::default_random_engine *gen)
{
    vector<double> newPositionVector(vectorDim);

    computeSpeed(w, speedConstant1, speedConstant2, gen);
    newPositionVector = PositionVectorOperator::add(positionVectors, tempSpeedVectors);

    if(!config->isPositionInRange(newPositionVector))
        throw "Particle out of the box constraints!";

    speedVectors = tempSpeedVectors;
    positionVectors = newPositionVector;
}
