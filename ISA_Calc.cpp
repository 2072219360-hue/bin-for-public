#include "ISA_Calc.h"
#include <iostream>
#include <math.h>   
using namespace ISA;

/*
* Returns International Standard Atmosphere temperature as a function of height in meters
* https://en.wikipedia.org/wiki/Barometric_formula#Pressure_equations
*/
double ISA_Calc::getTemperature(int altitude)
{
	double temperature;
	altitudeBase = 0;
	if (altitude <= 11000)
	{
		temperature = tempBase_11 + lapseRate_11 * (altitude - altitudeBase);
	}
	else if (altitude <= 20000)
	{
		altitudeBase = 11000;
		temperature = tempBase_20_32 + lapseRate_20 * (altitude - altitudeBase);
	}
	else if (altitude <= 32000)
	{
		altitudeBase = 20000;
		temperature = tempBase_20_32 + lapseRate_32 * (altitude - altitudeBase);
	}
	else
	{
		altitudeBase = 20000;
		temperature = tempBase_20_32 + lapseRate_32 * (altitude - altitudeBase);
		
	}

	return temperature;
}

/*
* This function convert Kelvin temperature into Celsius.
*/
double ISA_Calc::kelvinToCelsius(double kelvin)
{
	double tmpCel = kelvin - 273.15;
	return tmpCel;
}

/*
* This function convert Feet temperature into Meter.
*/
double ISA_Calc::feetToMeter(double feet)
{
	double meter = feet / 3.28084;
	return meter;
}

/*
* This function convert Meter temperature into Feet.
*/
double ISA_Calc::meterToFeet(double meter)
{
	double feet = meter * 3.28084;
	return feet;
}

/*
* a Default constructor.
*/
ISA::ISA_Calc::ISA_Calc()
{
	altitudeBase = 0;
	tempBase_11 = 15;
	tempBase_20_32 = -56.5;
	tempBase_32 = -44.5;
	lapseRate_11 = -0.0065;
	lapseRate_20 = 0.0;
	lapseRate_32 = 0.0010;
}