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
 static const unsigned short normal_i2c[] = { 0x2C, 0x2E, 0x2F };
 
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
 #define MAX6639_REG_FAN_MINTACH(ch)             (0x24 + (ch))
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
 #define RPM_2K		0x00
 #define RPM_4K		0x01
 #define RPM_8K		0x02
 #define RPM_16K	0x03
 
 static const int ppr_values[] = { 1, 2, 3, 4 };
 
 #define FAN_FROM_REG(val, div, rpm_range) ((val) == 0 ? -1 : \
   (val) == 255 ? 0 : (rpm_ranges[rpm_range] * 30) / ((div + 1) * (val)))
   
 #define TEMP_LIMIT_TO_REG(val) SENSORS_LIMIT((val) / 1000, 0, 255)
 
 #define TEMPSTEP_1	0x00
 #define TEMPSTEP_2	0x01
 #define TEMPSTEP_4	0x02
 #define TEMPSTEP_8	0x03
 
 #define PWM5KHZ		4	// High Frequency Range
 #define PWM8_33KHZ		5
 #define PWM12_5KHZ		6
 #define PWM25KHZ		7
 #define PWM20HZ		0	// Low Frequency Range
 #define PWM33_33HZ		1	
 #define PWM50HZ		2
 #define PWM100HZ		3
 
 #define PWM_MODE_MANUAL 	1
 #define PWM_MODE_RPM		0
 
 #define LOCAL 1
 #define REMOTE 0
 
 #define ACTIVE_LOW		0
 #define ACTIVE_HIGH	1
 
 #define TCHAN_OFF	0x00
 #define TCHAN_0	0x08
 #define TCHAN_1 	0x04
 
 #define TACHPPR_1	0x00
 #define TACHPPR_2	0x40
 #define TACHPPR_3	0x80
 #define TACHPPR_4	0xC0
 
 // Default Settings
 
 #define DEFAULT_RPM_RANGE		RPM_2K
 #define DEFAULT_PWM_POLARITY	ACTIVE_HIGH
 #define DEFAULT_FAN_PPR		TACHPPR_2
 #define DEFAULT_PWM_FREQ		PWM50HZ
 #define DEFAULT_MIN_START_TEMP	35
 #define DEFAULT_LIMIT_THERM	50
 #define DEFAULT_LIMIT_ALERT	60
 #define DEFAULT_LIMIT_OVERTEMP	65
 #define DEFAULT_BUS_TIMEOUT	MAX6639_GCONFIG_DISABLE_TIMEOUT
 #define DEFAULT_CH2_SOURCE		MAX6639_GCONFIG_CH2_LOCAL
 #define DEFAULT_FAN_TARGET_DUTY	120
 #define DEFAULT_FAN_ROC		0x60		//default rate of change 0x70 - 0x00 (only bits 6-4 used for default value)
 #define DEFAULT_MIN_TACH		0xF0
 #define DEFAULT_MIN_TACH_ENABLE	1
 #define DEFAULT_STEP_DELAY		0x01		//AKA Rate of Change (3bits) 0x00 - 0x07
 #define DEFAULT_TEMP_STEP_SIZE	0x01		//(2bits) 0x00 - 0x03
 #define DEFAULT_RPM_STEP_A		0x01		//(4bits) 0x00 - 0x0F
 #define DEFAULT_RPM_STEP_B		0x01		//(4bits) 0x00 - 0x0F
 #define DEFAULT_START_STEP_SIZE_B	0x01	//(4bits) 0x00 - 0x0F	

/*
 * Client data (each client gets its own)  **--Not currently used Provided for future use
 */
struct max6639_data {		//**** NOT CURRENTLY USED! ****//

	/* Register values sampled during readfan function */
	float temp;		/* Temperature, in degrees F */
	bool temp_fault;	/* Detected temperature diode failure */
	uint8_t fan;		/* Register value: TACH count for fans >=30 */
	uint16_t rpm;		/* Calculated RPM value derived from fan tach count */
	uint8_t status;		/* Detected channel alarms and fan failures */

	/* Register values only written to device with update function */
	uint8_t fan_mode;	/* Fan Speed Control Mode: 0=Auto RPM, 1=Manual RPM, 2=Set PWM */
	uint8_t pwm;		/* Register value: Duty cycle 0..120 */
	uint8_t target_tach;/* Target Tach for Manual RPM Mode */
	uint8_t temp_therm;	/* THERM Temperature, 0..255 C (->_max) */
	uint8_t temp_alert;	/* ALERT Temperature, 0..255 C (->_crit) */
	uint8_t temp_ot;		/* OT Temperature, 0..255 C (->_emergency) */

	/* Register values initialized only once with init function */
	uint8_t ppr;			/* Pulses per rotation 0..3 for 1..4 ppr */
	uint8_t rpm_range;		/* Index in above rpm_ranges table */
	bool pwm_polarity;		/* Polarity low (0) or high (1, default) */
	bool spin_up_disable;	/* False=Normal start-up fan spin-up, True=disable fan spin-up on start */
	bool therm_full_speed;	/* True=force fan to full speed when /THERM is asserted */
};


class MAX6639 {
public:
//  struct max6639_data fan_info[2];  // Info structure not currently implemented. Provided for future use...

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
  uint8_t getALERTLimit(uint8_t ch);
  void setALERTLimit(uint8_t ch, uint8_t Limit);
  uint8_t getOTLimit(uint8_t ch);
  void setOTLimit(uint8_t ch, uint8_t Limit);
  uint8_t getTHERMLimit(uint8_t ch);
  void setTHERMLimit(uint8_t ch, uint8_t Limit);
  uint8_t getFanConfig(uint8_t reg);
  void setFanConfig(uint8_t reg, uint8_t Config);
  uint8_t getFanTachCount(uint8_t ch);
  void setFanTachCount(uint8_t ch, uint8_t Count);
  uint8_t getFanTargetTach(uint8_t ch);
  void setFanTargetTach(uint8_t ch, uint8_t Count);
  uint8_t getFanPPR(uint8_t ch);
  uint8_t getFanPPRval(uint8_t ch);
  void setFanPPR(uint8_t ch, uint8_t ppr_set);
  void setFanPPRval(uint8_t ch, uint8_t ppr_set);
  uint8_t getFanMinTachCount(uint8_t ch);
  void setFanMinTachCount(uint8_t ch, uint8_t mintach);
  uint8_t getFanDuty(uint8_t ch);
  uint8_t getFanDutyPercent(uint8_t ch);
  void setFanDuty(uint8_t ch, uint8_t duty);
  void setFanDutyPercent(uint8_t ch, uint8_t duty);
  uint8_t getFanStartTempC(uint8_t ch);
  void setFanStartTempC(uint8_t ch, uint8_t Temperature);
  uint8_t getDevID(void);
  uint8_t getManuID(void);
  uint8_t getDevRev(void);
  void setFanSpinup(uint8_t ch, bool state);
  void setFanTherm(uint8_t ch, bool state);
  void setFanPulseStretch(uint8_t ch, bool state);
  void setFanPWMFreq(uint8_t ch, uint8_t freq);
  float getFanPWMFreq(uint8_t ch);
  void setRun(bool state);
  bool isRunning(void);
  void setPOR(bool state);
  void setChan2Source(uint8_t source);
  void setPWMPolarity(uint8_t ch, bool state);
  void setPWMMode(uint8_t ch, bool state);
  void setFanControl(uint8_t ch, uint8_t TChan);
  uint8_t getFanRPMRange(uint8_t ch);
  void setFanRPMRange(uint8_t ch, uint8_t range);
  void setFanMinimumSpeed(uint8_t ch, bool state, uint8_t count);
  void setFanRateOfChange(uint8_t ch, uint8_t val);
  uint8_t getFanRateOfChange(uint8_t ch);
  void setFanRPMStepA(uint8_t ch, uint8_t val);
  uint8_t getFanRPMStepA(uint8_t ch);
  void setFanTempStepA(uint8_t ch, uint8_t val);
  uint8_t getFanTempStepA(uint8_t ch);
  void setFanRPMStepB(uint8_t ch, uint8_t val);
  uint8_t getFanRPMStepB(uint8_t ch);
  void setFanStartStep(uint8_t ch, uint8_t val);
  uint8_t getFanStartStep(uint8_t ch);
  void initDefaults(void);
  uint16_t getFanRPM(uint8_t ch);
  void setFanAutoRPM(uint8_t ch, uint8_t Chan);
  void setFanPWMMode(uint8_t ch, uint8_t Duty);
  void setFanManualRPM(uint8_t ch, uint8_t Tach);
  int getFanSensorCorrection(uint8_t ch);
  void setFanSensorCorrection(uint8_t ch, int Val);
  
  int SensorCorrection[2];			//Signed integer value for adjusting sensor return values to a known baseline
protected: 

private:
  uint8_t _address;
  
 };

#endif
