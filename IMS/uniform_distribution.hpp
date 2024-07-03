/**
 * VUT FIT IMS SHO project - Model vyroby v oblasti strojírenství
 *
 * @file Uniform distribution header implementation
 * @author Petr Kaška <xkaska01@stud.fit.vutbr.cz>
 * @author Martin Hemza <xhemza05@stud.fit.vutbr.cz>
 */

#ifndef UNIFORMDISTRIBUTION_HPP
#define UNIFORMDISTRIBUTION_HPP

/**
 * @class UniformDistribution
 * @brief Represents a static class for generating random numbers from a uniform distribution.
 *
 * This class provides a static method for generating random numbers following a uniform distribution.
 */
class UniformDistribution
{
public:
    /**
     * @brief Generates a random number from a uniform distribution.
     * @param avg The average value of the distribution.
     * @param dev The deviation of the distribution.
     * @return A random number following a uniform distribution.
     */
    static double UniformDistr(double avg, double dev);

private:
};

#endif // UNIFORMDISTRIBUTION_HPP
