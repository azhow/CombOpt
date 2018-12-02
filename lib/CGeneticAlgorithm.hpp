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
            CGeneticAlgorithm(
                    double mutationRate,
                    bool debugFlag,
                    std::size_t populationSize,
                    unsigned int randSeed,
                    std::vector<flight::CFlight> flights);

            ~CGeneticAlgorithm();

            bool run(const unsigned int numOfGenerationsToConvergeWithoutImprovement);

        private:
            int selection();

            bool crossover(int parent1Idx, int parent2Idx, std::vector<CIndividual>& newGen);

            bool mutatePopulation();

            bool calculatePopulationFitness();

            double evolve();

            void printPopulationFitness() const;

            CIndividual getRandomElement() const;

            // How much the is the mutation
            double m_mutationRate;

            // Flag to print debug information
            bool m_debugFlag;

            // Population
            std::vector<CIndividual> m_population;

            // Size of the population
            std::size_t m_populationSize;

            // Seed used to generate pseud random numbers
            unsigned int m_randSeed;
    };
}

#endif
