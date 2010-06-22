#ifndef Sensors_h
#define Sensors_h


class Ping
{	
	public:
		Ping(int pin);
		long readcm();
		long readin();
	private:
		int _pin;
		long duration;
		long read(int pin);
		long microsecondsToCentimeters(long microseconds);
		long microsecondsToInches(long microseconds);
};


class QTI
{
	public:
		QTI(int pin);
		long read();
		void calibrate(int color /*1=black 2=white*/);
		int isBlack();
		int isWhite();
	private:
		int _pin;
		long RCtime(int sensPin);
		int qtival;
		int calWhiteMin;
		int calWhiteMax;
		int calBlackMin;
};

class HM55B
{	
	public:
		HM55B(int clk, int en, int dio);
		int readangle();
	private:
		int _clk_pin;
		int _en_pin;
		int _dio_pin;
		void ShiftOut(int Value, int BitsCount);
		int ShiftIn(int BitsCount);
		void HM55B_Reset();
		void HM55B_StartMeasurementCommand();
		int HM55B_ReadCommand();
		int X_Data;
		int Y_Data;
		int _angle;		
};
#endif