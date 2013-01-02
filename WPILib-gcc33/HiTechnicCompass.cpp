/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "HiTechnicCompass.h"
#include "DigitalModule.h"
#include "I2C.h"
#include "WPIErrors.h"

const UINT8 HiTechnicCompass::kAddress;
const UINT8 HiTechnicCompass::kManufacturerBaseRegister;
const UINT8 HiTechnicCompass::kManufacturerSize;
const UINT8 HiTechnicCompass::kSensorTypeBaseRegister;
const UINT8 HiTechnicCompass::kSensorTypeSize;
const UINT8 HiTechnicCompass::kHeadingRegister;

/**
 * Constructor.
 * 
 * @param moduleNumber The digital module that the sensor is plugged into (1 or 2).
 */
HiTechnicCompass::HiTechnicCompass(UINT8 moduleNumber)
	: m_i2c (NULL)
{
	DigitalModule *module = DigitalModule::GetInstance(moduleNumber);
	if (module)
	{
		m_i2c = module->GetI2C(kAddress);
	
		// Verify Sensor
		const UINT8 kExpectedManufacturer[] = "HiTechnc";
		const UINT8 kExpectedSensorType[] = "Compass ";
		if ( ! m_i2c->VerifySensor(kManufacturerBaseRegister, kManufacturerSize, kExpectedManufacturer) )
		{
			wpi_setWPIError(CompassManufacturerError);
			return;
		}
		if ( ! m_i2c->VerifySensor(kSensorTypeBaseRegister, kSensorTypeSize, kExpectedSensorType) )
		{
			wpi_setWPIError(CompassTypeError);
		}
	}
}

/**
 * Destructor.
 */
HiTechnicCompass::~HiTechnicCompass()
{
	delete m_i2c;
	m_i2c = NULL;
}

/**
 * Get the compass angle in degrees.
 * 
 * The resolution of this reading is 1 degree.
 * 
 * @return Angle of the compass in degrees.
 */
float HiTechnicCompass::GetAngle()
{
	UINT16 heading = 0;
	if (m_i2c)
	{
		m_i2c->Read(kHeadingRegister, sizeof(heading), (UINT8 *)&heading);

		// Sensor is little endian... swap bytes
		heading = (heading >> 8) | (heading << 8);
	}
	return (float)heading;
}

