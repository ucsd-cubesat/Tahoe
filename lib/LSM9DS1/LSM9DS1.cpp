#include "LSM9DS1.h"
#include "LSM9DS1_registers.h"

uint8_t LSM9DS1::rebootXLG() {
    m_accel_gyro.write(REG_CTRL_REG8, 0x05);
    delay(10);
    uint8_t whoami = m_accel_gyro.read(REG_WHO_AM_I);
    return whoami == 0x68 ? 0 : whoami;
}

void LSM9DS1::enterXLOnlyMode(ODR_XL odr, FS_XL fs) {
    ctrl_reg1_g_t config_g;
    config_g.v = m_accel_gyro.read(REG_CTRL_REG1_G);
    config_g.odr = ODR_G_OFF;
    m_accel_gyro.write(REG_CTRL_REG1_G, config_g.v);

    ctrl_reg6_xl_t config_xl;
    config_xl.odr = odr;
    config_xl.fs = this->fs_xl = fs;
    m_accel_gyro.write(REG_CTRL_REG6_XL, config_xl.v);
}

void LSM9DS1::enterXLGMode(ODR_G odr, FS_G fs_g, FS_XL fs_xl) {
    ctrl_reg6_xl_t config_xl;
    config_xl.v = m_accel_gyro.read(REG_CTRL_REG6_XL);
    config_xl.odr = ODR_XL_OFF;
    config_xl.fs = this->fs_xl = fs_xl;
    m_accel_gyro.write(REG_CTRL_REG6_XL, config_xl.v);

    ctrl_reg1_g_t config_g;
    config_g.odr = odr;
    config_g.fs = this->fs_g = fs_g;
    m_accel_gyro.write(REG_CTRL_REG1_G, config_g.v);
}



uint8_t LSM9DS1::rebootMag() {
    ctrl_reg2_m_t config;
    config.soft_rst = 1;
    m_magneto.write(REG_CTRL_REG2_M, config.v);
    delay(10);
    uint8_t whoami = m_magneto.read(REG_WHO_AM_I_M);
    return whoami == 0x3D ? 0 : whoami;
}

void LSM9DS1::configureMag(OPER_M mode, ODR_M odr, POWER_M power, FS_M fs) {
    ctrl_reg1_m_t config1;
    config1.om = power;
    config1.odr = odr;
    m_magneto.write(REG_CTRL_REG1_M, config1.v);

    ctrl_reg2_m_t config2;
    config2.fs = fs;
    m_magneto.write(REG_CTRL_REG2_M, config2.v);

    ctrl_reg3_m_t config3;
    config3.md = mode;
    m_magneto.write(REG_CTRL_REG3_M, config3.v);
    
    ctrl_reg4_m_t config4;
    config4.omz = power;
    m_magneto.write(REG_CTRL_REG4_M, config4.v);
}



void LSM9DS1::readAccelerometer(int16_t &x, int16_t &y, int16_t &z) {
    uint8_t buff[6];
    m_accel_gyro.read(START_OUT_XL, buff, 6);
    x = (buff[1] << 8) | buff[0];
    y = (buff[3] << 8) | buff[2];
    z = (buff[5] << 8) | buff[4];
}

void LSM9DS1::readAccelerometer(float &x, float &y, float &z) {
    int16_t x_raw, y_raw, z_raw;
    readAccelerometer(x_raw, y_raw, z_raw);
    float resolution = this->fs_xl == FS_XL_2 ? 0.061f :
                       this->fs_xl == FS_XL_4 ? 0.122f :
                       this->fs_xl == FS_XL_8 ? 0.244f : 0.732;
    x = x_raw * resolution * 9.80665f / 1000;
    y = y_raw * resolution * 9.80665f / 1000;
    z = z_raw * resolution * 9.80665f / 1000;
}



void LSM9DS1::readGyroscope(int16_t &x, int16_t &y, int16_t &z) {
    uint8_t buff[6];
    m_accel_gyro.read(START_OUT_G, buff, 6);
    x = (buff[1] << 8) | buff[0];
    y = (buff[3] << 8) | buff[2];
    z = (buff[5] << 8) | buff[4];
}

void LSM9DS1::readGyroscope(float &x, float &y, float &z) {
    int16_t x_raw, y_raw, z_raw;
    readGyroscope(x_raw, y_raw, z_raw);
    float resolution = this->fs_g == FS_G_245 ? 8.75f :
                       this->fs_g == FS_G_500 ? 17.5f : 70.0f;
    x = x_raw * resolution / 1000;
    y = y_raw * resolution / 1000;
    z = z_raw * resolution / 1000;
}



void LSM9DS1::readMagnetosensor(int16_t &x, int16_t &y, int16_t &z) {
    uint8_t buff[6];
    m_magneto.readAuto(START_OUT_M, buff, 6);
    x = (buff[1] << 8) | buff[0];
    y = (buff[3] << 8) | buff[2];
    z = (buff[5] << 8) | buff[4];
}

void LSM9DS1::readMagnetosensor(float &x, float &y, float &z) {
    int16_t x_raw, y_raw, z_raw;
    readMagnetosensor(x_raw, y_raw, z_raw);
    float resolution = this->fs_m == FS_M_4  ? 0.14f :
                       this->fs_m == FS_M_8  ? 0.29f :
                       this->fs_m == FS_M_12 ? 0.43f : 0.58f;
    x = x_raw * resolution / 1000;
    y = y_raw * resolution / 1000;
    z = z_raw * resolution / 1000;
}

float LSM9DS1::convertRaw(int16_t raw, FS_XL scale) {
    float resolution = scale == FS_XL_2 ? 0.061f :
                       scale == FS_XL_4 ? 0.122f :
                       scale == FS_XL_8 ? 0.244f : 0.732;
    return raw * resolution * 9.80665f / 1000;
}

float LSM9DS1::convertRaw(int16_t raw, FS_G scale) {
    float resolution = scale == FS_G_245 ? 8.75f :
                       scale == FS_G_500 ? 17.5f : 70.0f;
    return raw * resolution / 1000;
}

float LSM9DS1::convertRaw(int16_t raw, FS_M scale) {
    float resolution = scale == FS_M_4  ? 0.14f :
                       scale == FS_M_8  ? 0.29f :
                       scale == FS_M_12 ? 0.43f : 0.58f;
    return raw * resolution / 1000;
}