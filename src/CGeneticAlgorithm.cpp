#include <cmath>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "CGeneticAlgorithm.hpp"
#include "CFlight.hpp"

// Structure to use in the std::sort method
struct less_than_solution
{
	inline bool operator() (
		const GA::CIndividual& indiv1,
		const GA::CIndividual& indiv2)
	{
		return (indiv1.getSolutionValue() < indiv2.getSolutionValue());
	}
};

namespace GA
{
	CGeneticAlgorithm::CGeneticAlgorithm(
		double mutationRate,
		bool debugFlag,
		std::size_t populationSize,
		unsigned int randSeed,
		std::vector<flight::CFlight> flights) :
		m_mutationRate(mutationRate),
		m_debugFlag(debugFlag),
		m_populationSize(populationSize),
		m_randSeed(randSeed)
	{
		// Initialize population
		for (std::size_t i = 0; i < m_populationSize; i++)
		{
			// Create individual
			CIndividual pIndividual = CIndividual(flights, m_randSeed, true);
			// Adds individual to population
			m_population.push_back(pIndividual);
		}

		calculatePopulationFitness();
		// Sorts population on ascending order based on their fitness
		std::sort(m_population.begin(), m_population.end(), less_than_solution());
		std::cout << "Initial: " << m_population[0].getSolutionValue() << std::endl;
	}

	// Default destructor
	CGeneticAlgorithm::~CGeneticAlgorithm()
	{
	}

	// Runs the selection phase (Rank selection)
	int
		CGeneticAlgorithm::selection()
	{
		int sumRanks = (m_population.size() + 1)*(m_population.size()/2);
		double random = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
		double acc = 0;

		int i = 1;
		for (auto it = m_population.rbegin(); it != m_population.rend(); ++it)
		{
			acc += static_cast<double>(i) / sumRanks;
			if(!(acc >= random))
			{
				i++;
			}
			else
			{
				break;
			}
		}

		return m_population.size() - i;
	}

	// Does the crossover phase (One point crossover)
	bool
		CGeneticAlgorithm::crossover(int parent1Idx, int parent2Idx, std::vector<CIndividual>& newGen)
	{
		if (static_cast<double>(std::rand()) / RAND_MAX < 0.5)
		{
			int point = std::rand() % m_population[parent1Idx].getGenome().size();
			std::vector<flight::CFlight> p1Genome = m_population[parent1Idx].getGenome();
			std::vector<flight::CFlight> p2Genome = m_population[parent2Idx].getGenome();

			std::vector<flight::CFlight> firstHalf;
			for (int i = 0; i < point; i++)
			{
				firstHalf.push_back(p1Genome[i]);
			}
			std::vector<flight::CFlight> secondHalf = m_population[parent2Idx].getGenome();
			firstHalf.reserve(m_population[parent1Idx].getGenome().size());
			firstHalf.insert(firstHalf.end(), secondHalf.begin() + point, secondHalf.end());

			newGen.push_back(CIndividual(firstHalf, m_randSeed, false));

			std::vector<flight::CFlight> thirdHalf;
			for (int i = 0; i < point; i++)
			{
				thirdHalf.push_back(p2Genome[i]);
			}
			firstHalf = m_population[parent1Idx].getGenome();
			thirdHalf.reserve(m_population[parent1Idx].getGenome().size());
			thirdHalf.insert(thirdHalf.end(), firstHalf.begin() + point, firstHalf.end());

			newGen.push_back(CIndividual(thirdHalf, m_randSeed, false));
		}
		else
		{
			newGen.push_back(m_population[parent1Idx]);
			newGen.push_back(m_population[parent2Idx]);
		}

		return true;
	}

	// Calculates the fitness of each individual in the population
	bool
		CGeneticAlgorithm::calculatePopulationFitness()
	{
		// Iterates through all individuals
		for (auto it = m_population.begin(); it != m_population.end(); ++it)
		{
			(*it).calculateFitness();
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
		while (numOfGenerationsWithoutImprovement < numOfGenerationsToConvergeWithoutImprovement)
		{
			// Do the main operations
			currentValue = evolve();
			if (currentValue < bestValue)
			{
				bestValue = currentValue;
				numOfGenerationsWithoutImprovement = 0;
			}
			else
			{
				numOfGenerationsWithoutImprovement++;
			}

			if (m_debugFlag)
			{
				//std::cout << "Gen: " << generation << std::endl;
				//std::cout << "\tBest: " << bestValue << "\n\tCurrent best: " << currentValue << std::endl;
			}
			generation++;
			m_mutationRate *= 0.95;
		}
		std::cout << "Best: " << bestValue << std::endl;

		return true;
	}

	// Mutates the whole population
	bool
		CGeneticAlgorithm::mutatePopulation()
	{
		for (auto it = m_population.begin(); it != m_population.end(); ++it)
		{
			(*it).mutate(m_mutationRate);
		}

		return true;
	}

	// Works with the intermediate population and in the end generates the new population
	double
		CGeneticAlgorithm::evolve()
	{
		std::vector<CIndividual> newGen;

		// Each crossover adds 2 to the population
		for (int i = 0; i < m_populationSize/2; i++)
		{
			crossover(selection(), selection(), newGen);
		}
	
		m_population = newGen;

		mutatePopulation();

		// Updates the fitness
		calculatePopulationFitness();

		// Sorts the population based on the solution value in ascending order
		std::sort(m_population.begin(), m_population.end(), less_than_solution());

		// Returns the solution with the lowest value
		return m_population[0].getSolutionValue();
	}

	// Gets a random element of the vector
	CIndividual
		CGeneticAlgorithm::getRandomElement() const
	{
		return m_population[std::rand() % m_population.size()];
	}

	void
		CGeneticAlgorithm::printPopulationFitness() const
	{
		for (auto p : m_population)
		{
			std::cout << p.getSolutionValue() << " ";
		}
		std::cout << std::endl;
	}
}