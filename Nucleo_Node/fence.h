#ifndef FENCE_H
#define FENCE_H

#include <xtimer.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"

#define SIZE			3
#define RES             ADC_RES_10BIT

double measure (int numberOfMeasurement, double calculateParameter);
int calculate_transfer_value(double voltageFence, int recogniseGate);
int recognise_gate_funk (int port, int pin);

#endif