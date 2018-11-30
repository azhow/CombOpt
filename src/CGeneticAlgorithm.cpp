#include <cmath>
#include <iostream>
#include <cstdlib>

#include "CGeneticAlgorithm.hpp"

#define MAX_RAND 20000

namespace GA
{
    CGeneticAlgorithm::CGeneticAlgorithm(double mutationChance, double mutationRate, bool debugFlag, std::size_t populationSize, unsigned int randSeed) :
    m_mutationChance(mutationChance),
    m_mutationRate(mutationRate),
    m_debugFlag(debugFlag),
    m_populationSize(populationSize),
    m_randSeed(randSeed)
    {
        // Set random seed
        std::srand(m_randSeed);

        // Initialize population
        for(std::size_t i = 0; i < m_populationSize; i++)
        {
            std::shared_ptr<CIndividual> pIndividual = std::make_shared<CIndividual>();
            // Create individual's genome
            for(std::size_t j = 0; j < 10; j++)
            {
                // Add random gene to individual
                pIndividual->addGene(std::rand() - MAX_RAND/2);
            }
            // Adds individual to population
            m_population.push_back(pIndividual);
        }

        std::cout << "Setup ok!" << std::endl;

        for(auto it = m_population.begin(); it != m_population.end(); ++it)
        {
            std::cout << "Individual" << std::endl;
        }
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
        CGeneticAlgorithm::mutate()
    {
        return false;

    }

    bool
        CGeneticAlgorithm::crossover()
    {
        return false;

    }

    bool
        CGeneticAlgorithm::calculateFitness()
    {
        return false;

    }
}
