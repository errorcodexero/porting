// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef RATEGYRO_H
#define RATEGYRO_H

#include "WPILib.h"

// turn-rate gyro connected to an analog input channel

class RateGyro : public AnalogChannel {
private:
	float m_zero;

public:
	RateGyro(UINT8 moduleNumber, UINT32 channel);
	explicit RateGyro(UINT32 channel);

	void Zero();
	float GetRate();	// in degrees per second

private:
	void InitChannel();
};

#endif
