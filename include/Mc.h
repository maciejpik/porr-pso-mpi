#pragma once

#include "Settings.h"
#include "McParticle.h"

#include <vector>

class Mc
{
public:
    Mc(Settings* settings);
    virtual ~Mc() = default;

    void solve();

private:
    Settings* settings;
    std::vector<McParticle*> particles;
};