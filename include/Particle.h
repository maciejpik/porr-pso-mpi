#pragma once

#include "Settings.h"

#include <vector>

class Particle
{
public:
    Particle(Settings* settings, int id);
    virtual ~Particle() = default;

    void print();

    double getCost() {return cost;}
    std::vector<double> getPosition() {return position;}

protected:
    std::vector<double> position;
    double cost;
    Settings* settings;
    int id;

    void computeCost();
    double getCoefficientForBoundedPosition(std::vector<double> &newPosition,
        std::vector<double> &deltaPosition);
};