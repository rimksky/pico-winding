/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * Pocket watch winding machine using Step Motor "28BYJ-48 (5V)" + "ULN2003" Motor Driver
 */

#define LED_PIN PICO_DEFAULT_LED_PIN
#define IN1_PIN 10
#define IN2_PIN 11
#define IN3_PIN 12
#define IN4_PIN 13

// PINS
int pin_no[4] = { IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN };

// phases
int one_phase[4][4] = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};

int two_phase[4][4] = {
    { 1, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 1 },
    { 1, 0, 0, 1 }
};

int half_step[4][8] = {
    { 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 0, 0, 1, 1 }
};

int stepcount = 0;

// for one_phase / two_phase
int MAXSTEP = 4;
int ROUNDSTEP = 2048;

// for half_step
//int MAXSTEP = 8;
//int ROUNDSTEP = 1024;

int stop() {
    gpio_put( pin_no[0], 0 );
    gpio_put( pin_no[1], 0 );
    gpio_put( pin_no[2], 0 );
    gpio_put( pin_no[3], 0 );
}

int clockwise( int count, int waitms ) {
    for( int j=0; j<count; j++){
        stepcount++;
        stepcount %= MAXSTEP;
        gpio_put( pin_no[0], two_phase[stepcount][0] );
        gpio_put( pin_no[1], two_phase[stepcount][1] );
        gpio_put( pin_no[2], two_phase[stepcount][2] );
        gpio_put( pin_no[3], two_phase[stepcount][3] );
        if( waitms <= 2 ){
            sleep_ms( 2 );
            gpio_put( LED_PIN, 0 );
            stop();
        }
        else if( waitms <= 10 ){
            sleep_ms( waitms );
            gpio_put( LED_PIN, 0 );
            stop();        }
        else{
            sleep_ms( 10 );
            stop();
            gpio_put( LED_PIN, 0 );
            sleep_ms( waitms-10 );
        }
    }
}

int counterclockwise( int count, int waitms ) {
    for( int j=0; j<count; j++){
        stepcount--;
        if( stepcount < 0 ){
            while( stepcount < 0 ){
                stepcount += MAXSTEP
            ;
            }
        }
        stepcount %= MAXSTEP;
        gpio_put( LED_PIN, 1 );
        gpio_put( pin_no[0], two_phase[stepcount][0] );
        gpio_put( pin_no[1], two_phase[stepcount][1] );
        gpio_put( pin_no[2], two_phase[stepcount][2] );
        gpio_put( pin_no[3], two_phase[stepcount][3] );
        if( waitms <= 2 ){
            sleep_ms( 2 );
            gpio_put( LED_PIN, 0 );
            stop();
        }
        else if( waitms <= 10 ){
            sleep_ms( waitms );
            gpio_put( LED_PIN, 0 );
            stop();
        }
        else{
            sleep_ms( 10 );
            gpio_put( LED_PIN, 0 );
            stop();
            sleep_ms( waitms-10 );
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    for( int i=0; i<4; i++ ){
        gpio_init(pin_no[i]);
        gpio_set_dir(pin_no[i], GPIO_OUT);
    }

    while (true) {
        counterclockwise(ROUNDSTEP*20, 2);
        sleep_ms(3600*40*1000); // 20times winding by 40hours
    }

    return 0;
}
