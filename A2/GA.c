// Include everything necessary here 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main(int argc, char *argv[])
{
    // <YOUR CODE: Handle the possible errors in input data given by the user and say how to execute the code>
    if(argc != 6){
        printf("Incorrect number of inputs. Please enter 5 inputs in the order <POPULATION SIZE> <MAX GENERATIONS> <crossover_rate> <mutation _rate> <stop_criteria>\n");
        return 1;
    }
    //check types if necessary
    srand(time(NULL));

    // <YOUR CODE: Assign all inputs given by the user argv[i]> like
    // POPULATION_SIZE, MAX_GENERATIONS, crossover_rate, mutate_rate, stop_criteria
    int POPULATION_SIZE = atoi(argv[1]);
    int MAX_GENERATIONS = atoi(argv[2]);
    double crossover_rate = atof(argv[3]);
    double mutate_rate = atof(argv[4]);
    double stop_criteria = atof(argv[5]);


    // ###################################################################################
    // you dont need to change anything here
    // the number of variables
    int NUM_VARIABLES = 2;
    // the lower bounds of variables
    double Lbound[] = {-5.0, -5.0};
    // the upper bounds of variable
    double Ubound[] = {5.0, 5.0};
    // ###################################################################################

    // <YOUR CODE: Here make all the initial print outs>
    printf("Genetic Algorithm is initiated.\n");
    printf("--------------------------------------------------\n");
    printf("Lower bounds:[%.2f, %.2f]\n", Lbound[0], Lbound[1]);
    printf("Upper bounds:[%.2f, %.2f]\n", Ubound[0], Ubound[1]);
    printf("\n");

    printf("Population Size:  %d\n", POPULATION_SIZE);
    printf("Max Generations:  %d\n", MAX_GENERATIONS);
    printf("Crossover Rate:  %.2f\n", crossover_rate);
    printf("Mutation Rate:  %.2f\n", mutate_rate);
    printf("Stopping criteria: %.16f\n", stop_criteria);
    printf("\n");


    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    // <YOUR CODE: Declare all the arrays you need here>
    double population[POPULATION_SIZE][NUM_VARIABLES];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];


    double best_solution[NUM_VARIABLES];
    double prevbestfit=99; 
    // <YOUR CODE: Call generate_population function to initialize the "population"> like:
    // generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);
    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    // iteration starts here. The loop continues until MAX_GENERATIONS is reached
    // Or stopping criteria is met
    int index;
    for (int generation = 0; generation < MAX_GENERATIONS; generation++)
    {
        // <YOUR CODE: Compute the fitness values using objective function for
        // each row in "population" (each set of variables)> like:
        // compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);
        compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);

        // <YOUR CODE: Here implement the logic of finding best solution with minimum fitness value
        // and the stopping criteria>
        index = 0;
        for(int i =1; i < POPULATION_SIZE; ++i){
            if(fitness[i] < fitness[index]){
                index = i;
            }
        }

        for(int i=0; i < NUM_VARIABLES; ++i){
            best_solution[i] = population[index][i];
        }
        if(generation % 500 == 0 && generation != 0){
            if((fitness[index]-prevbestfit) < stop_criteria){
                break;
            }
        }
        // <YOUR CODE: Here call the crossover function>
        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);

        // <YOUR CODE: Here call the mutation function>
        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);

        prevbestfit = fitness[index];

        // Now you have the a new population, and it goes to the beginning of loop to re-compute all again
    }

    // <YOUR CODE: Jump to this part of code if the stopping criteria is met before MAX_GENERATIONS is met>


    // ###################################################################################
    // You dont need to change anything here
    // Here we print the CPU time taken for your code
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("CPU time: %f seconds\n", cpu_time_used);
    // ##################################################################################

    // <Here print out the best solution and objective function value for the best solution like the format>
    //print statements have to be changed if using another objective function
    printf("Best solution found is: (");
    for(int i=0; i < NUM_VARIABLES; ++i){
        if(i != NUM_VARIABLES-1){         
            printf("%.16f, ", best_solution[i]);
        }
        else{
            printf("%.16f)\n", best_solution[i]);
        }
    }
    printf("Best fitness: %.16f\n", fitness[index]);

    return 0;
}
