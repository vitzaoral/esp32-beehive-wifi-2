#ifndef __MicrophoneController_H
#define __MicrophoneController_H

#include <Arduino.h>
#include "math.h"
#include "arduinoFFT.h"

#define SAMPLES 512              // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define amplitude 250            // Depending on your audio source level, you may need to increase this value
#define MIC_PIN_D 32
#define MIC_PIN_E 33
#define MIC_PIN_F 35

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

// FIRST MIC CALIBRATION VALUES
// dB -> peakToPeak value
//  33                 277
//  39                 279
//  45                 284
//  49                 290
//  55                 326
//  60                 425
//  66                 638
//  69                 953
//  75                1515
//  79                2371


// SECOND MIC CALIBRATIONS VALUES
//  32                 360
//  41                 370
//  47                 381
//  53                 422
//  56                 516
//  60                 609
//  65                 809
//  67                1062
//  71                1705
//  75                2633
//  77                3264