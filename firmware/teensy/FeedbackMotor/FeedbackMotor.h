#ifndef FeedbackMotor_h
#define FeedbackMotor_h

#include "Arduino.h"

class FeedbackMotor
{
	public:
		FeedbackMotor(int pwmPin, int dirPin, int encA, int encB);
		void move(int steps);
		void stop();
	private:
		int _pwmPin;
};

#endif