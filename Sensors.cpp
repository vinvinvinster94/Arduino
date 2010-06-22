#include "WProgram.h"
#include "Sensors.h"
//#include "C:Users\Vinnie\Documents\Robot\Arduino\libraries\Ping\Ping.h"
//*****BEGIN PING*****\\

Ping::Ping(int pin)
{
	_pin = pin;
}

long Ping::read(int pin)
{
	pinMode(pin, OUTPUT);
  	digitalWrite(pin, LOW);
  	delayMicroseconds(2);
  	digitalWrite(pin, HIGH);
  	delayMicroseconds(5);
  	digitalWrite(pin, LOW);
	pinMode(pin, INPUT);
	duration = pulseIn(pin, HIGH);
	return duration;
}

long Ping::readin()
{
	delay(100);
	return microsecondsToInches(read(_pin));
}

long Ping::readcm()
{
	delay(100);
	return microsecondsToCentimeters(read(_pin));
}

long Ping::microsecondsToInches(long microseconds)
{
     return microseconds / 74 / 2;
}   

long Ping::microsecondsToCentimeters(long microseconds)
{
     return microseconds / 29 / 2;
}   
//*****END PING*****\\


//*****BEGIN QTI*****\\

long QTI::RCtime(int sensPin){
   long result = 0;
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(1);                       // wait a  ms to make sure cap is discharged

   pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensPin)){    // wait for pin to go low
      result++;
   }

   return result;                   // report results   
}   

QTI::QTI(int pin)
{
	_pin = pin;
}

long QTI::read()
{
	return RCtime(_pin);
}

void QTI::calibrate(int color)
{
	if (color == 1){
		calBlackMin = (calWhiteMax+40);
		if (calBlackMin < 0){
			calBlackMin = 0;
		}
	}
	if (color == 2){
		calWhiteMin = (QTI::read()-30);
		calWhiteMax = (QTI::read()+60);
		if (calWhiteMin < 0){
			calWhiteMin = 0;
		}
	}
}

int QTI::isWhite()
{
	if (QTI::read() >= calWhiteMin && QTI::read() <= calWhiteMax){
		return 1;
	}
	else
		return 0;
}

int QTI::isBlack()
{
	if (QTI::read() >= calBlackMin){
		return 1;
	}
	else
		return 0;
}

//*****END QTI*****\\

//*****BEGIN HM55B*****\\

HM55B::HM55B(int clk, int en, int dio){
	_clk_pin = clk;
	_en_pin = en;
	_dio_pin = dio;
}
void HM55B::ShiftOut(int Value, int BitsCount) {
  for(int i = BitsCount; i >= 0; i--) {
    digitalWrite(_clk_pin, LOW);
    if ((Value & 1 << i) == ( 1 << i)) {
      digitalWrite(_dio_pin, HIGH);
    }
    else {
      digitalWrite(_dio_pin, LOW);
    }
    digitalWrite(_clk_pin, HIGH);
    delayMicroseconds(1);
  }
}

int HM55B::ShiftIn(int BitsCount) {
  int ShiftIn_result;
    ShiftIn_result = 0;
    pinMode(_dio_pin, INPUT);
    for(int i = BitsCount; i >= 0; i--) {
      digitalWrite(_clk_pin, HIGH);
      delayMicroseconds(1);
      if (digitalRead(_dio_pin) == HIGH) {
        ShiftIn_result = (ShiftIn_result << 1) + 1; 
      }
      else {
        ShiftIn_result = (ShiftIn_result << 1) + 0;
      }
      digitalWrite(_clk_pin, LOW);
      delayMicroseconds(1);
	}
  if ((ShiftIn_result & 1 << 11) == 1 << 11) {
    ShiftIn_result = (B11111000 << 8) | ShiftIn_result; 
  }
  return ShiftIn_result;
}

void HM55B::HM55B_Reset() {
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_en_pin, LOW);
  ShiftOut(B0000, 3);
  digitalWrite(_en_pin, HIGH);
}

void HM55B::HM55B_StartMeasurementCommand() {
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_en_pin, LOW);
  ShiftOut(B1000, 3);
  digitalWrite(_en_pin, HIGH);
}

int HM55B::HM55B_ReadCommand() {
  int result = 0;
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_en_pin, LOW);
  ShiftOut(B1100, 3);
  result = ShiftIn(3);
  return result;
}

int HM55B::readangle(){
	_angle = HM55B_ReadCommand();
	return _angle;
}




//*****END HM55B*****\\