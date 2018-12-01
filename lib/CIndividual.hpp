#ifndef CINDIVIDUAL_HPP
#define CINDIVIDUAL_HPP

#include <vector>
#include <memory>

#include "CFlight.hpp"

namespace GA
{
    class CIndividual
    {
        public:
            CIndividual(
                std::vector<std::shared_ptr<flight::CFlight>> scheduledFlights,
                unsigned int randSeed,
                bool initialize);

            ~CIndividual();

            bool mutate();

            bool calculateFitness();

            bool operator<(const CIndividual& other) const;

            double getSolutionValue() const;

            std::vector<std::shared_ptr<flight::CFlight>> getGenome() const;

            bool crossover(std::vector<std::shared_ptr<flight::CFlight>> newGenome);

            bool checkConstraint2(int& numOfViolations);

            bool checkConstraint3(int& numOfViolations);

        private:
            std::shared_ptr<flight::CFlight> getRandomElement() const;

            // Chromosomes of the inidividual (order of the planes)
            std::vector<std::shared_ptr<flight::CFlight>> m_genome;

            // Current fitness value of this solution
            double m_fitnessValue;

            // Random seed
            unsigned int m_randSeed;

            // Solution value
            double m_solutionValue;
    };
}

#endif
