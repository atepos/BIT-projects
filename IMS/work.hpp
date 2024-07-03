/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Product process header implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#ifndef WORKDAY_HPP
#define WORKDAY_HPP

#include <simlib.h>

/**
 * @class WorkDay
 * @brief Represents a simulation process for a typical workday in a manufacturing facility.
 *
 * This class extends the Process class from SIMLIB library and models the behavior of workers and machines
 * during a workday simulation.
 */
class WorkDay : public Process
{
public:
    /**
     * @brief Constructor for WorkDay class.
     * @param workers Number of workers in the facility.
     * @param average_orders_count Average number of orders to be processed.
     * @param order_deviation Deviation in the number of orders.
     * @param CNC_count Number of CNC machines.
     * @param milling_machine_count Number of milling machines.
     */
    WorkDay(unsigned long workers, int average_orders_count, double order_deviation, int CNC_count, int milling_machine_count);

    /**
     * @brief Destructor for WorkDay class.
     */
    ~WorkDay() override;

    /**
     * @brief Behavior function that defines the simulation logic for a workday.
     */
    void Behavior() override;

private:
    const double START_OF_LUNCH = 4.0 * 60.0; ///< Constant representing the start of lunchtime.

    Store *workers; ///< Store representing the pool of workers.

    Store *CNC;              ///< Store representing the pool of CNC machines.
    Store *milling_machine;  ///< Store representing the pool of milling machines.
    Facility *is_lunch_time; ///< Facility representing the lunchtime.

    int *orders; ///< Pointer to an array storing the number of orders.

    Stat *product_loading_time_stats;        ///< Statistics for product loading time.
    Stat *product_machining_time_stats;      ///< Statistics for product machining time.
    Stat *product_milling_preparation_stats; ///< Statistics for product milling preparation time.
    Stat *product_milling_time_stats;        ///< Statistics for product milling time.
    Stat *product_finalizing_time_stats;     ///< Statistics for product finalizing time.
};

#endif // WORKDAY_HPP
