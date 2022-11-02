#include <cmath>
#include <cstdint>
#include <cstdio>

#include "mbed.h"

const double pi = 3.141592653589793238462;
const double amplitude = 0.5f;
const double offset = 65535 / 2.0;
Thread waveform_thread;
Thread t;
EventQueue queue;
float data[120];

// The sinewave is created on this pin
// Adjust analog output pin name to your board spec.
AnalogOut aout(PA_4);

double rads = 0.0;
float sample = 0;
float sample1 = 0;
float sample2 = 0;
int finish = 0;

void PrintData() {
    for (int i = 0; i < 120; i++) printf("%f\r\n", data[i] / 65535.0);
}

void Sin() {
    while (1) {
        // sinewave output
        for (int i = 0; i < 360; i += 3) {
            rads = (pi * i) / 180.0f;
            sample1 = (uint16_t)(amplitude * (offset * (cos(2 * rads + pi))) +
                                 offset);
            sample2 =
                (uint16_t)(amplitude * (offset * (cos(rads + pi))) + offset);
            sample = (sample1 + sample2) / 2;
            data[i / 3] = sample;
            aout.write_u16(sample);
        }
        finish = 1;
        ThisThread::sleep_for(1ms);
    }
}

int main() {
    // double rads = 0.0;
    // float sample = 0;
    // float sample1 = 0;
    // float sample2 = 0;

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    waveform_thread.start(Sin);
    while (1) {
        if (finish == 1) {
            queue.call(PrintData);
        }
    }

    // while (1)
    // {
    //     // sinewave output
    //     for (int i = 0; i < 360; i++)
    //     {
    //         rads = (pi * i) / 180.0f;
    //         sample1 = (uint16_t)(amplitude * (offset * (cos(rads + pi))) +
    //         offset); sample2 = (uint16_t)(amplitude * (offset * (cos(rads +
    //         pi))) + offset); sample = (sample1 + sample2) / 2;
    //         aout.write_u16(sample1);
    //     }

    //     ThisThread::sleep_for(1ms);
    // }
}
