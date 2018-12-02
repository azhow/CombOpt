#include <cmath>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "CGeneticAlgorithm.hpp"
#include "CFlight.hpp"

#define MAX_RAND 20000

// Structure to use in the std::sort method
struct less_than_solution
{
	inline bool operator() (
		const std::shared_ptr<GA::CIndividual>& indiv1,
		const std::shared_ptr<GA::CIndividual>& indiv2)
	{
		return (indiv1->getSolutionValue() < indiv2->getSolutionValue());
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
			std::shared_ptr<CIndividual> pIndividual = std::make_shared<CIndividual>(
				flights,
				m_randSeed,
				true);
			// Adds individual to population
			m_population.push_back(pIndividual);
		}

		calculatePopulationFitness();
	}

	// Default destructor
	CGeneticAlgorithm::~CGeneticAlgorithm()
	{
	}

	// Runs the selection phase (Stochastic Universal Selection)
	bool
		CGeneticAlgorithm::selection()
	{
		double populationTotalFitness = 0;
		for (auto indiv : m_population)
		{
			populationTotalFitness += indiv->getSolutionValue();
		}
		double distanceBetweenPoints = populationTotalFitness/m_populationSize;

		// Start point of the selection
		double start = distanceBetweenPoints * (static_cast<double>(std::rand())/ RAND_MAX);
		// Selection points vector
		std::vector<double> points;

		for(std::size_t i = 0; i < 2; i++)
		{
			points.push_back(start + i * distanceBetweenPoints);
		}

		// Individuals chosen in the process
		std::vector<std::shared_ptr<CIndividual>> selectedIndividuals;

		rouletteWheelSelection(points, selectedIndividuals);

		// Update population
		m_population = selectedIndividuals;
		/*
		// Individuals chosen in the process
		std::vector<std::shared_ptr<CIndividual>> selectedIndividuals;
	
		// Tounament selection (selects the best of 5 samples 10 times)
		for (int j = 0; j < 10; j++)
		{
			std::vector<std::shared_ptr<CIndividual>> contenders;
			for (int i = 0; i < 5; i++)
			{
				contenders.push_back(getRandomElement());
			}
			std::sort(contenders.begin(), contenders.end(), less_than_solution());
			selectedIndividuals.push_back(*(contenders.end() - 1));
		}
		// Update population
		m_population = selectedIndividuals;
		*/
		printPopulationFitness();

		return true;
	}

	// Roulette wheel selection
	bool
		CGeneticAlgorithm::rouletteWheelSelection(std::vector<double> points, std::vector<std::shared_ptr<CIndividual>>& selectedIndividuals)
	{
		for (double p : points)
		{
			std::size_t index = 0;
			double accumulator = 0;
			while (accumulator < p)
			{
				accumulator += m_population[index]->getSolutionValue();
				index++;
			}
			selectedIndividuals.push_back(m_population[index]);
		}

		return true;
	}

	// Does the crossover phase (One point crossover)
	bool
		CGeneticAlgorithm::crossover()
	{
		int popInitialSize = m_population.size();

		std::vector<std::shared_ptr<CIndividual>> newGen;
		// Creates offspring and add to population
		for (std::size_t i = 0; i < m_populationSize; i++)
		{
			// Select the first parent
			std::shared_ptr<CIndividual> parent1 = getRandomElement();
			// Select the second parent
			std::shared_ptr<CIndividual> parent2 = getRandomElement();
			// Crossover point
			int point = std::rand() % parent1->getGenome().size();

			// New genome
			std::vector<flight::CFlight> genome;
			// Genome of the first parent
			std::vector<std::shared_ptr<flight::CFlight>> p1genome = parent1->getGenome();
			// Genome of the second parent
			std::vector<std::shared_ptr<flight::CFlight>> p2genome = parent2->getGenome();

			// Pre-allocate vector memory
			genome.reserve(parent1->getGenome().size());

			// Inserts the slice of the first parent genome
			for (auto it = p1genome.begin(); it != p1genome.begin() + point; ++it)
			{
				flight::CFlight el = flight::CFlight((*it)->mp_appTime,
					(*it)->mp_minTime,
					(*it)->mp_idealTime,
					(*it)->mp_maxTime,
					(*it)->mp_earlyCost,
					(*it)->mp_lateCost,
					(*it)->mp_timeBetweenFlights);

				el.mp_actualLandingTime = (*it)->mp_actualLandingTime;
				genome.push_back(el);
			}

			// Inserts the slice of the first parent genome
			for (auto it = p2genome.begin() + point; it != p2genome.end(); ++it)
			{
				flight::CFlight el = flight::CFlight((*it)->mp_appTime,
					(*it)->mp_minTime,
					(*it)->mp_idealTime,
					(*it)->mp_maxTime,
					(*it)->mp_earlyCost,
					(*it)->mp_lateCost,
					(*it)->mp_timeBetweenFlights);

				el.mp_actualLandingTime = (*it)->mp_actualLandingTime;
				genome.push_back(el);
			}

			newGen.push_back(std::make_shared<CIndividual>(genome, m_randSeed, false));
		}

		m_population = newGen;
		return true;
	}

	// Calculates the fitness of each individual in the population
	bool
		CGeneticAlgorithm::calculatePopulationFitness()
	{
		// Iterates through all individuals
		for (auto it = m_population.begin(); it != m_population.end(); ++it)
		{
			(*it)->calculateFitness();
		}

		return true;
	}

	// Runs the learning algorithm
	bool
		CGeneticAlgorithm::run(const unsigned int numOfGenerationsToConvergeWithoutImprovement)
	{
		// Best value/solution starts as +infinite
		double bestValue = -std::numeric_limits<double>::infinity();
		// Current generation best value/solution starts as +infinite
		double currentValue = -std::numeric_limits<double>::infinity();
		// Number of generations without real improvement in the answer
		unsigned int numOfGenerationsWithoutImprovement = 0;
		// Current generation
		unsigned int generation = 0;

		// Main loop of the algorithm
		while (numOfGenerationsWithoutImprovement < numOfGenerationsToConvergeWithoutImprovement)
		{
			// Do the main operations
			currentValue = evolve();
			if (currentValue > bestValue)
			{
				bestValue = currentValue;
				numOfGenerationsWithoutImprovement = 0;
			}
			else
			{
				(numOfGenerationsWithoutImprovement++);
			}
			if (m_debugFlag)
			{
				std::cout << "Gen: " << generation << std::endl;
				std::cout << "\tBest: " << std::pow(bestValue, -1) << "\n\tCurrent best: " << std::pow(currentValue, -1) << std::endl;
			}
			generation++;
		}

		return true;
	}

	// Mutates the whole population
	bool
		CGeneticAlgorithm::mutatePopulation()
	{
		for (auto it = m_population.begin(); it != m_population.end(); ++it)
		{
			double i = static_cast<double>(std::rand()) / MAX_RAND;
			if (i < m_mutationRate)
			{
				(*it)->mutate();
			}
		}

		return true;
	}

	// Works with the intermediate population and in the end generates the new population
	double
		CGeneticAlgorithm::evolve()
	{
		if (m_debugFlag && false)
		{
			std::cout << "Starting selection" << std::endl;
		}
		selection();
		if (m_debugFlag && false)
		{
			std::cout << "Starting crossover" << std::endl;
		}
		crossover();
		if (m_debugFlag && false)
		{
			std::cout << "Starting mutation" << std::endl;
		}
		mutatePopulation();
		if (m_debugFlag && false)
		{
			std::cout << "Recalculating fitness of the population" << std::endl;
		}
		// Updates the fitness
		calculatePopulationFitness();

		// Sorts the population based on the solution value in ascending order
		std::sort(m_population.begin(), m_population.end(), less_than_solution());

		// Returns the solution with the highest value
		//printPopulationFitness();
		return (*(m_population.end() - 1))->getSolutionValue();
		//return m_population[0]->getSolutionValue();
	}

	// Gets a random element of the vector
	std::shared_ptr<CIndividual>
		CGeneticAlgorithm::getRandomElement() const
	{
		return m_population[std::rand() % m_population.size()];
	}

	void
		CGeneticAlgorithm::printPopulationFitness() const
	{
		for (auto p : m_population)
		{
			std::cout << std::pow(p->getSolutionValue(), -1) << " ";
		}
		std::cout << std::endl;
	}
}
