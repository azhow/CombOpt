#include <cmath>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "CGeneticAlgorithm.hpp"
#include "CFlight.hpp"

#define MAX_RAND 20000

// Structure to use in the std::sort method
struct less_than_solution
{
    inline bool operator() (
            const std::shared_ptr<GA::CIndividual>& indiv1,
            const std::shared_ptr<GA::CIndividual>& indiv2)
    {
        return (indiv1->getSolutionValue() < indiv2->getSolutionValue());
    }
};

namespace GA
{
    CGeneticAlgorithm::CGeneticAlgorithm(
            double mutationRate,
            bool debugFlag,
            std::size_t populationSize,
            unsigned int randSeed) :
    m_mutationRate(mutationRate),
    m_debugFlag(debugFlag),
    m_populationSize(populationSize),
    m_randSeed(randSeed)
    {
        // Set random seed
        std::srand(m_randSeed);

        std::vector<std::shared_ptr<flight::CFlight>> flights;

        // Initialize population
        for(std::size_t i = 0; i < m_populationSize; i++)
        {
            // Create individual
            std::shared_ptr<CIndividual> pIndividual = std::make_shared<CIndividual>(flights, m_randSeed);
            // Adds individual to population
            m_population.push_back(pIndividual);
        }

        std::cout << "Setup ok!" << std::endl;

        for(auto it = m_population.begin(); it != m_population.end(); ++it)
        {
            std::cout << "Individual" << std::endl;
        }

        calculatePopulationFitness();
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
        CGeneticAlgorithm::crossover()
    {
        return false;

    }

    // Calculates the fitness of each individual in the population
    bool
        CGeneticAlgorithm::calculatePopulationFitness()
    {
        // Iterates through all individuals
        for(auto it = m_population.begin(); it != m_population.end(); ++it)
        {
            (*it)->calculateFitness();
        }

        return true;

    }

    // Runs the learning algorithm
    bool
        CGeneticAlgorithm::run(const unsigned int numOfGenerationsToConvergeWithoutImprovement)
    {
        // Best value/solution starts as +infinite
        double bestValue = std::numeric_limits<double>::infinity();
        // Current generation best value/solution starts as +infinite
        double currentValue = std::numeric_limits<double>::infinity();
        // Number of generations without real improvement in the answer
        unsigned int numOfGenerationsWithoutImprovement = 0;

        // Main loop of the algorithm
        while(numOfGenerationsWithoutImprovement < numOfGenerationsToConvergeWithoutImprovement)
        {
            // Do the main operations
            currentValue = evolve();
            (currentValue < bestValue) ? (bestValue = currentValue) : (numOfGenerationsWithoutImprovement++);
        }

        return true;
    }

    // Mutates the whole population
    bool
        CGeneticAlgorithm::mutatePopulation()
    {
        return true;
    }

    // Works with the intermediate population and in the end generates the new population
    double
        CGeneticAlgorithm::evolve()
    {
        selection();

        crossover();

        mutatePopulation();

        calculatePopulationFitness();

        // Sorts the population based on the solution value in ascending order
        std::sort(m_population.begin(), m_population.end(), less_than_solution());

        // Returns the solution with the lowest value
        return m_population[0]->getSolutionValue();
    }
}
