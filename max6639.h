#ifndef MAX6639_H
#define MAX6639_H


/* Retro-compatibility with arduino 0023 and previous version */
#if ARDUINO >= 100
#include "Arduino.h"
#define I2CWRITE(x) Wire.write(x)
#define I2CREAD() Wire.read()
#else
#include "WProgram.h"
#define I2CWRITE(x) Wire.send(x)
#define I2CREAD() Wire.receive()
#endif

#include <inttypes.h>


#ifndef I2CTIMEOUT
  #define I2CTIMEOUT 100
#endif


/* Addresses to scan */
 static const unsigned short normal_i2c[] = { 0x2c, 0x2e, 0x2f };
 
 /* The MAX6639 registers, valid channel numbers: 0, 1 */
 #define MAX6639_REG_TEMP(ch)                    (0x00 + (ch))
 #define MAX6639_REG_STATUS                      0x02
 #define MAX6639_REG_OUTPUT_MASK                 0x03
 #define MAX6639_REG_GCONFIG                     0x04
 #define MAX6639_REG_TEMP_EXT(ch)                (0x05 + (ch))
 #define MAX6639_REG_ALERT_LIMIT(ch)             (0x08 + (ch))
 #define MAX6639_REG_OT_LIMIT(ch)                (0x0A + (ch))
 #define MAX6639_REG_THERM_LIMIT(ch)             (0x0C + (ch))
 #define MAX6639_REG_FAN_CONFIG1(ch)             (0x10 + (ch) * 4)
 #define MAX6639_REG_FAN_CONFIG2a(ch)            (0x11 + (ch) * 4)
 #define MAX6639_REG_FAN_CONFIG2b(ch)            (0x12 + (ch) * 4)
 #define MAX6639_REG_FAN_CONFIG3(ch)             (0x13 + (ch) * 4)
 #define MAX6639_REG_FAN_CNT(ch)                 (0x20 + (ch))
 #define MAX6639_REG_TARGET_CNT(ch)              (0x22 + (ch))
 #define MAX6639_REG_FAN_PPR(ch)                 (0x24 + (ch))
 #define MAX6639_REG_TARGTDUTY(ch)               (0x26 + (ch))
 #define MAX6639_REG_FAN_START_TEMP(ch)          (0x28 + (ch))
 #define MAX6639_REG_DEVID                       0x3D
 #define MAX6639_REG_MANUID                      0x3E
 #define MAX6639_REG_DEVREV                      0x3F
 
 /* Register bits */
 #define MAX6639_GCONFIG_STANDBY                 0x80
 #define MAX6639_GCONFIG_POR                     0x40
 #define MAX6639_GCONFIG_DISABLE_TIMEOUT         0x20
 #define MAX6639_GCONFIG_CH2_LOCAL               0x10
 #define MAX6639_GCONFIG_PWM_FREQ_HI             0x08
 
 #define MAX6639_FAN_CONFIG1_PWM                 0x80
 
 #define MAX6639_FAN_CONFIG3_THERM_FULL_SPEED    0x40
 
 static const int rpm_ranges[] = { 2000, 4000, 8000, 16000 };
 #define PWM5HZ		4
 #define PWM8_33HZ	5
 #define PWM12_5HZ	6
 #define PWM20HZ	0
 #define PWM25HZ	7
 #define PWM33_33HZ	1	
 #define PWM50HZ	2
 #define PWM100HZ	3
 
 #define LOCAL 1
 #define REMOTE 0
 
 #define ACTIVE_LOW		0
 #define ACTIVE_HIGH	1
 
 #define FAN_FROM_REG(val, rpm_range)    ((val) == 0 || (val) == 255 ? \
                                 0 : (rpm_ranges[rpm_range] * 30) / (val))
 #define TEMP_LIMIT_TO_REG(val)  clamp_val((val) / 1000, 0, 255)
 



class MAX6639 {
public:
  void begin(void);
  void begin(uint8_t _address);

  void writeByte(uint8_t data, uint8_t reg);
  bool readByte(uint8_t *data, uint8_t reg);
  uint8_t readTempC(uint8_t ch);
  uint8_t readTempF(uint8_t ch);
  uint8_t getStatus(void);
  uint8_t getMask(void);
  void setMask(uint8_t Mask);
  uint8_t getConfig(void);
  void setConfig(uint8_t Config);
  bool getDiodeFault(uint8_t ch);
  double getExtTemp(uint8_t ch);
  uint8_t getAlertLimit(uint8_t ch);
  void setAlertLimit(uint8_t Limit, uint8_t ch);
  uint8_t getOTLimit(uint8_t ch);
  void setOTLimit(uint8_t Limit, uint8_t ch);
  uint8_t getTHERMLimit(uint8_t ch);
  void setTHERMLimit(uint8_t Limit, uint8_t ch);
  uint8_t getFanConfig(uint8_t reg);
  void setFanConfig(uint8_t reg, uint8_t Config);
  uint8_t getFanTachCount(uint8_t ch);
  uint8_t getFanTargetTach(uint8_t ch);
  void setFanTargetTach(uint8_t ch, uint8_t Count);
  uint8_t getFanPPR(uint8_t ch);
  void setFanPPR(uint8_t ch, uint8_t ppr);
  uint8_t getFanDuty(uint8_t ch);
  uint8_t getFanDutyPercent(uint8_t ch);
  void setFanDuty(uint8_t ch, uint8_t duty);
  void setFanDutyPercent(uint8_t ch, uint8_t duty);
  uint8_t getFanStartTempC(uint8_t ch);
  void setFanStartTempC(uint8_t ch, uint8_t Temperature);
  uint8_t getDevID(void);
  uint8_t getManuID(void);
  uint8_t getDevRev(void);
  void setFanSpinup(bool state, uint8_t ch);
  void setFanTherm(bool state, uint8_t ch);
  void setFanPulseStretch(bool state, uint8_t ch);
  void setFanPWMFreq(uint8_t freq, uint8_t ch);
  void setRun(bool state);
  bool isRunning(void);
  void setPOR(bool state);
  void setChan2Source(uint8_t source);
  void setPWMPolarity(bool state, uint8_t ch);
  void setPWMMode(bool state, uint8_t ch);
  
protected: 

private:
  uint8_t _address;
  
 };

#endif
