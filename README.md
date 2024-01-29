# VCNL3036X01_ProximitySensor

  This is an Arduino library for the Vishay VCNL36687 VCSEL based proximity sensor

 Author
  Hriday Keni
  info@amken.us


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
