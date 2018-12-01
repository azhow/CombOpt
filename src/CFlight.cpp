#include "CFlight.hpp"

namespace flight
{
    CFlight::CFlight(double minTime, double maxTime, double idealTime, double earlyCost, double lateCost, double timeBetweenFlights) :
        mp_minTime(minTime),
        mp_maxTime(maxTime),
        mp_idealTime(idealTime),
        mp_earlyCost(earlyCost),
        mp_lateCost(lateCost),
        mp_timeBetweenFlights(timeBetweenFlights)
    {
    }

    CFlight::~CFlight()
    {
    }

}
