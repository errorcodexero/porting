// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "RateGyro.h"

#define AVERAGE_BITS 10
#define OVERSAMPLE_BITS 0
#define SAMPLES_PER_SECOND 50
#define	SAMPLE_RATE (SAMPLES_PER_SECOND*(1<<(AVERAGE_BITS+OVERSAMPLE_BITS)))
#define	SENSITIVITY 0.010	// volts per degree per second

void RateGyro::InitChannel()
{
    // configure the A/D hardware sample rate and
    // the oversampling/averaging engine
    SetAverageBits(AVERAGE_BITS);
    SetOversampleBits(OVERSAMPLE_BITS);
    GetModule()->SetSampleRate(SAMPLE_RATE);
    Zero();
}

RateGyro::RateGyro(UINT8 moduleNumber, UINT32 channel) :
	    AnalogChannel(moduleNumber, channel)
{
    InitChannel();
}

RateGyro::RateGyro(UINT32 channel) : AnalogChannel(channel)
{
    InitChannel();
}

void RateGyro::Zero()
{
    // wait at least two sample intervals
    Wait(2.0/SAMPLE_RATE);
    // read the average value
    m_zero = GetAverageVoltage();
    printf("DriveBase: gyro zero voltage %g\n", m_zero);
}

float RateGyro::GetRate()
{
    return (GetAverageVoltage() - m_zero) / SENSITIVITY;
}
