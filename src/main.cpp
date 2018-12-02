#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#include "CFlight.hpp"
#include "CGeneticAlgorithm.hpp"

std::vector<std::shared_ptr<flight::CFlight>> parseInput(std::string filename);

int main(int argc, char* argv[])
{
    std::vector<std::shared_ptr<flight::CFlight>> flights;
    for(int i = 0; i < 10; i++)
    {
        double appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime;
        std::string input = "";
        getline(std::cin, input);
        std::stringstream myString(input);
        if(myString >> appTime >> minTime >> idealTime >> maxTime >> earlyCost >> lateCost)
        {
            //std::cout << appTime  << " " << minTime  << " " << idealTime  << " " << maxTime  << " " << earlyCost  << " " << lateCost << std::endl;
            flights.push_back(std::make_shared<flight::CFlight>(appTime, minTime, idealTime, maxTime, earlyCost, lateCost, freezeTime));
        }
    }

    //std::string filepath(argv[1]);

    //flights = parseInput(filepath);

    GA::CGeneticAlgorithm geneticAlgorithm = GA::CGeneticAlgorithm(0, true, 10, 1000, flights);

    geneticAlgorithm.run(10);

    return 0;
}

std::vector<std::shared_ptr<flight::CFlight>>
    parseInput(std::string filename)
{
    std::vector<std::shared_ptr<flight::CFlight>> readValues;
    std::ifstream infile(filename);

    double appTime, minTime, idealTime, maxTime, earlyCost, lateCost;

    int numOfPlanes, freezeTime;

    std::string line;
    while(std::getline(infile, line))
    {
        std::istringstream iss(line);
        if((iss >> numOfPlanes >> freezeTime))
        {
            if((iss >> appTime >> minTime >> idealTime >> maxTime >> earlyCost >> lateCost))
            {
                std::cout << appTime  << " " << minTime  << " " << idealTime  << " " << maxTime  << " " << earlyCost  << " " << lateCost << std::endl;
                readValues.push_back(std::make_shared<flight::CFlight>(
                            appTime,
                            minTime,
                            idealTime,
                            maxTime,
                            earlyCost,
                            lateCost,
                            freezeTime));
            }
        }
        else
        {
            std::cout << numOfPlanes << " " << freezeTime << std::endl;
        }
    }

    return readValues;
}
