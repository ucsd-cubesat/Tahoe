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