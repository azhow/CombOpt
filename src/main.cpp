#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#include "CFlight.hpp"
#include "CGeneticAlgorithm.hpp"

int main(int argc, char* argv[])
{
    std::vector<flight::CFlight> flights;
    for(int i = 0; i < 10; i++)
    {
        double appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime = 10;
        std::string input = "";
        getline(std::cin, input);
        std::stringstream myString(input);
        if(myString >> appTime >> minTime >> idealTime >> maxTime >> earlyCost >> lateCost)
        {
            flights.push_back(flight::CFlight(appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime));
        }
    }

    //std::string filepath(argv[1]);

    //flights = parseInput(filepath);

	// Set random seed
	std::srand(1000);

    GA::CGeneticAlgorithm geneticAlgorithm = GA::CGeneticAlgorithm(1, true, 50, 1000, flights);

    geneticAlgorithm.run(10);

	std::cin.get();

    return 0;
}