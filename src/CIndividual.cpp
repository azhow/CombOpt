#include <cstdlib>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>

#include "CIndividual.hpp"
#include "CFlight.hpp"

namespace GA
{
    // Class constructor
    CIndividual::CIndividual(
            std::vector<std::shared_ptr<flight::CFlight>> scheduledFlights,
            unsigned int randSeed) :
        m_genome(scheduledFlights),
        m_randSeed(randSeed)
    {
        // Set seed
        std::srand(m_randSeed);

        // Random flight order in the initialization
        std::random_shuffle(m_genome.begin(), m_genome.end());
    }

    // Default destructor
    CIndividual::~CIndividual()
    {
    }

    // Returns the genome of the individual
    std::vector<std::shared_ptr<flight::CFlight>>
        CIndividual::getGenome() const
    {
        return m_genome;
    }

    // The mutation process switches the order of 2 randomly selected flights
    bool
        CIndividual::mutate()
    {
        // Selects randomly one individual
        std::shared_ptr<flight::CFlight> el1 = getRandomElement();
        // Selects randomly one individual
        std::shared_ptr<flight::CFlight> el2 = getRandomElement();

        // Swaps Flights's landing times
        std::swap(el1->mp_actualLandingTime, el2->mp_actualLandingTime);

        return true;
    }

    // Calculates the fitness of the individual according to the problem definition
    bool
        CIndividual::calculateFitness()
    {

        return true;
    }

    // Operator overload to use with sort method
    bool
        CIndividual::operator<(const CIndividual& other) const
    {
        return (m_solutionValue < other.getSolutionValue());
    }

    // Returns the solution value of the individual
    double
        CIndividual::getSolutionValue() const
    {
        return m_solutionValue;
    }

    // Gets a random element of the vector
    std::shared_ptr<flight::CFlight> 
        CIndividual::getRandomElement() const
    {
        // Creates a mersenne twister engine
        std::mt19937 gen(m_randSeed);

        // Creates an integer distribution
        std::uniform_int_distribution<> dis(0, m_genome.size() - 1);

        return m_genome[dis(gen)];
    }
}
