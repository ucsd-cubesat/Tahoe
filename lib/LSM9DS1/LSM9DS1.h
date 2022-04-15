#ifndef LSM9DS1_H
#define LSM9DS1_H

#include "SPIDevice.h"

/**
 * @brief Determines the output data rate of the gyroscope
 */
enum ODR_G: uint8_t {
    // Power Off
    ODR_G_OFF  = 0b000,

    // Output at 14.9 Hz
    ODR_G_14_9 = 0b001,

    // Output at 59.9 Hz
    ODR_G_59_9 = 0b010,

    // Output at 119 Hz
    ODR_G_119  = 0b011,

    // Output at 238 Hz
    ODR_G_238  = 0b100,

    // Output at 476 Hz
    ODR_G_476  = 0b101,

    // Output at 952 Hz
    ODR_G_952  = 0b110
};

/**
 * @brief Determines the scale and precision of the gyroscope
 */
enum FS_G: uint8_t {
    // Scale of 245 deg/s at a precision of 8.75 mdeg/s/LSB
    FS_G_245  = 0b00,

    // Scale of 500 deg/s at a precision of 17.5 mdeg/s/LSB
    FS_G_500  = 0b01,

    // Scale of 2000 deg/s at a precision of 70 mdeg/s/LSB
    FS_G_2000 = 0b11
};

/**
 * @brief Determines the output data rate of the accelerometer
 */
enum ODR_XL: uint8_t {
    // Power Off
    ODR_XL_OFF = 0b000,

    // Output at 10 Hz
    ODR_XL_10  = 0b001,

    // Output at 50 Hz
    ODR_XL_50  = 0b010,

    // Output at 119 Hz
    ODR_XL_119 = 0b011,

    // Output at 238 Hz
    ODR_XL_238 = 0b100,

    // Output at 476 Hz
    ODR_XL_476 = 0b101,

    // Output at 952 Hz
    ODR_XL_952 = 0b110
};

/**
 * @brief Determines the scale and precision of the accelerometer.
 *        Note that g is the standard acceleration due to Earth's gravity
 */
enum FS_XL: uint8_t {
    // Scale of 2g at a precision of 0.061 mg/LSB
    FS_XL_2  = 0b00,

    // Scale of 4g at a precision of 0.122 mg/LSB
    FS_XL_4  = 0b10,

    // Scale of 8g at a precision of 0.244 mg/LSB
    FS_XL_8  = 0b11,

    // Scale of 16g at a precision of 0.732 mg/LSB
    FS_XL_16 = 0b01
};

/**
 * @brief Determines the output data rate of the magnetosensor
 */
enum ODR_M: uint8_t {
    // Output at 0.625 Hz
    ODR_M_0_625 = 0b0000,

    // Output at 1.25 Hz
    ODR_M_1_25  = 0b0010,

    // Output at 2.5 Hz
    ODR_M_2_5   = 0b0100,

    // Output at 5 Hz
    ODR_M_5     = 0b0110,

    // Output at 10 Hz
    ODR_M_10    = 0b1000,

    // Output at 20 Hz
    ODR_M_20    = 0b1010,

    // Output at 40 Hz
    ODR_M_40    = 0b1100,

    // Output at 80 Hz
    ODR_M_80    = 0b1110,

    // Output at 155 Hz
    // Max ODR for ultra-high power mode
    ODR_M_155   = 0b0001,

    // Output at 300 Hz
    // Max ODR for high power mode
    ODR_M_300   = 0b0011,

    // Output at 155 Hz
    // Max ODR for medium power mode
    ODR_M_560   = 0b0101,

    // Output at 155 Hz
    ODR_M_1000  = 0b0111
};

/**
 * @brief Determines the scale and precision of the magnetosensor
 */
enum FS_M: uint8_t {
    // Scale of 4 guass at a precision of 0.14 mgauss/LSB
    FS_M_4  = 0b00,

    // Scale of 8 guass at a precision of 0.29 mgauss/LSB
    FS_M_8  = 0b01,

    // Scale of 12 guass at a precision of 0.43 mgauss/LSB
    FS_M_12 = 0b10,

    // Scale of 16 guass at a precision of 0.58 mgauss/LSB
    FS_M_16 = 0b11
};

/**
 * @brief Determines the power mode for the magnetosensor
 */
enum POWER_M: uint8_t {
    // Low power mode
    POWER_M_LOW = 0b00,
    
    // Medium power mode
    POWER_M_MED = 0b01,

    // High power mode
    POWER_M_HIGH   = 0b10,

    // Ultra-high power mode
    POWER_M_ULTRA = 0b11
};

/**
 * @brief Determines the operation mode for the magnetosensor
 */
enum OPER_M: uint8_t {
    // Power Off
    OPER_M_OFF    = 0b11,
    
    // Single Mode
    OPER_M_SINGLE = 0b01,

    // Continuous Mode
    OPER_M_CONT   = 0b00
};

/**
 * @brief Determines the accelerometer interrupt trigger event
 */
enum INT_XL_CONFIG: uint8_t {

  // Z-axis high event
  ZHIE_XL = 0b00100000,

  // Z-axis low event
  ZLIE_XL = 0b00010000,
  
  // Y-axis high event
  YHIE_XL = 0b00001000,

  // Y-axis low event
  YLIE_XL = 0b00000100,

  // X-axis high event
  XHIE_XL = 0b00000010,

  // X-axis low event
  XLIE_XL = 0b00000001,

  // All high Events
  XYZHIE_XL = ZHIE_XL | YHIE_XL | XHIE_XL, 

  // All low events
  XYZLIE_XL = ZLIE_XL | YLIE_XL | XLIE_XL,

  // All events - 6Direction enabled
  XYZ_ALL_IE_XL = 0b01000000
  
};


/**
 * @brief Determines the gyroscope interrupt trigger event
 */
enum INT_G_CONFIG: uint8_t {

  // Z-axis (YAW) high event
  ZHIE_G = 0b00100000,

  // Z-axis (YAW) low event
  ZLIE_G = 0b00010000,
  
  // Y-axis (ROLL) high event
  YHIE_G = 0b00001000,

  // Y-axis (ROLL) low event
  YLIE_G = 0b00000100,

  // X-axis (PITCH) high event
  XHIE_G = 0b00000010,

  // X-axis (PITCH) low event
  XLIE_G = 0b00000001,

  // All high Events
  XYZHIE_G = ZHIE_G | YHIE_G | XHIE_G,

  // All low events
  XYZLIE_G = ZLIE_G | YLIE_G | XLIE_G,

  // All events
  XYZ_ALL_IE_G = 0b00111111

};


/**
 * @brief Determines magnetometer interrupt trigger event (on INTM)
 */
enum INT_MAG_CONFIG: uint8_t {

  // X-axis
  XIEN = 0b10000000,

  // Y-axis
  YIEN = 0b01000000,

  // X-axis
  ZIEN = 0b00100000,

  // All axis
  XYZIEN = XIEN | YIEN | ZIEN

};


/**
 * @brief Determines axis to modify
 */
enum AXIS { AXIS_X, AXIS_Y, AXIS_Z };


// Only Int1 responds to the interrupt generator; both have data-ready
// interrupt functionality
enum INT_PIN { INT1, INT2 };


/**
 * @brief Interrupt pin trigger conditions; includes conditions for both pins!
 */
enum INT_PIN_CONFIG: uint8_t {

  // *** Interrupt pin 1 ***

  // Gyroscope interrupt
  INT1_IG_G = (1 << 7),

  // Accelerometer interrupt
  INT1_IG_XL = (1 << 6),

  //FSS5 interrupt
  INT1_FSS5 = (1 << 5),

  //Overrun interrupt
  INT1_OVR = (1 << 4),

  // FIFO threshold interrupt
  INT1_FTH = (1 << 3),

  // Boot status available
  INT1_Boot = (1 << 2),

  // Gyroscope Data ready
  INT1_DRDY_G = (1 << 1),

  // Accelerometer Data ready
  INT1_DRDY_XL = 1,


  // *** Interrupt pin 2 ***

  // Inactivity interrupt
  INT2_INACT = (1 << 7),

  //FSS5 interrupt
  INT2_FSS5 = (1 << 5),

  //Overrun interrupt
  INT2_OVR = (1 << 4),

  // FIFO threshold interrupt
  INT2_FTH = (1 << 3),

  // Boot status available
  INT2_DRDY_TEMP = (1 << 2),

  // Gyroscope Data ready
  INT2_DRDY_G = (1 << 1),

  // Accelerometer Data ready
  INT2_DRDY_XL = 1
};



class LSM9DS1 {
public:

    /**
     * @brief Constructs a LSM9DS1 device driver
     * @param clock      The clock frequency for the SPI bus, in Hz; 10 MHz max
     * @param xlg_ss_pin The CS/SS pin of the dual accelerometer+gyroscope
     *                   subchip
     * @param mag_ss_pin The CS/SS pin of the magnetosensor subchip
     */
    LSM9DS1(uint32_t clock, uint16_t xlg_ss_pin, uint16_t mag_ss_pin) :
        m_accel_gyro(SPISettings(clock, MSBFIRST, SPI_MODE0), xlg_ss_pin),
        m_magneto(SPISettings(clock, MSBFIRST, SPI_MODE0), mag_ss_pin) {
        pinMode(xlg_ss_pin, OUTPUT);
        digitalWrite(xlg_ss_pin, HIGH);
        pinMode(mag_ss_pin, OUTPUT);
        digitalWrite(mag_ss_pin, HIGH);
    }

    /**
     * @brief Returns whether the accelerometer / gyroscope subchip is online
     */
    bool XLGisAvailable();

    /**
     * @brief Reboots the dual accel+gyro subchip
     * @return 0 if successful, otherwise the result of reading the WHO_AM_I
     *         register
     */
    uint8_t rebootXLG();

    /**
     * @brief Enters acceleromter-only mode on the dual accel+gyro subchip
     * @param odr The output data rate
     * @param fs  The full-selection mode, which determines resolution
     */
    void enterXLOnlyMode(ODR_XL odr, FS_XL fs);

    /**
     * @brief Enters full accel+gyro mode on the dual subchip. The same
     *        output-data-rate (ODR) is applied for both measurements.
     * @param odr   The output data rate
     * @param fs_g  The full-selection mode for the gyroscope
     * @param fs_xl The full-selection mode for the accelerometer
     */
    void enterXLGMode(ODR_G odr, FS_G fs_g, FS_XL fs_xl);
    


    /**
     * @brief Reboots the magnetosensor subchip
     * @return 0 if successful, otherwise the result of reading the WHO_AM_I_M
     *         register
     */
    uint8_t rebootMag();

    /**
     * @brief Enters continuous-conversion mode on the magnetosensor subchip
     * @param mode  The operation mode; auto continuously samples, while single
     *              samples only on request
     * @param odr   The output data rate
     * @param power The power mode, which limits the ODR
     * @param fs    The full-selection mode, which determines resolution
     */
    void configureMag(OPER_M mode, ODR_M odr, POWER_M power, FS_M fs);
    


    /**
     * @brief Reads the linear acceleration values of the x, y, and z axes in
     *        units of mg/LSB; g is the acceleration due to Earth's gravity
     * @param x The variable in which to store the x acceleration
     * @param y The variable in which to store the y acceleration
     * @param z The variable in which to store the z acceleration
     */

    void readAccelerometer(int16_t &x, int16_t &y, int16_t &z);
    /**
     * @brief Reads the linear acceleration values of the x, y, and z axes in
     *        units of m/s^2
     * @param x The variable in which to store the x acceleration
     * @param y The variable in which to store the y acceleration
     * @param z The variable in which to store the z acceleration
     */
    void readAccelerometer(float &x, float &y, float &z);



    /**
     * @brief Reads the rotational velocity about the x, y, and z axes in units
     *        of mdeg/s/LSB
     * @param x The variable in which to store the x rotation (pitch)
     * @param y The variable in which to store the y rotation (roll)
     * @param z The variable in which to store the z rotation (yaw)
     */
    void readGyroscope(int16_t &x, int16_t &y, int16_t &z);

    /**
     * @brief Reads the rotational velocity about the x, y, and z axes in units
     *        of deg/s
     * @param x The variable in which to store the x rotation (pitch)
     * @param y The variable in which to store the y rotation (roll)
     * @param z The variable in which to store the z rotation (yaw)
     */
    void readGyroscope(float &x, float &y, float &z);

    /**
     * @brief Changes the sleep / wake status of the gyroscope
     * @param wake true (default) wakes the gyroscope, false puts the gyroscope
     *       into a sleep state
     */
    void setGyroActivity(bool wake=true);


    /**
     * @brief Reads the magnetic field of the x, y, and z axes in units of
     *        mgauss/LSB
     * @param x The variable in which to store the x induction
     * @param y The variable in which to store the y induction
     * @param z The variable in which to store the z induction
     */
    void readMagnetosensor(int16_t &x, int16_t &y, int16_t &z);

    /**
     * @brief Reads the magnetic field of the x, y, and z axes in units of gauss
     * @param x The variable in which to store the x induction
     * @param y The variable in which to store the y induction
     * @param z The variable in which to store the z induction
     */
    void readMagnetosensor(float &x, float &y, float &z);


    /**
     * @brief Configures the accel interrupt generator register
     * @param config the interrupt configuration (when to interrupt)
     * @param andInterrupt whether the interrupt is an AND or OR interrupt, applicable
     *        to configurations that interrupt on multiple criteria only
     * @param duration the interrupt duration; the number of samples that the interrupt
     *        will stay triggered (this is dependent on the selected ODR i.e. if duration = ODR
     *        the interrupt will stay triggered for 1 second)
     */
    void configXLInterrupt(INT_XL_CONFIG config, bool andInterrupt, uint8_t duration = 0);

    /**
     * @brief Sets up the accel interrupt threshold for a single axis
     * @param axis the axis to configure
     * @param threshold the interrupt threshold - raw accelerometer value
     */
    void setXLaxisInterruptTHS(AXIS axis, uint8_t threshold);

    /**
     * @brief Sets up the accel interrupt threshold for all axis
     * @param threshold the interrupt threshold - raw accelerometer value
     */
    void setAllXLInterruptTHS(uint8_t threshold);

    /**
     * @brief Configures the gyro interrupt generator register
     * @param intConfig the interrupt configuration (when to interrupt)
     * @param andInterrupt whether the interrupt is an AND or OR interrupt, applicable
     *        to configurations that interrupt on multiple criteria only
     * @param latchingInterrupt whether the interrupt is latching
     * @param duration the interrupt duration; the number of samples that the interrupt
     *        will stay triggered (this is dependent on the selected ODR i.e. if duration = ODR
     *        the interrupt will stay triggered for 1 second)
     */
    void configGInterrupt(INT_G_CONFIG intConfig, bool andInterrupt, bool latching, uint8_t duration = 0);

    /**
     * @brief Configures the gyro interrupt generator register
     * @param axis the axis to configure
     * @param threshold the interrupt threshold - raw gyroscope value
     * @param resetCounter whether to reset the counter after an interrupt (defaults to true)
     */
    void setGInterruptTHS(AXIS axis, uint16_t threshold, bool resetCounter = true);

    /**
     * @brief Sets up the gyroscope interrupt threshold for all axis
     * @param threshold the interrupt threshold - raw accelerometer value
     * @param resetCounter whether to reset the counter after an interrupt (defaults to true)
     */
    void setAllGInterruptTHS(uint16_t threshold, bool resetCounter = true);

    /**
     * @brief Configure an interrupt pin on interrupt criteria and trigger status
     * @param pin the pin to configure - INT1 or INT2
     * @param config the configuration - all wanted interrupts OR'd together
     * @param activeHigh whether all interrupt pins are active high or low (default: high)
     * @param pushPull whether all interrupt pins are push-pull or open-drain (default: push-pull)
     */
    void configXLGIntPin(INT_PIN pin, uint8_t config, bool pushPull=true, bool activeHigh=true);

    /**
     * @brief Configures the magnetometer interrupt generator register
     * @param intConfig the interrupt configuration (when to interrupt)
     * @param latch whether the interrupt is latching
     * @param activeHigh whether the interrupt on the INTM pin will be active high (defaults to high)
     */
    void configMagInterrupt(INT_MAG_CONFIG intConfig, bool latch, bool activeHigh=true);

    /**
     * @brief Sets up the gyroscope interrupt threshold for all axis
     * @param threshold the interrupt threshold - raw magnetometer value
     */
    void configMagTHS(uint16_t threshold);

    /**
     * @brief Unlatches latched magnetometer interrupts
     */
    void unlatchMagInt();

    /**
     * @brief Scales a raw Accelerometer reading to proper units of m/s^2
     * @param raw The raw axis reading
     * @param scale The appropriate scaling factor
     * @return The scaled value
     */
    static float convertRaw(int16_t raw, FS_XL scale);

    /**
     * @brief Scales a raw Gyroscope reading to proper units of deg/s
     * @param raw The raw axis reading
     * @param scale The appropriate scaling factor
     * @return The scaled value
     */
    static float convertRaw(int16_t raw, FS_G scale);

    /**
     * @brief Scales a raw Magnetosensor reading to proper units of guass
     * @param raw The raw axis reading
     * @param scale The appropriate scaling factor
     * @return The scaled value
     */
    static float convertRaw(int16_t raw, FS_M scale);

private:
    SPIDevice m_accel_gyro;
    SPIDevice m_magneto;
    FS_XL fs_xl;
    FS_G fs_g;
    FS_M fs_m;
};

#endif /* LSM9DS1_H */