#include "CGeneticAlgorithm.hpp"

int main()
{
    GA::CGeneticAlgorithm geneticAlgorithm = GA::CGeneticAlgorithm(0, false, 10, 0);

    geneticAlgorithm.run(10);

    return 0;
}
