#ifndef __MicrophoneController_H
#define __MicrophoneController_H

#include <Arduino.h>
#include "math.h"
#include "arduinoFFT.h"

#define SAMPLES 512              // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define amplitude 250            // Depending on your audio source level, you may need to increase this value

#define MIC_PIN_D 35
#define MIC_PIN_E 32
#define MIC_PIN_F 33

struct MicrophoneData
{
    double leq;
    int Hz125;
    int Hz250;
    int Hz500;
    int Hz1000;
    int Hz2000;
    int Hz4000;
    int Hz8000;
    int Hz16000;
};

class MicrophoneController
{
public:
    MicrophoneController();
    MicrophoneData sensorD;
    MicrophoneData sensorE;
    MicrophoneData sensorF;
    void setData();

private:
    void setSensorDecibelData(MicrophoneData *data, int);
    float decibels(int);
    int decibelMeter(int);
    void spectrumAnalyzer(MicrophoneData *data, int);
    void displayBand(int, int);
};

#endif

// MIC CALIBRATIONS, on 36dB -> adc = 62
// MIC CALIBRATION VALUES
// dB -> peakToPeak value
//  39                 52.5
//  43                 57
//  49                 84.5
//  56                156
//  60                251
//  65                401
//  69                589
//  70                668
//  74               1043