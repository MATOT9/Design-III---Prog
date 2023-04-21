#pragma once                    // Make sure the library is only import once
#include <math.h>               // Library for various mathematical functions


typedef unsigned short int ushort;


// ====================== Utilities ======================

float average(float * array) {
    /*
        Compute the average of a given array.

        Parameters
        ----------
        array : float
            input aray.

        Returns
        -------
        avg : float
            The numeric average of the input array.
    */

    float sum = 0;
    ushort len = sizeof(array);

    for (ushort i = 0; i < len; i++)
        sum += array[i];

    float avg = ((float) sum) / len;  // Average will be fractional, so float is appropriate
    return avg;
}

float roundDec(float in, ushort P) {
    /*
        Round a number to the specified decimal.

        Parameters
        ----------
        in : float
            input value.
        P : unsigned short int
            the decimal power to round to.

        Returns
        -------
        rnd : float
            The rounded value.
    */

    float rnd = round(in*P) / P;
    return rnd;
}

ushort extractDigit(short value, ushort P) {
    /*
        Extract digit of the specified position.

        Parameters
        ----------
        value : short
            input value.
        P : unsigned short int
            the power of ten of the digit's position.

        Returns
        -------
        digit : short
            The extracted digit.
    */

    short digit = abs(value) / P % 10;
    return digit;
}


// ====================== Wheatstone bridge equation ======================

float Vbias_wheatstone(float Vg){
    /*
        Compute the resistance of the NTCLE413E2103F524A thermistor.

        This function uses the Wheatstone bridge voltage analysis to find the
        value of the unknown resistor (thermistor).

        Parameter
        ---------
        Vg : float
            Voltage output in Volts of the selected ADC.

        Returns
        -------
        Rx : float
            Thermistor resistance value in Ohms.
    */

    static const float Vs = 3.3;        // Weatstone bridge supply voltage
    static const ushort Rmux = 9;       // Analog MUX ON resistance
    static const ushort Rref = 10E3;    // Wseatstone bridge's resistances value

    Vg = round(Vg*10E3) / 10E3;         // Round the voltage to the 4th decimal since it's the ADC's precision
    float Rx = Rref*(1 - (4*Vg) / (2*Vg - Vs));     // Unknown thermistor resistance

    // Substract the mux ON resistance to get thermistor resistance
    Rx = Rx - Rmux;
    return Rx;
}


// ====================== Steinhart-Hart equation ======================

float Vbias_steinhart(float R){
    /*
        Compute the temperature of the NTCLE413E2103F524A thermistor.

        This function uses the Steinhart-Hart equation to get the thermistor
        temperature with the input resistance value.

        Parameter
        ---------
        R : float
            Measured resistance value in Ohms.

        Returns
        -------
        temp : float
            Thermistors temperature in Celsius.
    */

    // Steinhart-Hart temperature coefficients for the thermistor
    static const float A = 0.00335401643468053;
    static const float B = 0.000300130825115663;
    static const float C = 5.08516494379094E-06;
    static const float D = 2.18765049258341E-07;

    float Log = log(R);
	float temp = A + B*Log + C*powf(Log, 2) + D*powf(Log, 3);    // Run the c-Hart equation
    temp = powf(temp, -1) - 273.15;                              // Convert Kelvin to Celsius and return the temperature
	return temp;
}
