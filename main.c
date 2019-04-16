#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arena.h>
#include "ch.h"
#include "hal.h"
#include "leds.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <chprintf.h>
#include "motors_advanced.h"
#include <i2c_bus.h>
#include <audio/microphone.h>
#include <communications.h>
#include <arm_math.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    //starts the USB communication
    usb_start();
    //inits the arena
    init_arena();
    set_rgb_led(0, 255, 0, 0);
	set_rgb_led(1, 255, 0, 0);
	set_rgb_led(2, 255, 0, 0);
	set_rgb_led(3, 255, 0, 0);

    messagebus_init(&bus,&bus_lock,&bus_condvar);
    goforward(true, 0);

    gotoarenacenter();
    //searchwaste();


    /* Infinite loop. */
    while (1) {

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}


