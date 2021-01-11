#include "../include/Particle.h"
#include "../include/Settings.h"

#include <random>
#include <stdio.h>
#include <string>

Particle::Particle(Settings* settings, int id)
    : settings(settings), id(id)
{
    position.resize(settings->getDimensions(), 0);

    std::default_random_engine* randomEngine = settings->getRandomEngine();
    std::uniform_real_distribution<double> positionGenerator(
        settings->getInitializationConstraints().min,
        settings->getInitializationConstraints().max
    );
    for(int i = 0; i < settings->getDimensions(); i++)
        position[i] = positionGenerator(*randomEngine);

    computeCost();
}

void Particle::computeCost()
{
    cost = settings->getTask()->computeCost(position);
}

void Particle::print()
{
    printf("[%d] f([%d]) = %lf\n", settings->getProcessRank(), id, cost);
}

double Particle::getCoefficientForBoundedPosition(std::vector<double> &newPosition,
    std::vector<double> &deltaPosition)
{
    double k = 1;
    for (int i = 0; i < settings->getDimensions(); i++)
    {
        int violation = 0;
        if (newPosition[i] > settings->getSolutionConstraints().max)
            violation = 1;
        else if (newPosition[i] < settings->getSolutionConstraints().min)
            violation = -1;

        if (violation)
        {
            /* Compute coefficient k so as to make x_{i+1,j} = x{i,j} + k*v{i, j} fit in
            the box constraints. */
            double c1 = violation > 0 ? settings->getSolutionConstraints().max : settings->getSolutionConstraints().min;
            double k_temp = (-position[i] + c1) / deltaPosition[i];
            if (k_temp < k)
                k = k_temp;
        }
    }
    return k;
}  