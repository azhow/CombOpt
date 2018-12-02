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
                    std::vector<std::shared_ptr<flight::CFlight>> flights);

            ~CGeneticAlgorithm();

            bool run(const unsigned int numOfGenerationsToConvergeWithoutImprovement);

        private:
            bool selection();

            bool crossover();

            bool mutatePopulation();

            bool calculatePopulationFitness();

            double evolve();

            bool rouletteWheelSelection(
                    std::vector<double> points,
                    std::vector<std::shared_ptr<CIndividual>>& selectedIndividuals);

            void printPopulationFitness() const;

            std::shared_ptr<CIndividual> getRandomElement() const;

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
