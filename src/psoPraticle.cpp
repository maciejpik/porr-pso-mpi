//
// Created by Agnieszka Jurkiewicz on 10/01/2021.
//

#include "../include/psoPraticle.h"
#include "../include/OptimizationExercisesConfig.h"
#include "../include/PositionVectorOperator.h"
#include <random>
#include <vector>


psoParticle::psoParticle(int dim,OptimizationExercisesConfig* config) {
    dimensions = dim;
    globalBestPosition=new double[dim];
    w = 0.72984;
    c1 = 2.05;
    c2 = 2.05;
    c1 = w * c1;
    c2 = w * c2;
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
    std::uniform_real_distribution<double> unif(config->upperLimitPositionVector ,
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
    std::uniform_real_distribution<double> unif((config->lowerLimitPositionVector - config->upperLimitPositionVector) / (dimensions * dimensions),
                                                (config->upperLimitPositionVector - config->lowerLimitPositionVector) / (dimensions * dimensions));
            //(config->lowerLimitPositionVector - config->upperLimitPositionVector),
            //(config->upperLimitPositionVector - config->lowerLimitPositionVector));
    for (int i = 0; i < dimensions; i++)
    {
        speedVectors[i] = unif(gen);
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
/*
void psoParticle::computeSpeed(std::default_random_engine *gen)
{
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    double m = sqrt(dimensions);
    for (int i = 0; i< dimensions; i++) {
        double rand_1 = unif(*gen) / m;
        double rand_2 = unif(*gen) / m;
        double tempSpeedValue =
                w * speedVectors[i] + c1 * rand_1 * (positionVectorsParticlePbest[i] - positionVectors[i]) +
                c2 * rand_2 * (globalBestPosition[i] - positionVectors[i]);
        //  cout<<"ts: "<<tempSpeedValue<<"\n";
        tempSpeedVectors[i] = tempSpeedValue;
    }
}*/

double psoParticle::getCoefficientForBoundedPosition(std::vector<double> &newPositionVector,
                                                  std::vector<double> &tempSpeedVectors)
{
    double k = 1;
    for (int i = 0; i < dimensions; i++)
    {
        char violation = 0;
        if (newPositionVector[i] > config->upperLimitPositionVector)
            violation = 1;
        else if (newPositionVector[i] < config->lowerLimitPositionVector)
            violation = -1;

        if (violation)
        {
            /* Compute coefficient k so as to make x_{i+1,j} = x{i,j} + k*v{i, j} fit in
            the box constraints. */
            double c1 = violation > 0 ? config->upperLimitPositionVector : config->lowerLimitPositionVector;
            double k_temp = (-positionVectors[i] + c1) / tempSpeedVectors[i];
            if (k_temp < k)
                k = k_temp;
        }
    }
    return k;
}


void psoParticle::computeSpeed(/*float w, float speedConstant1, float speedConstant2,*/
                                 std::default_random_engine *gen)
{
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    float speedConstant1=c1;
    float speedConstant2=c2;
    double rand_1 = unif(*gen);
    double rand_2 = unif(*gen);

    std::vector<double> v_velocityProposition = PositionVectorOperator::add(
            PositionVectorOperator::mult(w, speedVectors),
            PositionVectorOperator::add(
                    PositionVectorOperator::mult(
                            speedConstant1 * rand_1,
                            PositionVectorOperator::minus(positionVectorsParticlePbest, positionVectors)),
                    PositionVectorOperator::mult(
                            speedConstant2 * rand_2,
                            PositionVectorOperator::minus(std::vector<double>(globalBestPosition,globalBestPosition+dimensions),
                                                          positionVectors))));

    std::vector<double> v_positionProposition = PositionVectorOperator::add(positionVectors,
                                                                            v_velocityProposition);
    tempSpeedVectors = PositionVectorOperator::mult(getCoefficientForBoundedPosition(v_positionProposition,
                                                                                     v_velocityProposition), v_velocityProposition);
}
void psoParticle::computePosition(
                                    std::default_random_engine *gen) {
    std::vector<double> newPositionVector(dimensions);

    computeSpeed(gen);
    newPositionVector = PositionVectorOperator::add(positionVectors, tempSpeedVectors);

    speedVectors = tempSpeedVectors;
    positionVectors = newPositionVector;
    computeCostFunctionValue();
    if (getCostFunctionValue() < getCostFunctionValuePbest()) {
        positionVectorsParticlePbest = positionVectors;
        computeCostFunctionValuePbest();
    }
}
/*
void psoParticle::computePosition(std::default_random_engine *gen)
{
    std::vector<double> newPositionVector(dimensions);

    computeSpeed(gen);
    for(int i = 0; i < dimensions; i++)
    {
        newPositionVector[i] = positionVectors[i] + tempSpeedVectors[i];
    }
    double coef = getCoefficientForBoundedPosition(newPositionVector,  tempSpeedVectors);
    for(int i = 0; i < dimensions; i++)
    {
        newPositionVector[i] = positionVectors[i] + coef*tempSpeedVectors[i];
    }

    speedVectors = tempSpeedVectors;
    positionVectors = newPositionVector;
    computeCostFunctionValue();
    if (getCostFunctionValue() < getCostFunctionValuePbest()) {
        positionVectorsParticlePbest = positionVectors;
        computeCostFunctionValuePbest();
    }
}
*/