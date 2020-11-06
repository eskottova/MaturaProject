#include "GeneticAlgorithm.hpp"
#include "random_generator.hpp"
#include "Input.hpp"
#include "Solution.hpp"
#include "Parameters.hpp"

using std::pair;

auto compare_solptr = [](auto const& lhs, auto const& rhs)
{
    return lhs->val < rhs->val;
};    

vector<pair<Parameters*, Parameters*>> GeneticAlgorithm::selection()
{
    int valsum = 0;
    for(auto individual : this->population) valsum += individual->val;
    
    vector<int> parents(2 * this->num_offspring);
    for(int i = 0; i < 2 * this->num_offspring; i ++) parents[i] = randint(0, valsum);

    sort(parents.begin(), parents.end());

    vector<pair<Parameters*, Parameters*>> parent_pars(num_offspring);

    int i = 0;
    int j = 0;
    int curvalsum = 0;
    while(i < population_size)
    {
        curvalsum += population[i]->val;
        while(j < 2 * this->num_offspring && curvalsum > parents[j])
        {
            if(j % 2 == 0) parent_pars[j / 2].first = population[i]->get_pars();
            else parent_pars[j / 2].second = population[i]->get_pars();
            j ++;
        }
        i ++;
    }
    
    return parent_pars;
}

vector<Parameters*> GeneticAlgorithm::crossing(vector<pair<Parameters*, Parameters*>> parent_pars)
{
    vector<Parameters*> offspring_pars(num_offspring);

    int chrom_size = parent_pars[0].first->pars.size();

    for(int i = 0; i < num_offspring; i ++)
    {
        int cutoff = randint(0, chrom_size);

        offspring_pars[i] = new Parameters;

        for(int gene = 0; gene < chrom_size; gene ++)
        {
            if(gene < cutoff) offspring_pars[i]->pars[gene] = parent_pars[i].first->pars[gene];
            else offspring_pars[i]->pars[gene] = parent_pars[i].second->pars[gene];
        }
    }
    return offspring_pars;
}

vector<Parameters*> GeneticAlgorithm::mutation(vector<Parameters*> offspring_pars)
{
    int chrom_size = offspring_pars[0]->pars.size();
    for(int i = 0; i < num_mutations; i ++)
    {
        offspring_pars[randint(0, this->num_offspring)]->random_par(randint(0, chrom_size));
    }
    return offspring_pars;
}

void GeneticAlgorithm::compute_offspring(vector<Parameters*> offspring_pars)
{
    vector<Solution*> offspring(num_offspring);
    for(int i = 0; i < num_offspring; i ++) offspring[i] = new Solution(this->input, offspring_pars[i]);

    sort(offspring.begin(), offspring.end(), compare_solptr);
    sort(this->population.begin(), this->population.end(), compare_solptr);

    int i = 0;
    while(i < num_replaced && population[i] < offspring[this->num_offspring - 1 - i])
    {
        delete population[i];
        population[i] = offspring[this->num_offspring - i - 1];
        i++;
    }
    for(;i < num_offspring; i++) delete offspring[this->num_offspring - i - 1];
}

void GeneticAlgorithm::optimize_solution(Solution::Input* input)
{
    this->input = input;

    //create population
    this->population.resize(population_size);
    for(int i = 0; i < population_size; i ++)
    {
        Parameters* para = new Parameters();
        this->population[i] = new Solution(this->input, para);

    }

    // compute all generations
    int generation = 0;
    while(generation < this->num_gens)
    {
        this->compute_offspring(this->mutation(this->crossing(this->selection())));
        generation ++;
    }

    //set best solution
    this->bests = this->population[0];
    for(auto individual : population) if(individual->val > bests->val) bests = individual;
}
