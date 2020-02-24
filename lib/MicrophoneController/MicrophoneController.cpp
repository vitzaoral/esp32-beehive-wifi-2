#include "MicrophoneController.h"

unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;

// https://github.com/kosme/arduinoFFT, in IDE, Sketch, Include Library, Manage Library, then search for FFT
arduinoFFT FFT = arduinoFFT();

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

MicrophoneController::MicrophoneController()
{
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
    Serial.println("Microphones I2S setup DONE");
}

void MicrophoneController::setData()
{
    Serial.print("MIC D: ");
    setSensorDecibelData(&sensorD, MIC_PIN_D);
    Serial.print(sensorD.leq);
    Serial.println(" dB");

    Serial.print("MIC E: ");
    setSensorDecibelData(&sensorE, MIC_PIN_E);
    Serial.print(sensorE.leq);
    Serial.println(" dB");

    Serial.print("MIC F: ");
    setSensorDecibelData(&sensorF, MIC_PIN_F);
    Serial.print(sensorF.leq);
    Serial.println(" dB");

    // TODO: Spectrum Analyzer
    // Serial.print("Spectrum Analyzer MIC D: ");
    // spectrumAnalyzer(&sensorD, MIC_PIN_D);
    // Serial.print("Spectrum Analyzer MIC E: ");
    // spectrumAnalyzer(&sensorD, MIC_PIN_E);
    // Serial.print("Spectrum Analyzer MIC F: ");
    // spectrumAnalyzer(&sensorD, MIC_PIN_F);
}

void MicrophoneController::setSensorDecibelData(MicrophoneData *data, int microphonePin)
{
    int count = 120; // ~ 10 sec
    int num[count];

    for (int i = 0; i < count; i++)
    {
        num[i] = decibelMeter(microphonePin);
    }

    int n, i;
    float sum = 0.0, avg;

    n = sizeof(num) / sizeof(num[0]);
    for (i = 0; i < n; i++)
        sum += num[i];
    avg = sum / n;
    data->leq = avg;
}

int MicrophoneController::decibelMeter(int microphonePin)
{
    unsigned long startMillis = millis(); // Start of sample window
    unsigned int peakToPeak = 0;          // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 4095; // 1024 for ESP8266, 4095 for ESP32 (12bit)

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(microphonePin);
        if (sample < 4095) // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample; // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample; // save just the min levels
            }
        }
    }
    peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
                                        // Serial.println(peakToPeak);

    // Calibrate MIC - with some other mobile application/meter check dB compared to peakToPeak
    // kalibrace viz. https://circuitdigest.com/microcontroller-projects/arduino-sound-level-measurement
    // for example : 33dB -> 277, 39dB -> 279 etc.

    // 1. way to compute dB - regression https://www.socscistatistics.com/tests/regression/default.aspx
    // compute ŷ a give to equation
    // dB = (peakToPeak + 1376.82535) / 37.0636;

    // 2. way - take reference point 55dB => 326, then
    // dB = 20 * log((double)peakToPeak / (double)326) + 55;

    // 3. way - create function witch aproximate curve https://mycurvefit.com/
    // it seems that same equation for all mics is good for now..
    return 66.36645 + (-80867760 - 66.36645)/(1 + pow((peakToPeak/10.88287), 8.387665));
}

void MicrophoneController::spectrumAnalyzer(MicrophoneData *data, int microphonePin)
{
    for (int i = 0; i < SAMPLES; i++)
    {
        newTime = micros() - oldTime;
        oldTime = newTime;
        vReal[i] = analogRead(microphonePin); // A conversion takes about 1uS on an ESP32
        vImag[i] = 0;
        while (micros() < (newTime + sampling_period_us))
        { /* do nothing to wait */
        }
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    for (int i = 2; i < (SAMPLES / 2); i++)
    { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
        if (vReal[i] > 2000)
        { // Add a crude noise filter, 10 x amplitude or more
            int dsize = (int)vReal[i] / amplitude;

            if (i <= 2)
            {
                displayBand(0, dsize); // 125Hz
                data->Hz125 = dsize;
            }

            if (i > 3 && i <= 5)
            {
                displayBand(1, dsize); // 250Hz
                data->Hz250 = dsize;
            }

            if (i > 5 && i <= 7)
            {
                displayBand(2, dsize); // 500Hz
                data->Hz500 = dsize;
            }
            if (i > 7 && i <= 15)
            {
                displayBand(3, dsize); // 1000Hz
                data->Hz1000 = dsize;
            }
            if (i > 15 && i <= 30)
            {
                displayBand(4, dsize); // 2000Hz
                data->Hz2000 = dsize;
            }
            if (i > 30 && i <= 53)
            {
                displayBand(5, dsize); // 4000Hz
                data->Hz4000 = dsize;
            }
            if (i > 53 && i <= 200)
            {
                displayBand(6, dsize); // 8000Hz
                data->Hz8000 = dsize;
            }
            if (i > 200)
            {
                displayBand(7, dsize); // 16000Hz
                data->Hz16000 = dsize;
            }
        }
    }
    if (millis() % 4 == 0)
    {
        for (byte band = 0; band <= 6; band++)
        {
            if (peak[band] > 0)
                peak[band] -= 1;
        }
    }
}

void MicrophoneController::displayBand(int band, int dsize)
{
    Serial.print(band);
    Serial.print("-");
    Serial.println(dsize);

    int dmax = 50;
    if (dsize > dmax)
        dsize = dmax;
    if (dsize > peak[band])
    {
        peak[band] = dsize;
    }
}
