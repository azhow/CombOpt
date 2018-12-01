#include <cmath>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <random>
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

        // This will be input data
        // Scheduled flights
        std::vector<std::shared_ptr<flight::CFlight>> flights;
        for(std::size_t i = 0; i < 10; i++)
        {
            flights.push_back(std::make_shared<flight::CFlight>(0, 0, 0, 0, 0));
        }

        // Initialize population
        for(std::size_t i = 0; i < m_populationSize; i++)
        {
            // Create individual
            std::shared_ptr<CIndividual> pIndividual = std::make_shared<CIndividual>(flights, m_randSeed, true);
            // Adds individual to population
            m_population.push_back(pIndividual);
        }

        // Debug info
        if(m_debugFlag && false)
        {
            std::cout << "Setup ok!" << std::endl;

            for(auto it = m_population.begin(); it != m_population.end(); ++it)
            {
                std::cout << "Individual" << std::endl;
            }
        }

        calculatePopulationFitness();
    }

    // Default destructor
    CGeneticAlgorithm::~CGeneticAlgorithm()
    {
    }

    // Runs the selection phase (Stochastic Universal Selection)
    bool
        CGeneticAlgorithm::selection()
    {
        double populationTotalFitness = 0;
        double distanceBetweenPoints = populationTotalFitness/m_populationSize;

        // Random engine
        std::default_random_engine gen(m_randSeed);
        // Random real distribution
        std::uniform_real_distribution<double> dis(0.0, distanceBetweenPoints);
        // Start point of the selection
        double start = dis(gen);
        // Selection points vector
        std::vector<double> points;

        for(std::size_t i = 0; i < m_populationSize; i++)
        {
            points.push_back(start + i * distanceBetweenPoints);
        }

        // Individuals chosen in the process
        std::vector<std::shared_ptr<CIndividual>> selectedIndividuals;

        rouletteWheelSelection(points, selectedIndividuals);

        // Update population
        m_population = selectedIndividuals;

        return false;
    }

    // Roulette wheel selection
    bool
        CGeneticAlgorithm::rouletteWheelSelection(std::vector<double> points, std::vector<std::shared_ptr<CIndividual>>& selectedIndividuals)
    {
        for(double p : points)
        {
            std::size_t index = 0;
            double accumulator = 0;
            while(accumulator < p)
            {
                accumulator += m_population[index]->getSolutionValue();
                index++;
            }
            selectedIndividuals.push_back(m_population[index]);
        }
    
        return true;
    }

    // Does the crossover phase (One point crossover)
    bool
        CGeneticAlgorithm::crossover()
    {
        std::default_random_engine gen(m_randSeed);

        // Creates offspring and add to population
        for(std::size_t i = 0; i < 10; i++)
        {
            std::shared_ptr<CIndividual> parent1 = getRandomElement();
            std::shared_ptr<CIndividual> parent2 = getRandomElement();
            std::uniform_int_distribution<> dis(0, parent1->getGenome().size() - 1);
            int point = dis(gen);

            std::vector<std::shared_ptr<flight::CFlight>> genome;
            std::vector<std::shared_ptr<flight::CFlight>> p1genome = parent1->getGenome();
            std::vector<std::shared_ptr<flight::CFlight>> p2genome = parent2->getGenome();
 
            genome.reserve(parent1->getGenome().size());

            genome.insert(
                    genome.end(),
                    p1genome.begin(),
                    p1genome.begin() + point);

            genome.insert(
                    genome.end(),
                    p2genome.begin() + point,
                    p2genome.end());

            m_population.push_back(std::make_shared<CIndividual>(genome, m_randSeed, false));
        }

        return true;
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
        // Current generation
        unsigned int generation = 0;

        // Main loop of the algorithm
        while(numOfGenerationsWithoutImprovement < numOfGenerationsToConvergeWithoutImprovement)
        {
            if(m_debugFlag)
            {
                std::cout << "Gen: " << generation << std::endl;
            }
            // Do the main operations
            currentValue = evolve();
            (currentValue < bestValue) ? (bestValue = currentValue) : (numOfGenerationsWithoutImprovement++);
            if(m_debugFlag)
            {
                std::cout << "\tBest: " << bestValue << "\n\tCurrent best: " << currentValue << std::endl;
            }
            generation++;
        }

        return true;
    }

    // Mutates the whole population
    bool
        CGeneticAlgorithm::mutatePopulation()
    {
        for(auto it = m_population.begin(); it != m_population.end(); ++it)
        {
            (*it)->mutate();
        }

        return true;
    }

    // Works with the intermediate population and in the end generates the new population
    double
        CGeneticAlgorithm::evolve()
    {
        if(m_debugFlag && false)
        {
            std::cout << "Starting selection" << std::endl;
        }
        selection();
        if(m_debugFlag && false)
        {
            std::cout << "Starting crossover" << std::endl;
        }
        crossover();
        if(m_debugFlag && false)
        {
            std::cout << "Starting mutation" << std::endl;
        }
        mutatePopulation();
        if(m_debugFlag && false)
        {
            std::cout << "Recalculating fitness of the population" << std::endl;
        }
        calculatePopulationFitness();

        // Sorts the population based on the solution value in ascending order
        std::sort(m_population.begin(), m_population.end(), less_than_solution());

        // Returns the solution with the lowest value
        return m_population[0]->getSolutionValue();
    }

    // Gets a random element of the vector
    std::shared_ptr<CIndividual> 
        CGeneticAlgorithm::getRandomElement() const
    {
        // Creates a mersenne twister engine
        std::default_random_engine gen(m_randSeed);

        // Creates an integer distribution
        std::uniform_int_distribution<> dis(0, m_population.size() - 1);

        return m_population[dis(gen)];
    }

}
