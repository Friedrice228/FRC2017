/*
 * LEDLightStrip.h
 *
 *  Created on: Jan 20, 2017
 *      Author: DriversStation
 */

#ifndef SRC_LEDLIGHTSTRIP_H_
#define SRC_LEDLIGHTSTRIP_H_

#include <LEDLightStrip.h>
#include <mindsensors.h>
#include <LEDLightStrip.h>
using mindsensors::CANLight;

class LEDLightStrip {
public:
	LEDLightStrip();
	void LEDGreen();

	CANLight *light;
};

#endif /* SRC_LEDLIGHTSTRIP_H_ */
