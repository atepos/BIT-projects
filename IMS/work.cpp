/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Product process implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include <iostream>

#include "work_timer.hpp"
#include "work.hpp"
#include "uniform_distribution.hpp"
#include "product.hpp"
#include "lunch_timer.hpp"

using namespace std;

WorkDay::WorkDay(unsigned long workers, int average_orders_count, double order_deviation, int CNC_count, int milling_machine_count)
{
    this->workers = new Store("Workers-Store", workers);
    this->CNC = new Store("CNC-store", CNC_count);
    this->milling_machine = new Store("Milling_machine-Store", milling_machine_count);
    this->is_lunch_time = new Facility("Lunch_time-Facility");

    orders = new int(static_cast<int>(
        UniformDistribution::UniformDistr(average_orders_count, order_deviation)));

    product_loading_time_stats = new Stat("Preparing material for CNC");
    product_machining_time_stats = new Stat("CNC Machining");
    product_milling_preparation_stats = new Stat("Preparing for milling");
    product_milling_time_stats = new Stat("Milling");
    product_finalizing_time_stats = new Stat("Final adjustment");

    cout << "############################################################" << endl;
    cout << "Work day starts." << endl;
    cout << "\tStart time: " << Time << endl;
    cout << "\tOrder count: " << *orders << endl;
}

WorkDay::~WorkDay()
{
    cout << "Work day ends." << endl;
    cout << "\tEnd time: " << Time << endl;
    cout << "\tNumber of orders left: " << *orders << endl;

    milling_machine->Output();
    CNC->Output();
    workers->Output();
    product_loading_time_stats->Output();
    product_machining_time_stats->Output();
    product_milling_preparation_stats->Output();
    product_milling_time_stats->Output();
    product_finalizing_time_stats->Output();
    is_lunch_time->Output();

    delete orders;
    delete product_loading_time_stats;
    delete product_machining_time_stats;
    delete product_milling_preparation_stats;
    delete product_milling_time_stats;
    delete product_finalizing_time_stats;
}

void WorkDay::Behavior()
{
    auto *timer = new WorkTimer(this);
    (new LunchTimer(is_lunch_time))->Activate(Time + START_OF_LUNCH);

    while (*orders > 0)
    {

        Seize(*is_lunch_time);
        Release(*is_lunch_time);

        Enter(*workers, 1);

        if ((*orders) == 0)
        {
            Leave(*workers, 1);
            break;
        }

        (*orders)--;

        (new Product(workers,
                     CNC,
                     milling_machine,
                     is_lunch_time,
                     product_loading_time_stats,
                     product_machining_time_stats,
                     product_milling_preparation_stats,
                     product_milling_time_stats,
                     product_finalizing_time_stats))
            ->Activate();
    }

    Enter(*workers, workers->Capacity());
    Leave(*workers, workers->Capacity());

    delete timer;
}