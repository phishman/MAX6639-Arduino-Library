/*
* Arduino Library for MAX6639 I2C 2-Chan PWM Fan/Temp Controller
*
* Copyright (C) 2015 RF Designs. All rights reserved.
*
* Author: Bob Frady <rfdesigns@live.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License v2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 021110-1307, USA.
*
* If you can't comply with GPLv2, alternative licensing terms may be
* arranged. Please contact RF Designs <rfdesigns@live.com> for proprietary
* alternative licensing inquiries.
*/


#include <wire.h>

#include <max6639.h>


void MAX6639::begin(void)
{
  _address = 0x2C;
  Wire.begin();
  SensorCorrection[0] = 0;
  SensorCorrection[1] = 0;
}

void MAX6639::begin(uint8_t addr)
{
  if(addr < 3) {
    _address = normal_i2c[addr];
  } else {
    _address = addr;
  }
  Wire.begin();
  SensorCorrection[0] = 0;
  SensorCorrection[1] = 0;
}


void MAX6639::writeByte(uint8_t data, uint8_t reg) {
  Wire.beginTransmission(_address);
  I2CWRITE((uint8_t) reg);
  
  I2CWRITE((uint8_t) data);
  Wire.endTransmission();

  return;
}

bool MAX6639::readByte(uint8_t *data, uint8_t reg) {
  Wire.beginTransmission(_address);
  I2CWRITE((uint8_t) reg);
  Wire.endTransmission();
  uint8_t timeout=0;
  
  Wire.requestFrom(_address, (uint8_t) 0x01);
  while(Wire.available() < 1) {
    timeout++;
	if(timeout > I2CTIMEOUT) {
	  return(true);
	}
	delay(1);
  } 			// Experimental
  
  *data = I2CREAD();

return(false);
}

uint8_t MAX6639::readTempC(uint8_t ch) {
  uint8_t Temperature;
  
  readByte(&Temperature, MAX6639_REG_TEMP(ch));
  return((Temperature)+SensorCorrection[ch]);
}

uint8_t MAX6639::readTempF(uint8_t ch) {
  uint8_t Temperature;
  
  Temperature = readTempC(ch);
  Temperature = Temperature * 9/5 + 32;
  return(Temperature);
}

uint8_t MAX6639::getStatus(void) {
  uint8_t Status;
  
  readByte(&Status, MAX6639_REG_STATUS);
  return(Status);
}

uint8_t MAX6639::getMask(void) {
  uint8_t Mask;
  
  readByte(&Mask, MAX6639_REG_OUTPUT_MASK);
  return(Mask);
}

void MAX6639::setMask(uint8_t Mask) {
  
  writeByte(Mask, MAX6639_REG_OUTPUT_MASK);
  return;
}

uint8_t MAX6639::getConfig(void) {
  uint8_t Config;
  
  readByte(&Config, MAX6639_REG_GCONFIG);
  return(Config);
}

void MAX6639::setConfig(uint8_t Config) {
  
  writeByte(Config, MAX6639_REG_GCONFIG);
  return;
}

bool MAX6639::getDiodeFault(uint8_t ch) {
  uint8_t Fault;
  
  readByte(&Fault, MAX6639_REG_TEMP_EXT(ch));
  Fault &= 0x01;
  if(Fault)
    return(true);
  else
    return(false);
}

double MAX6639::getExtTemp(uint8_t ch) {
  uint8_t Temp;
  double extTemp = 0.00;
  
  extTemp = readTempC(ch);
  readByte(&Temp, MAX6639_REG_TEMP_EXT(ch));
  if(Temp & 0x80)
    extTemp += 0.50;
  if(Temp & 0x40)
    extTemp += 0.25;
  if(Temp & 0x20)
    extTemp += 0.125;
	
  return(extTemp);
}


uint8_t MAX6639::getALERTLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_ALERT_LIMIT(ch));
  Limit -= SensorCorrection[ch];
  return(Limit);
}

void MAX6639::setALERTLimit(uint8_t ch, uint8_t Limit) {

  Limit += SensorCorrection[ch];
  writeByte(Limit, MAX6639_REG_ALERT_LIMIT(ch));
  return;
}

uint8_t MAX6639::getOTLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_OT_LIMIT(ch));
  Limit -= SensorCorrection[ch];
  return(Limit);
}

void MAX6639::setOTLimit(uint8_t ch, uint8_t Limit) {

  Limit += SensorCorrection[ch];
  writeByte(Limit, MAX6639_REG_OT_LIMIT(ch));
  return;
}

uint8_t MAX6639::getTHERMLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_THERM_LIMIT(ch));
  Limit -= SensorCorrection[ch];
  return(Limit);
}

void MAX6639::setTHERMLimit(uint8_t ch, uint8_t Limit) {

  Limit += SensorCorrection[ch];
  writeByte(Limit, MAX6639_REG_THERM_LIMIT(ch));
  return;
}


uint8_t MAX6639::getFanConfig(uint8_t reg) {
  uint8_t Config;
  
  readByte(&Config, reg);
  return(Config);
}

void MAX6639::setFanConfig(uint8_t reg, uint8_t Config) {
  
  writeByte(Config, reg);
  return;
}

uint8_t MAX6639::getFanTachCount(uint8_t ch) {
  uint8_t Count;
  
  readByte(&Count, MAX6639_REG_FAN_CNT(ch));
  return(Count);
}

void MAX6639::setFanTachCount(uint8_t ch, uint8_t Count) {
  
  writeByte(Count, MAX6639_REG_FAN_CNT(ch));
  return;
}

uint8_t MAX6639::getFanTargetTach(uint8_t ch) {
  uint8_t Count;
  
  readByte(&Count, MAX6639_REG_TARGET_CNT(ch));
  return(Count);
}

void MAX6639::setFanTargetTach(uint8_t ch, uint8_t Count) {
  
  writeByte(Count, MAX6639_REG_TARGET_CNT(ch));
  return;
}

uint8_t MAX6639::getFanPPR(uint8_t ch) {
  uint8_t ppr;
  
  readByte(&ppr, MAX6639_REG_FAN_PPR(ch));
  return(ppr & 0xC0);
}

uint8_t MAX6639::getFanPPRval(uint8_t ch) {
  uint8_t ppr;
  
  readByte(&ppr, MAX6639_REG_FAN_PPR(ch));
  ppr &= 0xC0;
  return(ppr_values[(ppr >> 6)]);
}

void MAX6639::setFanPPR(uint8_t ch, uint8_t ppr_set) {
  uint8_t ppr;
  
  readByte(&ppr, MAX6639_REG_FAN_PPR(ch));
  ppr &= 0x3F;
  ppr |= (ppr_set & 0xC0);
  writeByte(ppr, MAX6639_REG_FAN_PPR(ch));
  return;
}

void MAX6639::setFanPPRval(uint8_t ch, uint8_t ppr_set) {
  uint8_t ppr;
  
  readByte(&ppr, MAX6639_REG_FAN_PPR(ch));
  ppr &= 0x3F;
  ppr |= ((ppr_set<<6) & 0xC0);
  writeByte(ppr, MAX6639_REG_FAN_PPR(ch));
  return;
}

uint8_t MAX6639::getFanMinTachCount(uint8_t ch) {
  uint8_t mintach;
  
  readByte(&mintach, MAX6639_REG_FAN_MINTACH(ch));
  return(mintach & 0x3F);
}

void MAX6639::setFanMinTachCount(uint8_t ch, uint8_t mintach) {
  uint8_t ppr;
  
  mintach &= 0x3F;
  ppr = getFanPPR(ch);
  ppr |= mintach;
  writeByte(ppr, MAX6639_REG_FAN_MINTACH(ch));
  return;
}

uint8_t MAX6639::getFanDuty(uint8_t ch) {
  uint8_t duty;
  
  readByte(&duty, MAX6639_REG_TARGTDUTY(ch));
  return(duty);
}

uint8_t MAX6639::getFanDutyPercent(uint8_t ch) {
  uint8_t duty;
  
  readByte(&duty, MAX6639_REG_TARGTDUTY(ch));
  duty = (uint8_t) (duty * 100 / 120) ;
  return(duty);
}

void MAX6639::setFanDuty(uint8_t ch, uint8_t duty) {
  
  writeByte(duty, MAX6639_REG_TARGTDUTY(ch));
  return;
}

void MAX6639::setFanDutyPercent(uint8_t ch, uint8_t duty) {
  uint8_t val;
  if(duty > 100)
    duty = 100;
	
  val = (uint8_t) duty * 120 / 100;
  
  writeByte(val, MAX6639_REG_TARGTDUTY(ch));
  return;
}

uint8_t MAX6639::getFanStartTempC(uint8_t ch) {
  uint8_t Temperature;
  
  readByte(&Temperature, MAX6639_REG_FAN_START_TEMP(ch));
  Temperature -= SensorCorrection[ch];
  return(Temperature);
}

void MAX6639::setFanStartTempC(uint8_t ch, uint8_t Temperature) {
  
  Temperature += SensorCorrection[ch];
  writeByte(Temperature, MAX6639_REG_FAN_START_TEMP(ch));
  return;
}

uint8_t MAX6639::getDevID(void) {
  uint8_t id;
  
  readByte(&id, MAX6639_REG_DEVID);
  return(id);
}

uint8_t MAX6639::getManuID(void) {
  uint8_t id;
  
  readByte(&id, MAX6639_REG_MANUID);
  return(id);
}

uint8_t MAX6639::getDevRev(void) {
  uint8_t rev;
  
  readByte(&rev, MAX6639_REG_DEVREV);
  return(rev);
}

void MAX6639::setFanSpinup(uint8_t ch, bool state) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG3(ch));
  if(state) {
    curr_config &= 0x7F;
  } else {
    curr_config |= 0x80;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG3(ch), curr_config);
}

void MAX6639::setFanTherm(uint8_t ch, bool state) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG3(ch));
  if(state) {
    curr_config |= 0x40;
  } else {
    curr_config &= 0xBF;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG3(ch), curr_config);
}

void MAX6639::setFanPulseStretch(uint8_t ch, bool state) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG3(ch));
  if(state) {
    curr_config &= 0xDF;
  } else {
    curr_config |= 0x20;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG3(ch), curr_config);
}


float MAX6639::getFanPWMFreq(uint8_t ch) {
  uint16_t freq;
  bool range;
  uint8_t curr_config;
  
  curr_config = getConfig();
  curr_config &= 0xF7;
  range = curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG3(ch));
  curr_config &= 0xFC;
  freq &= 0x03;
  switch(freq) {
    case 0:
	  if(range)
	    return(5000.0);
	  else
	    return(20.0);
	  break;
	case 1:
	  if(range)
	    return(8330.0);
	  else
	    return(33.33);
	  break;
	case 2:
	  if(range)
	    return(12500.0);
	  else
	    return(50.0);
	  break;
	case 3:
	  if(range)
	    return(25000.0);
	  else
	    return(100.0);
	  break;
	default:
	  break;
  }
  return(0.0);
}


void MAX6639::setFanPWMFreq(uint8_t ch, uint8_t freq) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  curr_config &= 0xF7;
  curr_config |= ((freq & 0x04)<<1);
  setConfig(curr_config);
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG3(ch));
  curr_config &= 0xFC;
  freq &= 0x03;
  curr_config |= freq;
  setFanConfig(MAX6639_REG_FAN_CONFIG3(ch), curr_config);
}


void MAX6639::setRun(bool state) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  if(state) {
    curr_config &= 0x7F;
  } else {
    curr_config |= 0x80;
  }
  setConfig(curr_config);
}

bool MAX6639::isRunning(void) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  if(curr_config & 0x80) {
    return(false);
  } else {
    return(true);
  }
}

void MAX6639::setPOR(bool state) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  if(state) {
    curr_config |= 0x80;
  } else {
    curr_config &= 0xBF;
  }
  setConfig(curr_config);
}

void MAX6639::setChan2Source(uint8_t source) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  if(source == 1) {
    curr_config |= 0x10;
  } else {
    curr_config &= 0xEF;
  }
  setConfig(curr_config);
}

void MAX6639::setPWMPolarity(uint8_t ch, bool state) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  if(state) {
    curr_config |= 0x02;
  } else {
    curr_config &= 0xFD;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG2a(ch), curr_config);
}

void MAX6639::setPWMMode(uint8_t ch, bool state) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  if(state) {
    curr_config |= 0x80;
  } else {
    curr_config &= 0x7F;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
}

void MAX6639::setFanControl(uint8_t ch, uint8_t TChan) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0xF3;
  curr_config |= TChan;
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
}

uint8_t MAX6639::getFanRPMRange(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x03;
  return(curr_config);
}

void MAX6639::setFanRPMRange(uint8_t ch, uint8_t range) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0xFC;
  curr_config |= range;

  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
}


void MAX6639::initDefaults(void) {  	//Setup default values as listed in max6639.h  Specific channels and
  setPOR(true);							//Values may be moidfied at run-time.
  delay(100);
  setConfig(DEFAULT_BUS_TIMEOUT|DEFAULT_CH2_SOURCE|((DEFAULT_PWM_FREQ & 0x04)<<1));
  setFanPPR(0, DEFAULT_FAN_PPR);
  setFanPPR(1, DEFAULT_FAN_PPR);
  setPWMPolarity(0, DEFAULT_PWM_POLARITY);
  setPWMPolarity(1, DEFAULT_PWM_POLARITY);
  setFanConfig(MAX6639_REG_FAN_CONFIG3(0), (DEFAULT_PWM_FREQ&0x03));
  //setFanConfig(MAX6639_REG_FAN_CONFIG3(0), (MAX6639_FAN_CONFIG3_THERM_FULL_SPEED|DEFAULT_PWM_FREQ&0x03));
  setFanConfig(MAX6639_REG_FAN_CONFIG3(1), (DEFAULT_PWM_FREQ&0x03));
  //setFanConfig(MAX6639_REG_FAN_CONFIG3(1), (MAX6639_FAN_CONFIG3_THERM_FULL_SPEED|DEFAULT_PWM_FREQ&0x03));
  setTHERMLimit(0, DEFAULT_LIMIT_THERM);
  setTHERMLimit(1, DEFAULT_LIMIT_THERM);
  setALERTLimit(0, DEFAULT_LIMIT_ALERT);
  setALERTLimit(1, DEFAULT_LIMIT_ALERT);
  setOTLimit(0, DEFAULT_LIMIT_OVERTEMP);
  setOTLimit(1, DEFAULT_LIMIT_OVERTEMP);
  setFanStartTempC(0, DEFAULT_MIN_START_TEMP); 
  setFanStartTempC(1, DEFAULT_MIN_START_TEMP);
  setFanMinimumSpeed(0, DEFAULT_MIN_TACH_ENABLE, DEFAULT_MIN_TACH);
  setFanMinimumSpeed(1, DEFAULT_MIN_TACH_ENABLE, DEFAULT_MIN_TACH);
  setFanConfig(MAX6639_REG_FAN_CONFIG1(0), DEFAULT_RPM_RANGE|DEFAULT_FAN_ROC);
  setFanAutoRPM(0, TCHAN_0);
  setFanConfig(MAX6639_REG_FAN_CONFIG1(1), DEFAULT_RPM_RANGE|DEFAULT_FAN_ROC);
  setFanAutoRPM(1, TCHAN_1);
}

void MAX6639::setFanMinimumSpeed(uint8_t ch, bool state, uint8_t count) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  if(state) {
    curr_config |= 0x01;
  } else {
    curr_config &= 0xFE;
  }
  setFanConfig(MAX6639_REG_FAN_CONFIG2a(ch), curr_config);
  setFanMinTachCount(ch, count);
}

void MAX6639::setFanRateOfChange(uint8_t ch, uint8_t val) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x8F;
  val &= 0x07;
  
  curr_config |= (val << 4);
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
}

uint8_t MAX6639::getFanRateOfChange(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x70;
  
  return(curr_config >> 4);
}

void MAX6639::setFanRPMStepA(uint8_t ch, uint8_t val) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  curr_config &= 0x8F;
  val &= 0x07;
  
  curr_config |= (val << 4);
  setFanConfig(MAX6639_REG_FAN_CONFIG2a(ch), curr_config);
}

uint8_t MAX6639::getFanRPMStepA(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  curr_config &= 0x70;
  
  return(curr_config >> 4);
}
  
void MAX6639::setFanTempStepA(uint8_t ch, uint8_t val) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  curr_config &= 0xF3;
  val &= 0x03;
  
  curr_config |= (val << 2);
  setFanConfig(MAX6639_REG_FAN_CONFIG2a(ch), curr_config);
}

uint8_t MAX6639::getFanTempStepA(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2a(ch));
  curr_config &= 0xF3;
  
  return(curr_config >> 2);
}

void MAX6639::setFanRPMStepB(uint8_t ch, uint8_t val) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2b(ch));
  curr_config &= 0x0F;
  val &= 0x0F;
  
  curr_config |= (val << 4);
  setFanConfig(MAX6639_REG_FAN_CONFIG2b(ch), curr_config);
}

uint8_t MAX6639::getFanRPMStepB(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2b(ch));
  curr_config &= 0xF0;
  
  return(curr_config >> 4);
}

void MAX6639::setFanStartStep(uint8_t ch, uint8_t val) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2b(ch));
  curr_config &= 0xF0;
  val &= 0x0F;
  
  curr_config |= val;
  setFanConfig(MAX6639_REG_FAN_CONFIG2b(ch), curr_config);
}

uint8_t MAX6639::getFanStartStep(uint8_t ch) {
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG2b(ch));
  curr_config &= 0x0f;
  
  return(curr_config);
}

uint16_t MAX6639::getFanRPM(uint8_t ch) {
  uint16_t RPM = 0, Freq = 0;
  uint8_t TachCount, ppr;						//Datasheet problematic in determining RPM
  
  TachCount = getFanTachCount(ch);				//The tach count value which each rev/ppr is subtracted from 255
  ppr = getFanPPRval(ch);						//Number of Pulses per revolution
  Freq = rpm_ranges[getFanRPMRange(ch)]; 		//Clk Frequency is equal to the selected RPM range...
  
  if(TachCount == 255) {						// Just to make a "clean" zero value (no rounding errors) when it is near zero...
    RPM = 0;
  } else {
    TachCount = (255 - TachCount) / ppr;
    RPM = (uint16_t)((long)Freq *  (long)TachCount / (long)60);
  }

  return(RPM);
}

void MAX6639::setFanAutoRPM(uint8_t ch, uint8_t Chan) {  // Chan Values of TCHAN_0 ot TCHAN_1
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x73;
  
  curr_config |= Chan;
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
}

void MAX6639::setFanPWMMode(uint8_t ch, uint8_t Duty) {		//Duty Cycle in 0% to 100%
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x73;
  
  curr_config |= 0x80;
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
  setFanDutyPercent(ch, Duty);
}

void MAX6639::setFanManualRPM(uint8_t ch, uint8_t Tach) {	// Target Tach value from 0 to 255
  uint8_t curr_config;
  
  curr_config = getFanConfig(MAX6639_REG_FAN_CONFIG1(ch));
  curr_config &= 0x73;
  
  setFanConfig(MAX6639_REG_FAN_CONFIG1(ch), curr_config);
  setFanTargetTach(ch, Tach);
}


int MAX6639::getFanSensorCorrection(uint8_t ch) {
  return(SensorCorrection[ch]);
}

void MAX6639::setFanSensorCorrection(uint8_t ch, int Val) {
  SensorCorrection[ch] = Val;
}