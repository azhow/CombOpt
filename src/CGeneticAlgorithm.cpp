#include <cmath>
#include <iostream>

#include "CGeneticAlgorithm.hpp"

namespace GA
{
    CGeneticAlgorithm::CGeneticAlgorithm(double mutationChance, double mutationRate, bool debugFlag, int populationSize, int randSeed) :
    m_mutationChance(mutationChance),
    m_mutationRate(mutationRate),
    m_debugFlag(debugFlag),
    m_populationSize(populationSize),
    m_randSeed(randSeed)
    {
        std::cout << "Setup ok!" << std::endl;
    }

    // Default destructor
    CGeneticAlgorithm::~CGeneticAlgorithm()
    {
    }

    bool
        CGeneticAlgorithm::selection()
    {
        return false;
    }

    bool
        CGeneticAlgorithm::mutateIndividual()
    {
        return false;

    }

    bool 
        CGeneticAlgorithm::crossoverIndividual()
    {
        return false;

    }

    bool
        CGeneticAlgorithm::calculateFitnessIndividual()
    {
        return false;

    }

    bool 
        CGeneticAlgorithm::mutatePopulation()
    {
        return false;

    }

    bool
        CGeneticAlgorithm::crossoverPopulation()
    {
        return false;

    }

    bool
        CGeneticAlgorithm::calculateFitnessPopulation()
    {
        return false;

    }
}
