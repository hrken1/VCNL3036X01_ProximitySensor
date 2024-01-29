/**************************************************************************
*! 
  @file VCNL36687.h

  @section intro Introduction
  This is an Arduino library for the Vishay VCNL36687 VCSEL based proximity sensor

  @section author Author
  Hriday Keni
  info@amken.us

 **
 * @brief I2C Communication  for VCNL3036X01
 *
 * Write Command to VCNL3036X01:
 *  Start (S) - Slave Address (7-bit) - Write Bit (Wr) - Acknowledge (A) - 
 *  Command Code (8-bit) - A - Data Byte Low (8-bit) - A - Data Byte High (8-bit) - A - Stop (P)
 *
 * Read Data from VCNL3036X01:
 *  Start (S) - Slave Address (7-bit) - Write Bit (Wr) - A - 
 *  Command Code (8-bit) - A - Repeated Start (Sr) - Slave Address (7-bit) - 
 *  Read Bit (Rd) - A - Data Byte Low (8-bit) - A - Data Byte High (8-bit) - Not Acknowledge (N) - Stop (P)
 *
 * Note: 'A' indicates the sensor has acknowledged the received byte.
 *       'N' indicates no acknowledgment from the master to signal the end of the read operation.
 * 

**
 * 
 *  I2C Communication Protocol for VCNL3036X01
 *
 * The VCNL3036X01 sensor uses a 7-bit I2C addressing scheme with a single slave address of 0x41 (HEX). 
 * It employs a simple command register structure for easy programming and data retrieval. 
 * The sensor supports both write and read word operations for accessing its registers, 
 * particularly useful for 12-bit / 16-bit PS data. 
 * The I2C communication involves sending a start condition, followed by the slave address 
 * and the command code. Acknowledgement bits are used for reliable communication. 
 * An interrupt can be cleared by reading the INT_Flag register. 
 * It is essential to adhere to the read and write word protocols for command codes.
 *
**************************************************************************/

#ifndef VCNL3036_H
#define VCNL3036_H

#include "Arduino.h" // Arduino data type definitions
#include <Wire.h> // Standard I2C "Wire" library

/********************************************
** Declare all constants used in the class **
********************************************/

/**
 * @brief proximity sensor registers list.
 * @details Specified registers list of VCNL3036 proximity sensor.
 */
#define VCNL3036_REG_PS_CONF12               0x03
#define VCNL3036_REG_PS_CONF3_MS             0x04
#define VCNL3036_REG_PS_CANC                 0x05
#define VCNL3036_REG_PS_THDL                 0x06
#define VCNL3036_REG_PS_THDH                 0x07
#define VCNL3036_REG_PS1_DATA                0x08
#define VCNL3036_REG_PS2_DATA                0x09
#define VCNL3036_REG_PS3_DATA                0x0A
#define VCNL3036_REG_INT_FLAG                0x0D
#define VCNL3036_REG_ID                      0x0E

/**
 * @brief proximity sensorPS_CONF12 register settings.
 * @details Specified settings for PS_CONF12 register of VCNL3036 proximity sensor.
 */
#define VCNL3036_PS_CONF2_MPX_INT_EN_DIS     0x0000
#define VCNL3036_PS_CONF2_MPX_INT_EN_EN      0x8000
#define VCNL3036_PS_CONF2_MPX_INT_EN_MASK    0x8000
#define VCNL3036_PS_CONF2_MPX_MODE_DIS       0x0000
#define VCNL3036_PS_CONF2_MPX_MODE_EN        0x4000
#define VCNL3036_PS_CONF2_MPX_MODE_MASK      0x4000
#define VCNL3036_PS_CONF2_PS_GAIN_TWO_STEP   0x0000
#define VCNL3036_PS_CONF2_PS_GAIN_SINGLE_X8  0x2000
#define VCNL3036_PS_CONF2_PS_GAIN_SINGLE_X1  0x3000
#define VCNL3036_PS_CONF2_PS_GAIN_MASK       0x3000
#define VCNL3036_PS_CONF2_PS_HD_12BITS       0x0000
#define VCNL3036_PS_CONF2_PS_HD_16BITS       0x0800
#define VCNL3036_PS_CONF2_PS_HD_MASK         0x0800
#define VCNL3036_PS_CONF2_PS_NS_MASK         0x0400
#define VCNL3036_PS_CONF2_PS_INT_DIS         0x0000
#define VCNL3036_PS_CONF2_PS_INT_CLOSE       0x0100
#define VCNL3036_PS_CONF2_PS_INT_AWAY        0x0200
#define VCNL3036_PS_CONF2_PS_INT_CLOSE_AWAY  0x0300
#define VCNL3036_PS_CONF2_PS_INT_MASK        0x0300
#define VCNL3036_PS_CONF2_MASK               0xFF00
#define VCNL3036_PS_CONF1_PS_DUTY_1_OF_40    0x0000
#define VCNL3036_PS_CONF1_PS_DUTY_1_OF_80    0x0040
#define VCNL3036_PS_CONF1_PS_DUTY_1_OF_160   0x0080
#define VCNL3036_PS_CONF1_PS_DUTY_1_OF_320   0x00C0
#define VCNL3036_PS_CONF1_PS_DUTY_MASK       0x00C0
#define VCNL3036_PS_CONF1_PS_PERS_1          0x0000
#define VCNL3036_PS_CONF1_PS_PERS_2          0x0010
#define VCNL3036_PS_CONF1_PS_PERS_3          0x0020
#define VCNL3036_PS_CONF1_PS_PERS_4          0x0030
#define VCNL3036_PS_CONF1_PS_PERS_MASK       0x0030
#define VCNL3036_PS_CONF1_PS_IT_1T           0x0000
#define VCNL3036_PS_CONF1_PS_IT_1p5T         0x0002
#define VCNL3036_PS_CONF1_PS_IT_2T           0x0004
#define VCNL3036_PS_CONF1_PS_IT_2p5T         0x0006
#define VCNL3036_PS_CONF1_PS_IT_3T           0x0008
#define VCNL3036_PS_CONF1_PS_IT_3p5T         0x000A
#define VCNL3036_PS_CONF1_PS_IT_4T           0x000C
#define VCNL3036_PS_CONF1_PS_IT_8T           0x000E
#define VCNL3036_PS_CONF1_PS_IT_MASK         0x000E
#define VCNL3036_PS_CONF1_PS_SD_POWER_ON     0x0000
#define VCNL3036_PS_CONF1_PS_SD_SHUT_DOWN    0x0001
#define VCNL3036_PS_CONF1_PS_SD_MASK         0x0001
#define VCNL3036_PS_CONF1_MASK               0x00FF

/**
 * @brief proximity sensor PS_CONF3_MS register settings.
 * @details Specified settings for PS_CONF3_MS register of VCNL3036 proximity sensor.
 */
#define VCNL3036_PS_MS_PS_SC_CUR_1X          0x0000
#define VCNL3036_PS_MS_PS_SC_CUR_2X          0x2000
#define VCNL3036_PS_MS_PS_SC_CUR_4X          0x4000
#define VCNL3036_PS_MS_PS_SC_CUR_8X          0x6000
#define VCNL3036_PS_MS_PS_SC_CUR_MASK        0x6000
#define VCNL3036_PS_MS_PS_SP_1X_CAP          0x0000
#define VCNL3036_PS_MS_PS_SP_1p5X_CAP        0x1000
#define VCNL3036_PS_MS_PS_SP_MASK            0x1000
#define VCNL3036_PS_MS_PS_SPO_00H            0x0000
#define VCNL3036_PS_MS_PS_SPO_FFH            0x0800
#define VCNL3036_PS_MS_PS_SPO_MASK           0x0800
#define VCNL3036_PS_MS_LED_I_50mA            0x0000
#define VCNL3036_PS_MS_LED_I_75mA            0x0100
#define VCNL3036_PS_MS_LED_I_100mA           0x0200
#define VCNL3036_PS_MS_LED_I_120mA           0x0300
#define VCNL3036_PS_MS_LED_I_140mA           0x0400
#define VCNL3036_PS_MS_LED_I_160mA           0x0500
#define VCNL3036_PS_MS_LED_I_180mA           0x0600
#define VCNL3036_PS_MS_LED_I_200mA           0x0700
#define VCNL3036_PS_MS_LED_I_MASK            0x0700
#define VCNL3036_PS_MS_MASK                  0xFF00
#define VCNL3036_PS_CONF3_LED_I_LOW_DIS      0x0000
#define VCNL3036_PS_CONF3_LED_I_LOW_EN       0x0080
#define VCNL3036_PS_CONF3_LED_I_LOW_MASK     0x0080
#define VCNL3036_PS_CONF3_IRED_SELECT_IRED1  0x0000
#define VCNL3036_PS_CONF3_IRED_SELECT_IRED2  0x0020
#define VCNL3036_PS_CONF3_IRED_SELECT_IRED3  0x0040
#define VCNL3036_PS_CONF3_IRED_SELECT_MASK   0x0060
#define VCNL3036_PS_CONF3_PS_SMART_PERS_DIS  0x0000
#define VCNL3036_PS_CONF3_PS_SMART_PERS_EN   0x0010
#define VCNL3036_PS_CONF3_PS_SMART_PERS_MASK 0x0010
#define VCNL3036_PS_CONF3_PS_AF_DIS          0x0000
#define VCNL3036_PS_CONF3_PS_AF_EN           0x0008
#define VCNL3036_PS_CONF3_PS_AF_MASK         0x0008
#define VCNL3036_PS_CONF3_PS_TRIG_NO_TRIG    0x0000
#define VCNL3036_PS_CONF3_PS_TRIG_ONE_TIME   0x0004
#define VCNL3036_PS_CONF3_PS_TRIG_MASK       0x0004
#define VCNL3036_PS_CONF3_PS_MS_NORMAL       0x0000
#define VCNL3036_PS_CONF3_PS_MS_DET_LOGIC    0x0002
#define VCNL3036_PS_CONF3_PS_MS_MASK         0x0002
#define VCNL3036_PS_CONF3_PS_SC_EN_TURN_OFF  0x0000
#define VCNL3036_PS_CONF3_PS_SC_EN_TURN_ON   0x0001
#define VCNL3036_PS_CONF3_PS_SC_EN_MASK      0x0001
#define VCNL3036_PS_CONF3_MASK               0x00FF

/**
 * @brief proximity sensor INT_FLAG register settings.
 * @details Specified settings for INT_FLAG register of VCNL3036 proximity sensor.
 */
#define VCNL3036_INT_FLAG_MPX_DATA_READY     0x8000
#define VCNL3036_INT_FLAG_PS_SP              0x4000
#define VCNL3036_INT_FLAG_PS_IF_CLOSE        0x0200
#define VCNL3036_INT_FLAG_PS_IF_AWAY         0x0100
#define VCNL3036_INT_FLAG_MASK               0xFF00

/**
 * @brief proximity sensor ID setting.
 * @details Specified ID of VCNL3036 proximity sensor.
 */
#define VCNL3036_DEVICE_ID                   0x80
#define VCNL3036_DEVICE_ID_MASK              0xFF

/**
 * @brief proximity sensor data ready timeout.
 * @details Specified data ready timeout of VCNL3036 proximity sensor.
 */
#define VCNL3036_DATA_READY_TIMEOUT_MS       5000

/**
 * @brief proximity sensor device address setting.
 * @details Specified setting for device slave address selection of
 * VCNL3036 proximity sensor.
 */
#define VCNL3036_DEVICE_ADDRESS              0x41

/**
 * @brief proximity sensorClick return value data.
 * @details Predefined enum values for driver return values.
 */
typedef enum
{
    VCNL3036_OK = 0,
    VCNL3036_ERROR = -1

} VCNL3036_RETURN_VALUE_T;

/**************************************************************************/
/*!
    @brief  The VCNL3036 class
*/
/**************************************************************************/
class VCNL3036 {
public:
    VCNL3036(uint8_t i2caddr = VCNL3036_DEVICE_ADDRESS);
    boolean begin(TwoWire &wirePort = Wire);
    boolean checkCommunication();
    boolean defaultConfiguration(); 
    boolean waitForDataReady();
    uint8_t getIntPin();
    boolean setInterruptThresholds(uint16_t low, uint16_t high);
    boolean setSmartPersistence(boolean smart_persistence_enabled);
    boolean enableInterrupts();
    boolean disableInterrupts();
    uint16_t readProximity();
    void setInterruptPin(uint8_t intPin);

private:
    uint8_t _i2caddr;
    uint8_t _intPin; // Added interrupt pin variable
    TwoWire *_wire;
    
    boolean writeCommand(uint8_t reg, uint16_t value);
    boolean readCommand(uint8_t reg, uint16_t &value); // Changed return type to boolean
};


#endif