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
                unsigned int randSeed);

            ~CIndividual();

            bool mutate();

            bool calculateFitness();

            bool operator<(const CIndividual& other) const;

            double getSolutionValue() const;

            std::vector<std::shared_ptr<flight::CFlight>> getGenome() const;

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
