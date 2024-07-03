/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Work timer event header implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#ifndef WORK_TIMER_HPP
#define WORK_TIMER_HPP

#include <simlib.h>
#include "work.hpp"

/**
 * @brief Class representing a timer for tracking work events during a work day.
 *
 * The WorkTimer class is derived from the Event class and is responsible for
 * managing the timing of work-related events, such as the end of the work shift
 * and the start of the lunch break.
 */
class WorkTimer : public Event
{
public:
    /**
     * @brief Constructor for WorkTimer.
     *
     * @param workDay Pointer to the associated WorkDay object.
     */
    explicit WorkTimer(WorkDay *workDay);

    /**
     * @brief Overridden behavior function that is executed when the event occurs.
     */
    void Behavior() override;

private:
    const double WORK_SHIFT_DURATION = 9.0 * 60.0; /**< Duration of a standard work shift in minutes. */

    WorkDay *workDay; /**< Pointer to the associated WorkDay object. */
};

#endif // WORK_TIMER_HPP
