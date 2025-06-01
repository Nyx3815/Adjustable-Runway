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
  currentTime = millis();              // Get the current time
  timeChange = currentTime - lastTime; // Measure the change in time 
  delta = targetValue - lastValue;

  if (delta > 0) { // Code to run to calculate change if the difference is positive
    maxDelta = maxIncrease * (timeChange / 1000.0); // Get the maximum positive change per second
    delta = min(delta, maxDelta);
  } else if (delta < 0) { // Code to run to calculate change if the difference is negative
    maxDelta = maxDecrease * (timeChange / 1000.0); // Get the maximum negative change per second
    delta = max(delta, -maxDelta);
  }

  lastValue += delta;  // Add the change to the value 
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