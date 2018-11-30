#ifndef CINDIVIDUAL_HPP
#define CINDIVIDUAL_HPP

#include <vector>

namespace GA
{
    class CIndividual
    {
        public:
            CIndividual();

            ~CIndividual();

            bool mutate();

            bool crossover();

            bool calculateFitness();

            bool addGene(double gene);
        private:
            // Chromosomes of the inidividual
            std::vector<double> m_genome;

            // Genome size of the individual (every individual should have the same)
            int m_genomeSize;

            // Current fitness value of this solution
            double m_fitnessValue;
    };
}

#endif
