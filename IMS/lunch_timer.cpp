/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Lunch timer Event implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include "lunch_timer.hpp"

using namespace std;

LunchTimer::LunchTimer(Facility *is_lunch_time) : is_lunch_time(is_lunch_time){}

void LunchTimer::Behavior()
{
    Seize(*is_lunch_time);
    Wait(LUNCH_TIME);
    Release(*is_lunch_time);
}