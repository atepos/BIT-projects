/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Work timer event implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include <iostream>
#include "work_timer.hpp"

using namespace std;

WorkTimer::WorkTimer(WorkDay *workDay) : workDay(workDay)
{
    Activate(Time + WORK_SHIFT_DURATION);
}

void WorkTimer::Behavior()
{

    cout << "ERROR: end of the work day, due to the time out" << endl;

    delete workDay;
}