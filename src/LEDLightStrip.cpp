/*
 * LEDLightStrip.cpp
 *
 *  Created on: Jan 20, 2017
 *      Author: DriversStation
 */

#include <LEDLightStrip.h>
#include <mindsensors.h>
using mindsensors::CANLight;


LEDLightStrip::LEDLightStrip() {

	light = new CANLight(5);

}
	void LEDLightStrip::LEDGreen(){

		light->ShowRGB(0, 255, 0);

}


