#include "CFlight.hpp"

namespace flight
{
    CFlight::CFlight(double appTime, double minTime, double idealTime, double maxTime, double earlyCost, double lateCost, double timeBetweenFlights) :
        mp_appTime(appTime),
        mp_minTime(minTime),
        mp_idealTime(idealTime),
        mp_maxTime(maxTime),
        mp_earlyCost(earlyCost),
        mp_lateCost(lateCost),
        mp_timeBetweenFlights(timeBetweenFlights)
    {
    }

    CFlight::~CFlight()
    {
    }

}
