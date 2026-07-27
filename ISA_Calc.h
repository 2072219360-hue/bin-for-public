#pragma once
namespace ISA {

	class ISA_Calc {

	private:
		//Base Altitude.
		int altitudeBase;
		//Base Temperature at altitude up to a height of 11,000 m & 36,000 ft.
		float tempBase_11;
		//Base Temperature at altitude up to a height of 20,000 m & 65,000 ft.
		float tempBase_20_32;
		//Base Temperature at altitude up to a height of 32,000 m & 105,000 ft.
		float tempBase_32;
		//Lapse rate °„C/1000m up to a height of 11km & 36,000 ft.
		double lapseRate_11;
		//Lapse rate °„C/1000m up to a height of 20km & 65,000 ft.
		double lapseRate_20;
		//Lapse rate °„C/1000m up to a height of 32km & 105,000 ft.
		double lapseRate_32;

	public:
		//This is the main function to calculate the Temperature in celsius by given altitude by meters.
		double getTemperature(int altitude);
		//A utility function to convert feet to meters.
		double feetToMeter(double feet);
		//A utility function to convert meters to feet.
		double meterToFeet(double meter);
		//A utility function to convert kelvin to celsius
		double kelvinToCelsius(double kelvin);
		//a Default constructor.
		ISA_Calc();
	};

}
