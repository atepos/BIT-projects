/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Uniform distribution implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#include <simlib.h>
#include "uniform_distribution.hpp"

double UniformDistribution::UniformDistr(double avg, double dev)
{
    return Uniform(avg - (dev / 100.0 * avg), avg + (dev / 100.0 * avg));
}