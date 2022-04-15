#ifndef LSM9DS1_registers_H
#define LSM9DS1_registers_H

#include <stdint.h>

//////// Accel + Gyro Registers ////////

#define REG_WHO_AM_I 0x0F
#define REG_CTRL_REG1_G 0x10
#define REG_CTRL_REG6_XL 0x20
#define REG_CTRL_REG8 0x22
#define REG_CTRL_REG9 0x23

#define START_OUT_G  0x18
#define START_OUT_XL 0x28

// * Interrupt registers *

// Accelerometer
#define INT_GEN_CFG_XL 0x06
#define INT_GEN_THS_X_XL 0x07
#define INT_GEN_THS_Y_XL 0x08
#define INT_GEN_THS_Z_XL 0x09
#define INT_GEN_DUR_XL 0x0A
#define INT1_CTRL 0x0C
#define INT2_CTRL 0x0D

// Gyroscope
#define INT_GEN_CFG_G 0x30
#define INT_GEN_THS_XH_G 0x31
#define INT_GEN_THS_XL_G 0x32
#define INT_GEN_THS_YH_G 0x33
#define INT_GEN_THS_YL_G 0x34
#define INT_GEN_THS_ZH_G 0x35
#define INT_GEN_THS_ZL_G 0x36
#define INT_GEN_DUR_G 0x37

typedef union {
    struct {
        uint8_t bw     : 2;
        uint8_t unused : 1;
        uint8_t fs     : 2;
        uint8_t odr    : 3;
    };
    uint8_t v = 0;
} ctrl_reg1_g_t;

typedef union {
    struct {
        uint8_t bw     : 2;
        uint8_t bw_odr : 1;
        uint8_t fs     : 2;
        uint8_t odr    : 3;
    };
    uint8_t v = 0;
} ctrl_reg6_xl_t;

//////// Mag Registers ////////

#define REG_WHO_AM_I_M 0x0F
#define REG_CTRL_REG1_M 0x20
#define REG_CTRL_REG2_M 0x21
#define REG_CTRL_REG3_M 0x22
#define REG_CTRL_REG4_M 0x23

#define START_OUT_M  0x28

// * Interrupt Registers *

#define INT_CFG_M 0x30
#define INT_SRC_M 0x31
#define INT_THS_L_M 0x32
#define INT_THS_H_M 0x33

typedef union {
    struct {
        uint8_t st        : 1;
        // fused from ODR and FAST_ODR
        uint8_t odr       : 4;
        uint8_t om        : 2;
        uint8_t temp_comp : 1;
    };
    uint8_t v = 0x10;
} ctrl_reg1_m_t;

typedef union {
    struct {
        uint8_t unused3  : 2;
        uint8_t soft_rst : 1;
        uint8_t reboot   : 1;
        uint8_t unused2  : 1;
        uint8_t fs       : 2;
        uint8_t unused   : 1;
    };
    uint8_t v = 0;
} ctrl_reg2_m_t;

typedef union {
    struct {
        uint8_t md       : 2;
        uint8_t sim      : 1;
        uint8_t unused2  : 2;
        uint8_t lp       : 1;
        uint8_t unused   : 1;
        uint8_t i2c_dsbl : 1;
    };
    uint8_t v = 0x03;
} ctrl_reg3_m_t;

typedef union {
    struct {
        uint8_t unused2 : 1;
        uint8_t ble     : 1;
        uint8_t omz     : 2;
        uint8_t unused  : 4;
    };
    uint8_t v = 0;
} ctrl_reg4_m_t;

#endif /* LSM9DS1_registers_H */