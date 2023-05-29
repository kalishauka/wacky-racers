/* File:   ledtape_test2.c
   Author: B Mitchell, UCECE
   Date:   14 April 2021
   Descr:  Test ledtape
*/

#include <pio.h>
#include <stdlib.h>
#include "target.h"
#include "pacer.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"
#include "ledtape.h"

/*
    This is an alternative method for driving the LED tape using the ledbuffer
    module that is included in the ledtape driver.

    The buffer acts like a small framebuffer upon which you can set RGB values
    at specific positions (not GRB, it handles the translation automatically).
    It also makes it easy to make patterns, shuffle them allow the strip, and
    clear it later. See ledbuffer.h for more details (CTRL-Click it in VS Code).

    Troubleshooting: check the frequency of the generated waveform; if it is not
    close to 800 kHz, adjust the value of LEDTAPE_TPERIOD by defining it in
    target.h. (See src/mmculib/ledtape/ledtape.h for more info.)
*/

void set_pattern(int *count, ledbuffer_t *leds, bool *blue)
{
    pacer_wait();

    if (*count++ == NUM_LEDS)
    {
        // wait for a revolution
        ledbuffer_clear(leds);
        if (*blue)
        {
            ledbuffer_set(leds, 0, 0, 0, 255);
            ledbuffer_set(leds, NUM_LEDS / 2, 0, 0, 255);
        }
        else
        {
            ledbuffer_set(leds, 0, 255, 0, 0);
            ledbuffer_set(leds, NUM_LEDS / 2, 255, 0, 0);
        }
        *blue = !blue;
        *count = 0;
    }

    ledbuffer_write(leds);
    ledbuffer_advance(leds, 1);
}

void set_pattern_simple(uint8_t *leds_seq)
{

    for (int i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds_seq[i * 3] = rand() % (255 + 1 - 0) + 0;
        leds_seq[i * 3 + 1] = rand() % (255 + 1 - 0) + 0;
        leds_seq[i * 3 + 2] = rand() % (255 + 1 - 0) + 0;
    }

    ledtape_write(LEDTAPE_PIO, leds_seq, NUM_LEDS * 3);
}
