#ifndef CGENETICALGORITHM_H
#define CGENETICALGORITHM_H

#include <vector>

namespace GA
{
    class CGeneticAlgorithm
    {
        public:
            CGeneticAlgorithm(double mutationChance, double mutationRate, bool debugFlag, int populationSize, int randSeed);

            ~CGeneticAlgorithm();

            bool run();

        private:
            bool selection();

            bool mutateIndividual();

            bool crossoverIndividual();

            bool calculateFitnessIndividual(); 

            bool mutatePopulation();

            bool crossoverPopulation();

            bool calculateFitnessPopulation();

            // Chance of mutation
            double m_mutationChance;
            
            // How much the is the mutation
            double m_mutationRate;

            // Flag to print debug information
            bool m_debugFlag;

            // Population
            std::vector<double> m_population;

            // Random range
            const int mc_randRange = 10000;

            // Size of the population
            int m_populationSize;

            // Seed used to generate pseud random numbers
            int m_randSeed;

    };
}

#endif
