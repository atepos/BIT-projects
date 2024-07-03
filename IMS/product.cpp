/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Product process implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include "product.hpp"
#include "uniform_distribution.hpp"

Product::Product(Store *workers,
                 Store *CNC,
                 Store *milling_machine,
                 Facility *is_lunch_time,
                 Stat *product_loading_stats,
                 Stat *product_machining_time_stats,
                 Stat *product_milling_preparation_stats,
                 Stat *product_milling_time_stats,
                 Stat *product_finalizing_time_stats) : workers(workers),
                                                        CNC(CNC),
                                                        milling_machine(milling_machine),
                                                        is_lunch_time(is_lunch_time),
                                                        product_loading_stats(product_loading_stats),
                                                        product_machining_time_stats(product_machining_time_stats),
                                                        product_milling_preparation_stats(product_milling_preparation_stats),
                                                        product_milling_time_stats(product_milling_time_stats),
                                                        product_finalizing_time_stats(product_finalizing_time_stats) {}

void Product::Behavior()
{
    double product_loading_time = UniformDistribution::UniformDistr(PRODUCT_LOADING_TIME_AVG, PRODUCT_LOADING_TIME_DEVIATION);
    (*product_loading_stats)(product_loading_time);

    Wait(product_loading_time);

    Seize(*is_lunch_time);
    Release(*is_lunch_time);
    Enter(*CNC, 1);

    double product_machining_time = UniformDistribution::UniformDistr(PRODUCT_MACHINING_TIME_AVG, PRODUCT_MACHINING_TIME_DEVIATION);
    (*product_machining_time_stats)(product_machining_time);

    Wait(product_machining_time);

    Leave(*CNC, 1);

    double product_preparation_for_milling = UniformDistribution::UniformDistr(PRODUCT_PREPARING_FOR_MILLING_TIME_AVG, PRODUCT_PREPARING_FOR_MILLING_TIME_DEVIATION);
    (*product_milling_preparation_stats)(product_preparation_for_milling);

    Wait(product_preparation_for_milling);

    Seize(*is_lunch_time);
    Release(*is_lunch_time);
    Enter(*milling_machine, 1);

    double product_milling_time = UniformDistribution::UniformDistr(PRODUCT_MILLING_TIME_AVG, PRODUCT_MILLING_TIME_DEVIATION);
    (*product_milling_time_stats)(product_milling_time);

    Wait(product_milling_time);
    Leave(*milling_machine, 1);

    Seize(*is_lunch_time);
    Release(*is_lunch_time);
    if (Random() < 0.90)
    {
        double finalizing_time = UniformDistribution::UniformDistr(PRODUCT_FINALIZING_CORRECT_TIME_AVG, PRODUCT_FINALIZING_CORRECT_TIME_DEVIATION);
        (*product_finalizing_time_stats)(finalizing_time);
        Wait(finalizing_time);
    }
    else
    {
        double finalizing_time = UniformDistribution::UniformDistr(PRODUCT_FINALIZING_BAD_TIME_AVG, PRODUCT_FINALIZING_BAD_TIME_DEVIATION);
        (*product_finalizing_time_stats)(finalizing_time);
        Wait(finalizing_time);
    }

    Leave(*workers, 1);
}