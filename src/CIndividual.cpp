#include <cstdlib>

#include "CIndividual.hpp"

namespace GA
{
    CIndividual::CIndividual()
    {
    }

    CIndividual::~CIndividual()
    {
    }

    bool
        CIndividual::addGene(double gene)
    {
        m_genome.push_back(gene);
        
        return true;
    }
}
