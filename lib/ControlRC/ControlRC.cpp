#include "ControlRC.hpp"

ControlRC::ControlRC() {
  // Begins iBus and Serial communication
  iBus.begin(Serial);
}


void ControlRC::update() {
  // Joysticks
  left_x = iBus.readChannel(ChannelRC::LEFT_X);
  left_y = iBus.readChannel(ChannelRC::LEFT_Y);
  right_x = iBus.readChannel(ChannelRC::RIGHT_X);
  right_y = iBus.readChannel(ChannelRC::RIGHT_Y);

  // Switches
  swa = iBus.readChannel(ChannelRC::SWA);
  swb = iBus.readChannel(ChannelRC::SWB);
  swc = iBus.readChannel(ChannelRC::SWC);
  swd = iBus.readChannel(ChannelRC::SWD);

  // Variable Knobs
  vra = iBus.readChannel(ChannelRC::VRA);
  vrb = iBus.readChannel(ChannelRC::VRB);
}


void ControlRC::setMapping(const int mapArray[], mapType mappingType) {
  switch (mappingType) {
    case (mapType::JOYSTICK):
      joystickMap[0] = mapArray[0];
      joystickMap[1] = mapArray[1];
      
      break;
    case (mapType::THROTTLE):
      throttleMap[0] = mapArray[0];
      throttleMap[1] = mapArray[1];

      break;
    case (mapType::SWITCH):
      switchMap[0] = mapArray[0];
      switchMap[1] = mapArray[1];

      break;
    case (mapType::C_SWITCH):
      cSwitchMap[0] = mapArray[0];
      cSwitchMap[1] = mapArray[1];
      cSwitchMap[2] = mapArray[2];

      break;
    case (mapType::KNOB):
      knobMap[0] = mapArray[0];
      knobMap[1] = mapArray[1];

      break;
  }
}


int ControlRC::getChannelValue(ChannelRC channel, bool mapChannel) {
  if (mapChannel) {
    switch (channel) {
      case (ChannelRC::LEFT_X):
      case (ChannelRC::RIGHT_X):
      case (ChannelRC::RIGHT_Y):
        return map(*channelVal[channel], minRC, maxRC, joystickMap[0], joystickMap[1]);
      case (ChannelRC::LEFT_Y):
        return map(*channelVal[channel], minRC, maxRC, throttleMap[0], throttleMap[1]);
      case (ChannelRC::SWA):
      case (ChannelRC::SWB):
      case (ChannelRC::SWD):
        return map(*channelVal[channel], minRC, maxRC, switchMap[0], switchMap[1]);
      case (ChannelRC::SWC):
        if (*channelVal[channel] == minRC) {
          return cSwitchMap[0];
        } else if (*channelVal[channel] == 1500) {
          return cSwitchMap[1];
        } else {
          return cSwitchMap[2];
        }
      case (ChannelRC::VRA):
      case (ChannelRC::VRB):
        return map(*channelVal[channel], minRC, maxRC, knobMap[0], knobMap[1]);
    }
  } 
  
  return *channelVal[channel];
}


int ControlRC::getThrottle(bool mapThrottle) {
  if (mapThrottle) {
    return map(throttle, minRC, maxRC, throttleMap[0], throttleMap[1]);
  } else {
    return throttle;
  }
}


int* ControlRC::getValueArray() {
  return *channelVal;
}


int** ControlRC::getValuePointerArray() {
  return channelVal;
}


void ControlRC::printChannels(bool isMapped) {
  for (int i = 0; i < numChannels; i++) {
    Serial.print("Ch[");
    Serial.print(i + 1);
    Serial.print("] - ");
    Serial.print(getChannelValue((ChannelRC)i, isMapped));
    Serial.print(i < numChannels - 1 ? "\t| " : "\n");
  }
}


bool ControlRC::mapSwitches(int val) {
  return val == minRC ? false : true;
}