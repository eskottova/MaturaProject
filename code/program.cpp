#include "Input.hpp"

#include "Parameters.hpp"
#include "SimulatedAnnealing.hpp"
#include "GeneticAlgorithm.hpp"
#include "Solution.hpp"

class Event;

signed main()
{
    Solution* input_solution = new Solution();

    SimulatedAnnealing* SA = new SimulatedAnnealing();
    SA->optimize_solution(input_solution->get_input());
    Solution* sSA = SA->get_best();
    
    GeneticAlgorithm* GA = new GeneticAlgorithm();
    GA->optimize_solution(input_solution->get_input());
    Solution* sGA = GA->get_best();
    
    sSA->print();
    sGA->print();

}
