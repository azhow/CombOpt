#ifndef CFLIGHT_HPP
#define CFLIGHT_HPP

namespace flight
{
    class CFlight
    {
        public:
            CFlight(double minTime, double maxTime, double idealTime, double earlyCost, double lateCost, double timeBetweenFlights);

            ~CFlight();

            // Earliest time that the flight can land
            double mp_minTime;

            // Latest time that the flight can land
            double mp_maxTime;

            // Ideal time for the flight to land
            double mp_idealTime;

            // Cost per unit of time ahead of ideal time
            double mp_earlyCost;

            // Cost per unit of time behind of ideal time
            double mp_lateCost;

            // Time of the actual landing
            double mp_actualLandingTime;

            // Time between each flight landing
            double mp_timeBetweenFlights;
    };
}

#endif
