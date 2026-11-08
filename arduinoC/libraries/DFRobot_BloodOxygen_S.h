/*!
 * @file DFRobot_BloodOxygen_S.h
 * @brief This is the header of the library for the sensor that can detect human oxygen saturation and heart rate.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      PengKaixing(kaixing.peng@dfrobot.com)
 * @version  V1.1.0
 * @date  2026-11-05
 * @url https://github.com/DFRobot/DFRobot_BloodOxygen_S
 */
#ifndef __DFRobot_BloodOxygen_S_H__
#define __DFRobot_BloodOxygen_S_H__
#include "Arduino.h"
#include <Wire.h>

// ===== BEGIN Mind+ local modification (I2C only) =====
// 本扩展只支持 I2C 模式,已移除 UART / Modbus RTU 依赖:
//   #include "DFRobot_RTU.h"
//   #include "SoftwareSerial.h" / "HardwareSerial.h"
// 需要 UART 支持时,请从官方仓库取回原始文件:
//   https://github.com/DFRobot/DFRobot_BloodOxygen_S
// ===== END Mind+ local modification =====

// Open this macro to see the program running in detail
// NOTE(Mind+): 扩展场景下关闭调试输出。打开后每次读写寄存器都会 Serial.print,
// 而 Mind+ 生成的程序不会调用 Serial.begin(), 在 micro:bit 上会白白拖慢采集速度。
// #define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...)                 \
  {                              \
    Serial.print("[");           \
    Serial.print(__FUNCTION__);  \
    Serial.print("(): ");        \
    Serial.print(__LINE__);      \
    Serial.print(" ] ");         \
    Serial.println(__VA_ARGS__); \
  } 
#else
#define DBG(...)
#endif


class DFRobot_BloodOxygen_S
{
  public:
    /**
     * @struct sHeartbeatSPO2
     * @brief The struct for storing heart rate and oxygen saturation
     */
    typedef struct
    {
      int SPO2;
      int Heartbeat;
    } sHeartbeatSPO2;

    /**
     * @enum eSignalStatus
     * @brief Measurement status carried by getSignalInfo
     */
    typedef enum
    {
      eStatusStopped = 0,   ///< Measurement is stopped
      eStatusNoFinger = 1,  ///< No finger is detected
      eStatusAcquiring = 2, ///< Collecting data, no result yet
      eStatusValid = 3,     ///< Heart rate and SpO2 are valid
      eStatusPoor = 4       ///< Signal quality is too low
    } eSignalStatus;

    /**
     * @struct sSignalInfo
     * @brief Status and signal quality from hardware above V2.0
     * @n Hardware V2.0 is not supported. The fields are valid only after getSignalInfo returns true.
     * @n perfusion is the perfusion index in units of 0.01%. A value of 100 means 1.00%.
     * @n SQI is the signal quality, 0 to 100.
     * @n ledLevel is the infrared LED drive level.
     */
    typedef struct
    {
      uint8_t status;
      uint8_t sequence;
      uint16_t perfusion;
      uint8_t SQI;
      uint8_t ledLevel;
    } sSignalInfo;

    /**
     * @enum ebautrate
     * @brief Baud rate enum
     */
    typedef enum
    {
      BAUT_RATE_1200 = 0,
      BAUT_RATE_2400 = 1,
      BAUT_RATE_9600 = 3,
      BAUT_RATE_19200 = 5,
      BAUT_RATE_38400 = 6,
      BAUT_RATE_57600 = 7,
      BAUT_RATE_115200 = 8
    } ebautrate;

    DFRobot_BloodOxygen_S(void){};
    ~DFRobot_BloodOxygen_S(void){};

    /**
     * @fn getHeartbeatSPO2
     * @brief Get heart rate and oxygen saturation and store them into the struct sHeartbeatSPO2
     */
    void getHeartbeatSPO2(void);

    /**
     * @fn getTemperature_C
     * @brief Get the sensor board temp
     * @return The current onboard temp (unit: ℃)
     */
    float getTemperature_C(void);

    /**
     * @fn setBautrate
     * @brief Set serial baud rate of the sensor
     * @param bautrate baud rate
     * @n  BAUT_RATE_1200 
     * @n  BAUT_RATE_2400 
     * @n  BAUT_RATE_9600 
     * @n  BAUT_RATE_19200 
     * @n  BAUT_RATE_38400 
     * @n  BAUT_RATE_57600 
     * @n  BAUT_RATE_115200 
     */
    void setBautrate(ebautrate bautrate);

    /**
     * @fn getBautrate
     * @brief Get serial port baud rate
     * @return Serial port baud rate
     */
    uint32_t getBautrate(void);

    /**
     * @fn sensorStartCollect
     * @brief Data collecting starts
     */
    void sensorStartCollect(void);

    /**
     * @fn sensorEndCollect
     * @brief Data collecting stops
     */
    void sensorEndCollect(void);

    /**
     * @fn getVersion
     * @brief Read the firmware version register
     * @n This register exists on the earlier products as well as later ones.
     * @n 0x0100 is the earlier firmware. 0x0200 is hardware V2.0.
     * @return Version word. 0 means the register could not be read.
     */
    uint16_t getVersion(void);

    /**
     * @fn getSignalInfo
     * @brief Read measurement status, sequence, PI, signal quality and infrared LED level
     * @n This API can be used on hardware versions above V2.0.
     * @n Hardware V2.0 is not supported, and neither are earlier products.
     * @n Check the return value before using _sSignalInfo. A status of 0 is a real stopped state only when this function returns true.
     * @return true when the connected hardware is above V2.0 and the data has been stored in _sSignalInfo
     * @return false on hardware V2.0, earlier products, or a failed read. _sSignalInfo is cleared.
     */
    bool getSignalInfo(void);
    sHeartbeatSPO2 _sHeartbeatSPO2;
    sSignalInfo _sSignalInfo;
    ebautrate _bautrate;

  protected:
    /**
     * @fn writeReg
     * @brief Write data to the specified register of the sensor
     * @param reg_addr Register address to be written
     * @param data_buf Data to be written to register
     * @param len Length of data to be written
     */
    virtual void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len) = 0;

    /**
     * @fn readReg
     * @brief Get the data with specified length from the specified sensor
     * @param reg_addr Register address to be read
     * @param data_buf The position storing the register data to be read
     * @param len Length of the data to be read
     */
    virtual int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len) = 0;
};

class DFRobot_BloodOxygen_S_I2C : public DFRobot_BloodOxygen_S
{
  public:
    DFRobot_BloodOxygen_S_I2C(TwoWire *pWire = &Wire, uint8_t addr = 0x57);
    ~DFRobot_BloodOxygen_S_I2C(){};
    bool begin();
  protected:
    void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
    int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
  private:
    TwoWire *_pWire;
    uint8_t _I2C_addr;
};

// ===== BEGIN Mind+ local modification (I2C only) =====
// 已删除 UART 类: DFRobot_BloodOxygen_S_SoftWareUart / _HardWareUart
// 它们依赖 DFRobot_RTU(_modbus 成员)与 SoftwareSerial/HardwareSerial
// ===== END Mind+ local modification =====
#endif
