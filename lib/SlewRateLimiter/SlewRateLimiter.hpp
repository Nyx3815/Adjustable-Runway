#ifndef SLEWRATE_LIMITER
#define SLEWRATE_LIMITER

#include <Arduino.h>

/*----------------------------------------------------------------------------------*/
/** @file   SlewRateLimiter.hpp
 * @brief   Header for SlewRateLimiter class (used to limit variable rate of change)
*//*--------------------------------------------------------------------------------*/


/**
 * @brief Class used to limit the maxiumum amount a numerical value change per second 
 */
class SlewRateLimiter {
  private:
    double maxIncrease; // Maximum positive change in the input per second
    double maxDecrease; // Maximum negative change in the input per second

    double currentTime; // Current time value in milliseconds 
    double lastTime;    // Time of the previous iteration
    double timeChange;  // Change in time since previous iteration

    double lastValue;   // Value of the number at the previous iteration
    double maxDelta;    // Maximum change in the value since the previous iteration
    double delta;       // Change in the value since the previous iteration    

  public:
    /**
     * @brief Define a new SlewRateLimiter given the maximum change in either direction 
     * 
     * @param maxChange Maximum change per second
     */
    SlewRateLimiter(double maxChange);


    /**
     * @brief Define a new SlewRateLimiter given the maximum change in positive and negative directions
     * 
     * @param maxPosChange Maximum positive change per second
     * @param maxNegChange Maximum negative change per second
     */
    SlewRateLimiter(double maxPosChange, double maxNegChange);


    /**
     * @brief Calculates the allowed change in value
     * 
     * @param targetValue Target value to reach
     * @return The new value with the allowed amount of change 
     */
    double calculate(double targetValue);


    /**
     * @brief Sets the maximum rate of change
     * 
     * @param rate Maximum amount the value can change by
     */
    void setRate(double rate);

    /**
     * @brief Sets the maximum rate of change
     * 
     * @param pos Maximum positive change
     * @param neg Maximum negative change
     */
    void setRate(double pos, double neg);
};


#endif // SLEWRATE_LIMITER