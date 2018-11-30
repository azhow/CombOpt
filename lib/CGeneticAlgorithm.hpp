#ifndef CGENETICALGORITHM_HPP
#define CGENETICALGORITHM_HPP

#include <vector>
#include <memory>

#include "CIndividual.hpp"

namespace GA
{
    class CGeneticAlgorithm
    {
        public:
            CGeneticAlgorithm(double mutationChance, double mutationRate, bool debugFlag, std::size_t populationSize, unsigned int randSeed);

            ~CGeneticAlgorithm();

            bool run();

        private:
            bool selection();

            bool mutate();

            bool crossover();

            bool calculateFitness();

            // Chance of mutation
            double m_mutationChance;
            
            // How much the is the mutation
            double m_mutationRate;

            // Flag to print debug information
            bool m_debugFlag;

            // Population
            std::vector<std::shared_ptr<CIndividual>> m_population;

            // Size of the population
            std::size_t m_populationSize;

            // Seed used to generate pseud random numbers
            unsigned int m_randSeed;

    };
}

#endif
