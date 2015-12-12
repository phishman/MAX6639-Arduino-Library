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
}

void MAX6639::begin(uint8_t addr)
{
  if(addr < 3) {
    _address = normal_i2c[addr];
  } else {
    _address = addr;
  }
  Wire.begin();
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
  return(Temperature);
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


uint8_t MAX6639::getAlertLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_ALERT_LIMIT(ch));
  return(Limit);
}

void MAX6639::setAlertLimit(uint8_t Limit, uint8_t ch) {

  writeByte(Limit, MAX6639_REG_ALERT_LIMIT(ch));
  return;
}

uint8_t MAX6639::getOTLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_OT_LIMIT(ch));
  return(Limit);
}

void MAX6639::setOTLimit(uint8_t Limit, uint8_t ch) {

  writeByte(Limit, MAX6639_REG_OT_LIMIT(ch));
  return;
}

uint8_t MAX6639::getTHERMLimit(uint8_t ch) {
  uint8_t Limit;
  
  readByte(&Limit, MAX6639_REG_THERM_LIMIT(ch));
  return(Limit);
}

void MAX6639::setTHERMLimit(uint8_t Limit, uint8_t ch) {

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
  return(ppr >> 6);
}

void MAX6639::setFanPPR(uint8_t ch, uint8_t ppr) {
  
  readByte(&ppr, MAX6639_REG_FAN_PPR(ch));
  ppr &= 0x3F;
  ppr |= (ppr & 0xC0);
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
  duty = (uint8_t) (duty * 255 / 120) ;
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
	
  val = (uint8_t) duty * 120 / 255;
  
  writeByte(val, MAX6639_REG_TARGTDUTY(ch));
  return;
}

uint8_t MAX6639::getFanStartTempC(uint8_t ch) {
  uint8_t Temperature;
  
  readByte(&Temperature, MAX6639_REG_FAN_START_TEMP(ch));
  return(Temperature);
}

void MAX6639::setFanStartTempC(uint8_t ch, uint8_t Temperature) {
  
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

void MAX6639::setFanPWMFreq(uint8_t ch, uint8_t freq) {
  uint8_t curr_config;
  
  curr_config = getConfig();
  curr_config &= 0xF7;
  curr_config |= (freq & 0x04);
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


void MAX6639::maxRegDump(uint8_t start, uint8_t end) {
  uint8_t val;
  
  Serial.print("MAX6639 Register Dump 0x");
  Serial.println(_address, HEX);
  Serial.println("--- BEGIN DUMP ---");
  for(uint8_t n=start;n <= end;n++) {
    readByte(&val, n);
	if(!(n%8)) {
      Serial.print("\n");
    }
	if(n<16)
	  Serial.print("0");
    Serial.print(n, HEX);
    Serial.print(":");
	if(val < 0x10)
	  Serial.print("0");
    Serial.print(val, HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
  Serial.println("--- END DUMP ---");  
}
