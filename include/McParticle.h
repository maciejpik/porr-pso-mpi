#pragma once

#include "Particle.h"
#include "Settings.h"

class McParticle : public Particle
{
public:
    McParticle(Settings* settings, int id);
    virtual ~McParticle() = default;

    void updatePosition();
};