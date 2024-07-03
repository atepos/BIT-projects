/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Lunch timer Event header implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#ifndef LUNCH_TIMER_HPP
#define LUNCH_TIMER_HPP

#include <simlib.h>

/**
 * @class LunchTimer
 * @brief Represents a simulation process for tracking lunchtime in a manufacturing facility.
 *
 * This class extends the Process class from SIMLIB library and models the behavior of a lunch timer
 * in the simulation.
 */
class LunchTimer : public Process
{
public:
    /**
     * @brief Constructor for LunchTimer class.
     * @param is_lunch_time Pointer to the facility representing lunchtime.
     */
    explicit LunchTimer(Facility *is_lunch_time);

    /**
     * @brief Behavior function that defines the simulation logic for the lunch timer.
     */
    void Behavior() override;

private:
    const double LUNCH_TIME = 1.0 * 60.0; ///< Constant representing the duration of lunchtime.

    Facility *is_lunch_time; ///< Pointer to the facility representing lunchtime.
};

#endif // LUNCH_TIMER_HPP
