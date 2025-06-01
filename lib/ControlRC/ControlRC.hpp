#ifndef TEST_RC
#define TEST_RC

#include <Arduino.h>
#include <IBusBM.h>

/*-----------------------------------------------------------------------------*/
/** @file   ControlRC.hpp
 * @brief   Header for ControlRC class (used to receive RC commands using iBus)
*//*---------------------------------------------------------------------------*/


/**
 * @brief RC channels values
 */
enum ChannelRC {
  RIGHT_X = 0,
  RIGHT_Y,
  LEFT_Y,
  LEFT_X,
  SWA,
  SWD,
  SWB, 
  SWC,
  VRA,
  VRB
};

const int numChannels = 10; // Number of channels on the FlySky FS-i6X controller 

const int minRC = 1000; // Minimum value a channel can be
const int maxRC = 2000; // Maximum value a channel can be


/**
 * @brief Class used for recieving values from a FlySky FS-i6X receiver over iBus
 */
class ControlRC {
  private: 
    int left_x, left_y;
    int right_x, right_y;
    int swa, swb, swc, swd;
    int vra, vrb;

    int& throttle = left_y;         // The left y-axis has the throttle (Doesn't spring back to the middle)
    int* channelVal[numChannels]= { // Default pointers to use
      &right_x,
      &right_y,
      &left_y,
      &left_x,
      &swa,
      &swd,
      &swb,
      &swc,
      &vra, 
      &vrb
    };

    int joystickMap[2];
    int throttleMap[2];
    int switchMap[2];
    int cSwitchMap[3];
    int knobMap[2];

    bool joysticksCentered = false; // Condition for if the joysticks are centered 
    bool switchesOff = false;       // Condition for if the switches are off 
    bool knobsOff = false;          // Condition for if the knobs are off 
    bool isZeroed = false;          // Condition for if the receiver has been zeroed

    bool isReceiving[numChannels];  // Array of conditions for which channels are receiving

    IBusBM iBus; // iBus object for control over communications

  public:
    static const unsigned long iBusBaudrate = 115200;

    /**
     * @brief Type of mapping for setter method
     */
    enum mapType {
      JOYSTICK = 0,
      THROTTLE,
      SWITCH,
      C_SWITCH,
      KNOB
    };


    /**
     * @brief Defines a ControlRC object
     */
    ControlRC();


    /**
     * @brief Updates the values in the channels array
     */
    void update();


    /**
     * @brief Sets the mapping array given the type of mapping to set
     * 
     * @param mapArray Array to use to set the mapping values
     * @param mappingType Type of mapping to set
     */
    void setMapping(const int mapArray[], mapType mappingType);


    /**
     * @brief Get the value of a given channel
     * 
     * @param channel Channel to get the value of
     * @param mapChannel Condition for mapping the value from the channel
     * @return Value recieved, and possibly mapped, from the channel  
     */
    int getChannelValue(ChannelRC channel, bool mapChannel = true);


    /**
     * @brief Gets the value of a channel given a method to map to non-integer types
     * 
     * @tparam T Type to map to 
     * @param channel Channel to get the value from 
     * @param mapFunction Method to use to map the value (Must only take in a single int)
     * @return Mapped value from the channel
     */
    template <class T>
    inline T getChannelValue(ChannelRC channel, T (*mapFunction)(int)) {
      return mapFunction(*channelVal[channel]);
    }


    /**
     * @brief Gets the value of the throttle position
     * 
     * @note Effectively the same as getChannelValue(ChannelRC::LEFT_Y)
     * 
     * @param mapThrottle Condition for if to apply the throttle mapping (Default true)
     * @return Value of the throttle (Channel 3)
     */
    int getThrottle(bool mapThrottle);


    /**
     * @brief Gets all channel values as an array
     * 
     * @return Array of unmapped channel values 
     */
    int* getValueArray();


    /**
     * @brief Gets all the channel value pointers as an array
     * 
     * @return Array of channel value pointers
     */
    int** getValuePointerArray();


    /**
     * @brief Prints the value of each channel
     * 
     * @param isMapped Condition to map the values when printing
     */
    void printChannels(bool isMapped = false);


    /**
     * @brief Maps the value of switches to booleans 
     * 
     * @param val The value of the switch as an integer
     * @return The value of the switch as a boolean 
     */
    static bool mapSwitches(int val);
};

#endif // TEST_RC