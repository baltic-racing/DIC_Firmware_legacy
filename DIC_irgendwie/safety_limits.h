/*
 * security_limits.h
 *
 * Created: 13.07.2019 16:42:24
 *  Author: Simon
 */ 


#ifndef SAFETY_LIMITS_H_
#define SAFETY_LIMITS_H_


// Minimum revolutions per minute to enable error checking
// Voltage is excluded
// [RPM]
#define MINIMUM_RPM_CAR_ON 4000

// Oil temperature, maximum only
// [°C]
#define OIL_TEMPERATURE_MAX 120

// Oil pressure minimum
// [0,01 Bar]
#define OIL_PRESSURE_MIN 50 // 0,5 Bar

// Oil pressure maximum
// [0,01 Bar]
#define OIL_PRESSURE_MAX 400 // 4 Bar

// Oil pressure timer minimum
// After this period of to low oil pressure
// [0,1 s]
#define OIL_PRESSURE_LOW_TIME 10

// Cooling temperature, minimum only
// [°C]
#define COOLING_TEMPERATUR_MAX 105

// Battery voltage, maximum only
// [0,1V]
#define BATTERY_VOLTAGE_MAX 150

// Brake fluid temperature, maximum only
// [°C]
#define BRAKEFLUID_TEMPERATURE_MAXIMUM 145


#endif /* SAFETY_LIMITS_H_ */