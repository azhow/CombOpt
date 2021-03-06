#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "CIndividual.hpp"
#include "CFlight.hpp"

namespace GA
{
    // Class constructor
	CIndividual::CIndividual(
		std::vector<flight::CFlight> scheduledFlights,
		unsigned int randSeed,
		bool initialize) :
		m_randSeed(randSeed)
	{
		if (initialize)
		{
			for (auto gene : scheduledFlights)
			{
				flight::CFlight flight = flight::CFlight(
					gene.mp_appTime, gene.mp_minTime, gene.mp_idealTime, gene.mp_maxTime,
					gene.mp_earlyCost, gene.mp_lateCost, gene.mp_timeBetweenFlights);

				(flight.mp_maxTime != flight.mp_minTime) ? (flight.mp_actualLandingTime = (std::rand() % static_cast<int>(flight.mp_maxTime - flight.mp_minTime)) + flight.mp_minTime) : 0;

				m_genome.push_back(flight);
			}
		}
		else
		{
			for (auto gene : scheduledFlights)
			{

				flight::CFlight flight = flight::CFlight(
					gene.mp_appTime, gene.mp_minTime, gene.mp_idealTime, gene.mp_maxTime,
					gene.mp_earlyCost, gene.mp_lateCost, gene.mp_timeBetweenFlights);

				flight.mp_actualLandingTime = gene.mp_actualLandingTime;

				m_genome.push_back(flight);
			}
		}
	}

    // Default destructor
    CIndividual::~CIndividual()
    {
    }

    // Returns the genome of the individual
    std::vector<flight::CFlight>
        CIndividual::getGenome() const
    {
        return m_genome;
    }

    // The mutation process switches the order of 2 randomly selected flights
    bool
        CIndividual::mutate(double mutationChance)
    {
		for (auto gene : m_genome)
		{
			double op = static_cast<double>(std::rand()) / RAND_MAX;
			if(mutationChance > op)
			{
				(gene.mp_maxTime != gene.mp_minTime) ? (gene.mp_actualLandingTime = (std::rand() % static_cast<int>(gene.mp_maxTime - gene.mp_minTime)) + gene.mp_minTime) : 0;
				//gene.mp_actualLandingTime = (std::rand() % static_cast<int>(gene.mp_maxTime - gene.mp_minTime)) + gene.mp_minTime;
			}
		}

        return true;
    }

    // Calculates the fitness of the individual according to the problem definition
    bool
        CIndividual::calculateFitness()
    {
        const double penaltyValue = 30;
        bool bStatus(true);
        double fitness = 0;
        int numOfViolations = 0;

        for(auto f : m_genome)
        {
            // Check for violations of the constraints and how many times (favor more correct ones)
            bStatus = bStatus && checkConstraint2(numOfViolations);
            bStatus = bStatus && checkConstraint3(numOfViolations);
            // How early a plane landed
            double deltaEarly = std::max(0.0, f.mp_idealTime - f.mp_actualLandingTime);
            // How late a plane landed
            double deltaLate = std::max(0.0, f.mp_actualLandingTime - f.mp_idealTime);
			// Sum = objective + penalty
            fitness += f.mp_earlyCost * deltaEarly + f.mp_lateCost * deltaLate + penaltyValue * numOfViolations;
			//std::cout << f.mp_actualLandingTime << " " << f.mp_minTime << " " << f.mp_idealTime << " " << f.mp_maxTime << std::endl;
        }

        // The lowest the cost the better
        m_solutionValue = fitness;

        return bStatus;
    }

    // Constraint 2
    bool
        CIndividual::checkConstraint2(int& numOfViolations)
    {
        bool bStatus(true);

        for(auto i = m_genome.begin(); i != m_genome.end(); ++i)
        {
            for(auto j = m_genome.begin(); j != m_genome.end(); ++j)
            {
                if(i != j)
                {
                    // If the airplane I landed before the airplane J decide the delta
                    double delta = ((*i).mp_actualLandingTime < (*j).mp_actualLandingTime) ? (*i).mp_timeBetweenFlights : ((*i).mp_maxTime - (*j).mp_minTime);
                    // If the constraint is satisfied, the result is ok
                    if((*i).mp_actualLandingTime + delta <= (*j).mp_actualLandingTime)
                    {
                        bStatus = bStatus && true;
                    }
                    else
                    {
                        numOfViolations++;
                        bStatus = bStatus && false;
                    }
                }
            }
        }

        return bStatus;
    }

    // Constraint 3
    bool
        CIndividual::checkConstraint3(int& numOfViolations)
    {
        bool bStatus(true);

        // Check if all the airplane times are inside its interval of landing
        for(auto it = m_genome.begin(); it != m_genome.end(); ++it)
        {
            if(((*it).mp_minTime <= (*it).mp_actualLandingTime) <= (*it).mp_maxTime)
            {
                bStatus = bStatus && true;
            }
            else
            {
                numOfViolations++;
                bStatus = bStatus && false;
            }
        }

        return bStatus;
    }

    // Operator overload to use with sort method
    bool
        CIndividual::operator<(const CIndividual& other) const
    {
        return (m_solutionValue < other.getSolutionValue());
    }

    // Returns the solution value of the individual
    double
        CIndividual::getSolutionValue() const
    {
        return m_solutionValue;
    }

    // Gets a random element of the vector
    flight::CFlight
        CIndividual::getRandomElement() const
    {
        return m_genome[std::rand() % m_genome.size()];
    }
}
