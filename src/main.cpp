#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>

#include "CFlight.hpp"
#include "CGeneticAlgorithm.hpp"

int main(int argc, char* argv[])
{
    std::vector<flight::CFlight> flights;
	std::ifstream infile(argv[1]);
	double appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime = 10;
	while(infile >> appTime >> minTime >> idealTime >> maxTime >> earlyCost >> lateCost)
    {
		flights.push_back(flight::CFlight(appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime));
    }

	int randSeed = std::strtol(argv[2], NULL, 10);
	// Set random seed
	std::srand(randSeed);
	//std::srand(time(NULL));

	clock_t start = clock();
	for (int i = 0; i < 10; i++)
	{
		GA::CGeneticAlgorithm geneticAlgorithm = GA::CGeneticAlgorithm(1, true, 50, 1000, flights);

		geneticAlgorithm.run(10);
	}
	double tMean = ((double)(clock() - start) / CLOCKS_PER_SEC) / 10.0;
	std::cout << "Tempo medio: " << tMean << " s" << std::endl;

	std::cin.get();

    return 0;
}