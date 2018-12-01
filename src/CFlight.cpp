#include "CFlight.hpp"

namespace flight
{
    CFlight::CFlight(double minTime, double maxTime, double idealTime, double earlyCost, double lateCost) :
        mp_minTime(minTime),
        mp_maxTime(maxTime),
        mp_idealTime(idealTime),
        mp_earlyCost(earlyCost),
        mp_lateCost(lateCost)
    {
    }

    CFlight::~CFlight()
    {
    }

}
