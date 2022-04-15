#include "LSM9DS1.h"
#include "LSM9DS1_registers.h"

bool LSM9DS1::XLGisAvailable() {
  uint8_t whoami = m_accel_gyro.read(REG_WHO_AM_I);
  return whoami == 0x68;
}

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


void LSM9DS1::setGyroActivity(bool wake) {
  uint8_t config = m_accel_gyro.read(REG_CTRL_REG9);
  if (wake) {
    // set the sleep bit off
    config |= ~(1 << 6);
  } else {
    config |= (1 << 6);
  }
  m_accel_gyro.write(REG_CTRL_REG1_G, config);
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

void LSM9DS1::configXLInterrupt(INT_XL_CONFIG intConfig, bool andInterrupt, uint8_t duration) {

  uint8_t config = intConfig;
  if (andInterrupt) {
    config |= (1 << 7);
  }
  m_accel_gyro.write(INT_GEN_CFG_XL, config);

  // Configure the duration
  config = 0;

  if (duration != 0) {
    config |= (duration & 0b01111111);
    // configure wait bit
    config |= (1 << 7);
  }

  m_accel_gyro.write(INT_GEN_DUR_XL, config);
}

void LSM9DS1::setXLaxisInterruptTHS(AXIS axis, uint8_t threshold) {
  uint8_t axisRegister = 0;

  switch (axis) {
    case AXIS_X: axisRegister = INT_GEN_THS_X_XL; break;
    case AXIS_Y: axisRegister = INT_GEN_THS_Y_XL; break;
    case AXIS_Z: axisRegister = INT_GEN_THS_Z_XL; break;
  }

  m_accel_gyro.write(axisRegister, threshold);
}

void LSM9DS1::setAllXLInterruptTHS(uint8_t threshold) {
  setXLaxisInterruptTHS(AXIS_X, threshold);
  setXLaxisInterruptTHS(AXIS_Y, threshold);
  setXLaxisInterruptTHS(AXIS_Z, threshold);
}

void LSM9DS1::configGInterrupt(INT_G_CONFIG intConfig, bool andInterrupt, bool latching, uint8_t duration) {

  uint8_t config = intConfig;
  if (andInterrupt) {
    config |= (1 << 7);
  }
  if (latching) {
    config |= (1 << 6);
  }

  m_accel_gyro.write(INT_GEN_CFG_G, config);

  // Configure the duration
  config = 0;

  if (duration != 0) {
    config |= (duration & 0b01111111);
    // configure wait bit
    config |= (1 << 7);
  }

  m_accel_gyro.write(INT_GEN_DUR_G, config);
}

void LSM9DS1::setGInterruptTHS(AXIS axis, uint16_t threshold, bool resetCounter) {

  uint8_t axisRegisterH = 0;
  uint8_t axisRegisterL = 0;

  switch (axis) {
    case AXIS_X: axisRegisterH = INT_GEN_THS_XH_G; axisRegisterL = INT_GEN_THS_XL_G; break;
    case AXIS_Y: axisRegisterH = INT_GEN_THS_YH_G; axisRegisterL = INT_GEN_THS_YL_G; break;
    case AXIS_Z: axisRegisterH = INT_GEN_THS_ZH_G; axisRegisterL = INT_GEN_THS_ZL_G; break;
  }

  uint8_t highByte = (threshold >> 8) & 0b01111111;
  uint8_t lowByte = threshold & 0b11111111;

  if (!resetCounter) {
    // Instead of resetting the counter, opt to decrement the counter
    highByte |= (1 << 7);
  }

  m_accel_gyro.write(axisRegisterH, highByte);
  m_accel_gyro.write(axisRegisterL, lowByte);
}


void LSM9DS1::setAllGInterruptTHS(uint16_t threshold, bool resetCounter) {
  setGInterruptTHS(AXIS_X, threshold, resetCounter);
  setGInterruptTHS(AXIS_Y, threshold, resetCounter);
  setGInterruptTHS(AXIS_Z, threshold, resetCounter);
}


void LSM9DS1::configXLGIntPin(INT_PIN pin, uint8_t config, bool pushPull, bool activeHigh) {

  switch (pin) {
    case INT1: m_accel_gyro.write(INT1_CTRL, config);
    case INT2: m_accel_gyro.write(INT2_CTRL, config);
  }

  // Active high / low and push-pull / open-drain settings are configured in CTRL_REG8
  config = 0;
  if (!activeHigh) {
    config |= (1 << 5);
  }

  if (!pushPull) {
    config |= (1 << 4);
  }

  m_accel_gyro.write(REG_CTRL_REG8, config);
}

void LSM9DS1::configMagInterrupt(INT_MAG_CONFIG intConfig, bool latch, bool activeHigh) {

  uint8_t config = intConfig;

  // Enable interrupt functionality
  config |= (1);

  // For some reason the datsheet spec is opposite;
  // if the latch bit is 1, then it latches, else not
  if (latch) {
    config |= (1 << 1);
  }

  if (activeHigh) {
    config |= (1 << 2);
  }

  m_magneto.write(INT_CFG_M, config);
}

void LSM9DS1::configMagTHS(uint16_t threshold) {

  uint8_t highByte = (threshold >> 8) & 0b01111111;
  uint8_t lowByte = threshold & 0b11111111;

  m_magneto.write(INT_THS_H_M, highByte);
  m_magneto.write(INT_THS_L_M, lowByte);
}


void LSM9DS1::unlatchMagInt() {
  uint8_t settings = m_magneto.read(INT_SRC_M);
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