//
// Created by Agnieszka Jurkiewicz on 10/01/2021.
//

#include "../include/psoPraticle.h"
#include "../include/OptimizationExercisesConfig.h"
#include <random>
#include <vector>


psoParticle::psoParticle(int dim,OptimizationExercisesConfig* config) {
    dimensions = dim;
    globalBestPosition=new double[dim];
    w = 0.72984;
    c1 = 2.05;
    c2 = 2.05;
    this->config=config;
    positionVectors.resize(dim);
    positionVectorsParticlePbest.resize(dim);
    speedVectors.resize(dim);
    tempSpeedVectors.resize(dim);
};

psoParticle::~psoParticle(){
    delete[] globalBestPosition;
}

void psoParticle::setStartPosition(std::default_random_engine &gen)
{
    std::uniform_real_distribution<double> unif(config->upperLimitPositionVector - 70,
                                                config->lowerLimitPositionVector);
    for (int i = 0; i < dimensions; i++)
    {
        positionVectors[i] = unif(gen);
        positionVectorsParticlePbest[i] = positionVectors[i];
        globalBestPosition[i] = positionVectors[i];
    }
    computeCostFunctionValuePbest();

}

void psoParticle::setStartSpeed(std::default_random_engine &gen)
{
    std::uniform_real_distribution<double> unif(0.0,1.0);
            //(config->lowerLimitPositionVector - config->upperLimitPositionVector),
            //(config->upperLimitPositionVector - config->lowerLimitPositionVector));
    for (int i = 0; i < dimensions; i++)
    {
        speedVectors[i] = unif(gen)/160;
    }
}

void psoParticle::computeCostFunctionValue()
{
    costFunctionValue = config->computeCostFunctionValue(positionVectors);
}

void psoParticle::computeCostFunctionValuePbest()
{
    costFunctionValue = config->computeCostFunctionValue(positionVectorsParticlePbest);
}

double psoParticle::getCostFunctionValue() const
{
    return costFunctionValue;
}

double psoParticle::getCostFunctionValuePbest() const
{
    return costFunctionValuePbest;
}


std::vector<double> psoParticle::getPositionVector()
{
    return positionVectors;
}

void psoParticle::computeSpeed(std::default_random_engine *gen)
{
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    double m = sqrt(dimensions);
    for (int i = 0; i< dimensions; i++) {
        double rand_1 = unif(*gen) / m;
        double rand_2 = unif(*gen) / m;
        double tempSpeedValue =
                w * speedVectors[i] + c1 * rand_1 * (positionVectorsParticlePbest[i] - positionVectors[i]) +
                c2 + rand_2 * (globalBestPosition[i] - positionVectors[i]);
        //  cout<<"ts: "<<tempSpeedValue<<"\n";
        tempSpeedVectors[i] = tempSpeedValue;
    }
}

void psoParticle::computePosition(std::default_random_engine *gen)
{
    std::vector<double> newPositionVector(dimensions);

    do  {
        computeSpeed(gen);
        for (int i = 0; i < dimensions; i++) {
            newPositionVector[i] = positionVectors[i] + tempSpeedVectors[i];
        }
    } while(!config->isPositionInRange(newPositionVector));

    speedVectors = tempSpeedVectors;
    positionVectors = newPositionVector;
    computeCostFunctionValue();
    if (getCostFunctionValue() < getCostFunctionValuePbest()) {
        positionVectorsParticlePbest = positionVectors;
        computeCostFunctionValuePbest();
    }
}
