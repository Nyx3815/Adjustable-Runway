#ifndef PID_COMMAND
#define PID_COMMAND

#include <Arduino.h>

/*-----------------------------------------------------------------------------*/
/** @file    PidCommand.hpp
  * @brief   Header for PidCommand class (used to command multiple systems)
*//*---------------------------------------------------------------------------*/


/**
 * @brief Class used to create and control PID commands 
 */
class PidCommand {
  private:
    static int commands;      // Number of defined PID commands
    int commandID = 1;        // ID of the current PID command 

    double _kP;               // Proportional gain
    double _kI;               // Integral gain 
    double _kD;               // Derivative gain 

    double *_input;           // Input variable pointer 
    double *_output;          // Output variable pointer
    double *_setpoint;        // Setpoint variable pointer

    double kIntegrationLimit; // Value of error to begin adding the integral term 

    double finishedValue;     // Value of error rate for the PID command to be considered finished
    
    double error;             // Difference between setpoint and current value
    double errorSum;          // Integral of the error with respect to time
    double errorRate;         // Derivative of the error with respect to time
    double lastError;         // Error from the previous iteration 

    double deltaT;            // Time since last iteration 
    double lastTimestamp;     // Current timestamp 

    double minOutput;         // Minimum output of the PID command as a percentage 
    double maxOutput;         // Maximum output of the PID command as a percentage
    double outputRange[2];    // Output range for the PID command as percentages in the form {min, max}

    bool isStopped = false; 
    bool consoleOutput = false;
    bool hasOutputMethod = false;

    double (*timeFunc)();     // Timing function of the PID command 
    void (*displayMethod)();  // Method used to display output values if assigned
  public: 
    /**
     * @brief Defines a new PID command with a specified output range 
     * 
     * @note Defaults to just proportional control if no integral or derivative constants are given 
     * 
     * @param in Pointer to a double for the input value 
     * @param out Pointer to a double for the output value
     * @param set Pointer to a double for the setpoint value 
     * @param outRange Range of output values as percentages in the form {min, max}
     * @param func Timing function for the PID command (Must return value in seconds)
     * @param kP Propotional gain 
     * @param kI Integral gain (Defaults to 0)
     * @param kD Derivative gain (Defaults to 0)
     */
    PidCommand(double *in, double *out, double *set, double (&outRange)[2], double (*func)(), double kP, double kI = 0, double kD = 0);


    /**
     * @brief Defines a new PID command with default output range of [-100, 100]
     * 
     * @note Defaults to just proportional control if no integral or derivative constants are given 
     * 
     * @param in Pointer to a double for the input value 
     * @param out Pointer to a double for the output value 
     * @param set Pointer to a double for the setpoint value
     * @param func Timing function 
     * @param kP Proportional gain 
     * @param kI Integral gain (Default 0)
     * @param kD Derivative gain (Default 0)
     */
    PidCommand(double *in, double *out, double *set, double (*func)(), double kP, double kI = 0, double kD = 0);


    /**
     * @brief Calculates and sets the output value for the PID command 
     */
    void calculate();


    /**
     * @brief Sets the integration limit for the PID command
     * 
     * @param limit New integration limit
     */
    void setIntegrationLimit(double limit);

    
    /**
     * @brief Stops the PID command 
     * 
     * @note When this method is called, the PID command can't be restarted until the Arduino is reset
     */
    void eStop();


    /**
     * @brief Gets the error of the PID command 
     * 
     * @return The value of the error in the current iteration 
     */
    double getError();


    /**
     * @brief Gets the integral of the error with respect to time of the PID command 
     * 
     * @return The value of errorSum of the current iteration 
     */
    double getErrorSum();


    /**
     * @brief Gets the derivative of the error with respect to time of the PID command 
     * 
     * @return The value of errorRate of the current iteration 
     */
    double getErrorRate();


    /**
     * @brief Gets the ID of the current PID command 
     * 
     * @return The value of the current PID command ID
     */
    int getCommandID();


    /**
     * @brief Checks if the PID command is within an acceptable error rate 
     * 
     * @return Condition for whether or not the PID command is at the setpoint
     */
    bool atSetpoint();


    /**
     * @brief Checks if the PID command is within a given acceptable error rate 
     * 
     * @param threshold Threshold value for the PID command to be considered finished 
     * @return Condition for whether or not the PID command is at the setpoint
     */
    bool atSetpoint(double threshold);


    /**
     * @brief Sets the value to determine whether or not the system is done running
     * 
     * @param value New finished value for the PID command 
     */
    void setFinishedValue(double value);


    /**
     * @brief Sets the timing function for the PID command to call
     * 
     * @param func Pointer to a function to call for timing 
     */
    void setTimingFunction(double (*func)());


    /**
     * @brief Sets whether ot not the PID command sends output values to the console 
     * 
     * @param sendOutput Condition for if the PID command sends output (Default true)
     */
    void sendConsoleOutput(bool sendOutput = true);


    /**
     * @brief Sets whether or not the PID command sends output values to the console and sets a function to do so
     * 
     * @param displayFunc Function to use to display output
     * @param sendOutput Condition for if the PID command sends output (Default true)
     */
    void sendConsoleOutput(void (*displayFunc)(), bool sendOutput = true);


    /**
     * @brief Displays values to the Serial Plotter via VSC Teleplot extension
     * 
     * @note Displays a graph of the setpoint, error, error sum, error rate, and current position
     */
    void display();


    /**
     * @brief Constrains a value between a minimum and maxium 
     * 
     * @tparam T Data type of the value to be constrained (double, int, etc.)
     * @param x Value to be constrained 
     * @param min Minimum value 
     * @param max Maximum value
     * @return Constrained value
     */
    template <class T>
    inline static T constrainOutput(T x, T min, T max);


    /**
     * @brief Constrains a value between a minimum and maximum 
     * 
     * @tparam T Data type of the value to be constrained (double, int, etc.)
     * @param x Value to be contrained
     * @param range Range of values in the form {min, max}
     * @return Constrained value 
     */
    template<class T> 
    inline static T constrainOutput(T x, T (&range)[2]);


    /**
     * @brief Takes in a number and returns its absolute value
     * 
     * @tparam T Data type for the value
     * @param value Number to get the absolute value of
     * @return T Absolute value of the number 
     */
    template <class T> 
    inline static T absVal(T value);
};

#endif // PID_COMMAND