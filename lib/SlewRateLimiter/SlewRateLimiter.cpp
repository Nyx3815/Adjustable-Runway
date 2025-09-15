#include "SlewRateLimiter.hpp"

SlewRateLimiter::SlewRateLimiter(double maxChange) {
  maxIncrease = maxDecrease = maxChange;

  currentTime = millis();
  lastTime = millis();
}


SlewRateLimiter::SlewRateLimiter(double maxPosChange, double maxNegChange) {
  maxIncrease = maxPosChange;
  maxDecrease = maxNegChange;

  currentTime = millis();
  lastTime = millis();
}


double SlewRateLimiter::calculate(double targetValue) {
  currentTime = millis();               // Get the current time
  timeChange = currentTime - lastTime;  // Measure the change in time
  
  lastValue += constrain(               // Constrain the value change
    targetValue - lastValue,            // Change in value since previous iteration 
    -maxDecrease * (timeChange / 1000), // Maximum decrease value 
    maxIncrease * (timeChange / 1000)   // Maximum increase value
  );

  lastTime = millis(); // Record the time at the end of the current iteration

  return lastValue;
}


void SlewRateLimiter::setRate(double rate) {
  maxIncrease = maxDecrease = rate;
}


void SlewRateLimiter::setRate(double pos, double neg) {
  maxIncrease = pos;
  maxDecrease = neg;
}