#include "../include/McParticle.h"
#include "../include/Settings.h"

#include <vector>
#include <random>
#include <math.h>

McParticle::McParticle(Settings* settings, int id)
    : Particle(settings, id) {}

void McParticle::updatePosition()
{
    std::uniform_real_distribution<double> deltaGenerator(-1, 1);
    std::default_random_engine* randomEngine = settings->getRandomEngine();
    std::vector<double> deltaPosition(settings->getDimensions());
    std::vector<double> newPosition(settings->getDimensions());

    for(int i = 0; i < settings->getDimensions(); i++)
    {
        deltaPosition[i] = settings->getMcConstants().sigma * deltaGenerator(*randomEngine);
        newPosition[i] = position[i] + deltaPosition[i];
    }

    double coeff = getCoefficientForBoundedPosition(newPosition, deltaPosition);
    for(int i = 0; i < settings->getDimensions(); i++)
        newPosition[i] = position[i] + coeff*deltaPosition[i];

    double newCost = settings->getTask()->computeCost(newPosition);

    // Show particle evalutaion for 2D case
    // if(settings->getProcessRank() == 0)
    // {
    //     printf("[%d]\t(%lf,\t%lf)\t=%lf\n", 0, newPosition[0], newPosition[1], newCost);
    // }

    if(newCost < cost)
    {
        cost = newCost;
        position = newPosition;
    }
    else
    {
        std::uniform_real_distribution<double> wrongGenerator(0, 1);
        double threshold = std::exp(-(newCost - cost) / settings->getMcConstants().T);
        if(wrongGenerator(*randomEngine) < threshold)
        {
            cost = newCost;
            position = newPosition;
        }
    }
}