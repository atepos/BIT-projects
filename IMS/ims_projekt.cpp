/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Main file, start of the simulation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include <cstdlib>
#include <simlib.h>
#include <getopt.h>
#include <iostream>

#include "work.hpp"

#define RESET "\033[0m"
#define GREEN "\033[32m"

#define SIMULATION_START_TIME 0.0
#define SIMULATION_END_TIME 24.0 * 60.0

#define DEFAULT_NUMBER_OF_WORKERS 10
#define DEFAULT_NUMBER_OF_CNC 3
#define DEFAULT_NUMBER_OF_MILLING_MACHINES 7

#define DEFAULT_COUNT_OF_INCOMING_ORDER 29
#define DEFAULT_DEVIATION_OF_ORDERS 3.3

#define SIMULATION_TIMES 7

using namespace std;

int main()
{

    cout << "############################################################" << endl;
    cout << "\tSTART OF SIMULATION" << endl;
    cout << "############################################################" << endl;
    cout << "Experiments starts with " << GREEN << DEFAULT_NUMBER_OF_WORKERS << RESET << " count of workers." << endl;
    cout << "Experiments starts with " << GREEN << DEFAULT_NUMBER_OF_CNC << RESET << " count of CNC's." << endl;
    cout << "Experiments starts with " << GREEN << DEFAULT_NUMBER_OF_MILLING_MACHINES << RESET << " count of Milling machine's ." << endl;
    cout << "Experiments starts with " << GREEN << DEFAULT_COUNT_OF_INCOMING_ORDER << RESET << " count of order's." << endl;
    cout << "Experiments starts with " << GREEN << DEFAULT_DEVIATION_OF_ORDERS << RESET << " deviation of order's" << endl;
    cout << "Experiments starts with " << GREEN << SIMULATION_TIMES << RESET << " count of simulation start" << endl;

    cout << "############################################################" << endl;

    for (int i = 1; i <= SIMULATION_TIMES; i++)
    {

        cout << "\tExperiment number: " << i << endl;
        Init(SIMULATION_START_TIME, SIMULATION_END_TIME);

        (new WorkDay(DEFAULT_NUMBER_OF_WORKERS,
                     DEFAULT_COUNT_OF_INCOMING_ORDER,
                     DEFAULT_DEVIATION_OF_ORDERS,
                     DEFAULT_NUMBER_OF_CNC,
                     DEFAULT_NUMBER_OF_MILLING_MACHINES))
            ->Activate();
        Run();
        SIMLIB_statistics.Output();
        cout << "############################################################" << endl;
        cout << "\t End of Experiment number: " << i << endl;
        cout << "############################################################\n"
             << endl;
    }
    return EXIT_SUCCESS;
}