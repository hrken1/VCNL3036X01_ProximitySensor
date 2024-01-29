/**************************************************************************
*
  @file VCNL36687.cpp

  @section intro Introduction
  This is an Arduino library for the Vishay VCNL36687 VCSEL based proximity sensor

  @section author Author
  Hriday Keni
  info@amken.us
**************************************************************************/

#include "VCNL3036.h"

VCNL3036::VCNL3036(uint8_t i2caddr) : _i2caddr(i2caddr) {
    // Constructor code, if any
}

boolean VCNL3036::begin(TwoWire &wirePort) {
    _wire = &wirePort;
    _wire->begin(); // Start the I2C bus

    // Set up the interrupt pin if you are using one
    pinMode(_intPin, INPUT); // Assuming _intPin is defined and connected to the sensor's interrupt pin

    // Check if the sensor is connected and responds with the correct ID
    if (!checkCommunication()) {
        return false;
    }

    // Initialize the sensor with default settings
    return defaultConfiguration();
}

boolean VCNL3036::checkCommunication() {
    // Read and check the device ID
    uint16_t device_id;
    if (!readCommand(VCNL3036_REG_ID, device_id)) {
        return false;
    }
    return (device_id & VCNL3036_DEVICE_ID_MASK) == VCNL3036_DEVICE_ID;
}

boolean VCNL3036::defaultConfiguration() {
    if (!checkCommunication()) {
        return false;
    }

    boolean error_flag = true;
    // Set configurations in PS_CONF12 register
    error_flag &= writeCommand(VCNL3036_REG_PS_CONF12, VCNL3036_PS_CONF2_MPX_INT_EN_EN | 
                                                           VCNL3036_PS_CONF2_MPX_MODE_EN | 
                                                           VCNL3036_PS_CONF2_PS_GAIN_TWO_STEP | 
                                                           VCNL3036_PS_CONF2_PS_HD_12BITS | 
                                                           VCNL3036_PS_CONF2_PS_INT_DIS | 
                                                           VCNL3036_PS_CONF1_PS_DUTY_1_OF_40 | 
                                                           VCNL3036_PS_CONF1_PS_PERS_4 | 
                                                           VCNL3036_PS_CONF1_PS_IT_8T | 
                                                           VCNL3036_PS_CONF1_PS_SD_POWER_ON);

    // Set configurations in PS_CONF3_MS register
    error_flag &= writeCommand(VCNL3036_REG_PS_CONF3_MS, VCNL3036_PS_MS_PS_SC_CUR_1X | 
                                                           VCNL3036_PS_MS_PS_SP_1X_CAP | 
                                                           VCNL3036_PS_MS_PS_SPO_00H | 
                                                           VCNL3036_PS_MS_LED_I_100mA | 
                                                           VCNL3036_PS_CONF3_LED_I_LOW_DIS | 
                                                           VCNL3036_PS_CONF3_IRED_SELECT_IRED1 | 
                                                           VCNL3036_PS_CONF3_PS_SMART_PERS_DIS | 
                                                           VCNL3036_PS_CONF3_PS_AF_EN | 
                                                           VCNL3036_PS_CONF3_PS_TRIG_NO_TRIG | 
                                                           VCNL3036_PS_CONF3_PS_MS_NORMAL | 
                                                           VCNL3036_PS_CONF3_PS_SC_EN_TURN_OFF);

    return error_flag;
}




boolean VCNL3036::waitForDataReady() {
    unsigned long start = millis();
    while (millis() - start < VCNL3036_DATA_READY_TIMEOUT_MS) {
        if (digitalRead(_intPin) == LOW) { // Assuming _intPin is set up and represents the interrupt pin
            return true;
        }
    }
    return false;
}


// Method to get the state of the interrupt pin
uint8_t VCNL3036::getIntPin() {
    return digitalRead(_intPin); // You need to set up _intPin during initialization
}


// Set interrupt thresholds
boolean VCNL3036::setInterruptThresholds(uint16_t low, uint16_t high) {
    if (!writeCommand(VCNL3036_REG_PS_THDL, low)) {
        return false;
    }
    return writeCommand(VCNL3036_REG_PS_THDH, high);
}


// Method to read a command from the sensor
boolean VCNL3036::readCommand(uint8_t reg, uint16_t &value) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return false;
    }
    _wire->requestFrom((int)_i2caddr, 2);
    if (_wire->available() == 2) {
        value = _wire->read();
        value |= (uint16_t)_wire->read() << 8;
        return true;
    }
    return false;
}

// Method to write a command to the sensor
boolean VCNL3036::writeCommand(uint8_t reg, uint16_t value) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(reg);
    _wire->write((uint8_t)(value & 0xFF));
    _wire->write((uint8_t)(value >> 8));
    return _wire->endTransmission() == 0;
}

// Set Smart Persistence Mode
boolean VCNL3036::setSmartPersistence(boolean smart_persistence_enabled) {
    uint16_t reg_val;
    if (!readCommand(VCNL3036_REG_PS_CONF3_MS, reg_val)) {
        return false;
    }
    // Modify reg_val based on the smart persistence flag
    reg_val &= ~VCNL3036_PS_CONF3_PS_SMART_PERS_MASK; // Clear the PS_SMART_PERS bit
    if (smart_persistence_enabled) {
        reg_val |= VCNL3036_PS_CONF3_PS_SMART_PERS_EN; // Set the PS_SMART_PERS bit
    }
    return writeCommand(VCNL3036_REG_PS_CONF3_MS, reg_val);
}

// Enable interrupts
boolean VCNL3036::enableInterrupts() {
    uint16_t reg_val;
    if (!readCommand(VCNL3036_REG_PS_CONF12, reg_val)) {
        return false;
    }
    reg_val |= VCNL3036_PS_CONF2_MPX_INT_EN_EN;
    return writeCommand(VCNL3036_REG_PS_CONF12, reg_val);
}

// Disable interrupts
boolean VCNL3036::disableInterrupts() {
    uint16_t reg_val;
    if (!readCommand(VCNL3036_REG_PS_CONF12, reg_val)) {
        return false;
    }
    reg_val &= VCNL3036_PS_CONF2_MPX_INT_EN_DIS;
    return writeCommand(VCNL3036_REG_PS_CONF12, reg_val);
}

// Read proximity data
uint16_t VCNL3036::readProximity() {
    uint16_t proximity_data;
    if (readCommand(VCNL3036_REG_PS1_DATA, proximity_data)) {
        return proximity_data;
    }
    return 0; // Return 0 or an error code if reading fails
}

// Method to set the interrupt pin
void VCNL3036::setInterruptPin(uint8_t intPin) {
    _intPin = intPin;
}