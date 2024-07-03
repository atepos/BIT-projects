/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Product process header implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include "simlib.h"

/**
 * @class Product
 * @brief Represents a simulation process for the production of a product in a manufacturing facility.
 *
 * This class extends the Process class from SIMLIB library and models the behavior of a product during
 * the production process, including loading, machining, milling, and finalizing.
 */
class Product : public Process
{
public:
    /**
     * @brief Constructor for the Product class.
     * @param workers Pointer to the pool of workers.
     * @param CNC Pointer to the pool of CNC machines.
     * @param milling_machine Pointer to the pool of milling machines.
     * @param is_lunch_time Pointer to the facility representing lunchtime.
     * @param product_loading_stats Statistics for product loading time.
     * @param product_machining_time_stats Statistics for product machining time.
     * @param product_milling_preparation_stats Statistics for product milling preparation time.
     * @param product_milling_time_stats Statistics for product milling time.
     * @param product_finalizing_time_stats Statistics for product finalizing time.
     */
    Product(Store *workers,
            Store *CNC,
            Store *milling_machine,
            Facility *is_lunch_time,
            Stat *product_loading_stats,
            Stat *product_machining_time_stats,
            Stat *product_milling_preparation_stats,
            Stat *product_milling_time_stats,
            Stat *product_finalizing_time_stats);

    /**
     * @brief Behavior function that defines the simulation logic for the product.
     */
    void Behavior() override;

private:
    const double PRODUCT_LOADING_TIME_AVG = 29.5;        ///< Average time for product loading.
    const double PRODUCT_LOADING_TIME_DEVIATION = 13.24; ///< Deviation in product loading time.

    const double PRODUCT_MACHINING_TIME_AVG = 40.0;       ///< Average time for product machining.
    const double PRODUCT_MACHINING_TIME_DEVIATION = 4.77; ///< Deviation in product machining time.

    const double PRODUCT_PREPARING_FOR_MILLING_TIME_AVG = 12.5;        ///< Average time for preparing for milling.
    const double PRODUCT_PREPARING_FOR_MILLING_TIME_DEVIATION = 34.22; ///< Deviation in preparing for milling time.

    const double PRODUCT_MILLING_TIME_AVG = 32.0;       ///< Average time for product milling.
    const double PRODUCT_MILLING_TIME_DEVIATION = 5.89; ///< Deviation in product milling time.

    const double PRODUCT_FINALIZING_CORRECT_TIME_AVG = 6.5;         ///< Average time for correct product finalizing.
    const double PRODUCT_FINALIZING_CORRECT_TIME_DEVIATION = 18.75; ///< Deviation in correct product finalizing time.

    const double PRODUCT_FINALIZING_BAD_TIME_AVG = 17.5;       ///< Average time for incorrect (bad) product finalizing.
    const double PRODUCT_FINALIZING_BAD_TIME_DEVIATION = 16.6; ///< Deviation in incorrect (bad) product finalizing time.

    Store *workers;          ///< Pointer to the pool of workers.
    Store *CNC;              ///< Pointer to the pool of CNC machines.
    Store *milling_machine;  ///< Pointer to the pool of milling machines.
    Facility *is_lunch_time; ///< Pointer to the facility representing lunchtime.

    Stat *product_loading_stats;             ///< Statistics for product loading time.
    Stat *product_machining_time_stats;      ///< Statistics for product machining time.
    Stat *product_milling_preparation_stats; ///< Statistics for product milling preparation time.
    Stat *product_milling_time_stats;        ///< Statistics for product milling time.
    Stat *product_finalizing_time_stats;     ///< Statistics for product finalizing time.
};

#endif // PRODUCT_HPP
