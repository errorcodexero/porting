/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2011. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "networktables/TransactionStart.h"
#include "networktables/Buffer.h"
#include "networktables/InterfaceConstants.h"

namespace NetworkTables
{

void TransactionStart::Encode(Buffer *buffer)
{
	buffer->WriteByte(kNetworkTables_TRANSACTION);
}

} // namespace