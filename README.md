AVR-Alarm: Generic asynchronous time-triggered actions for AVR MCUs
===================================================================
The avr-alarm library allows its users to **schedule** the handling of events happening at arbitrary moments
during the execution of the program. Let's just start with a quick and dirty example:

    #include <stdlib.h>
    #include <stdint.h>
    #include <avr/interrupt.h>
    #include <avr-alarm/alarm.h>

    void handler_portb(void* n_uint8) { PORTB = *((uint8_t*) n_uint8); }

    void main(void) {
        alarm_init();
        alarm_timer_start();
        
        // do some stuff
        
        uint8_t ns[2] = { 23, 42 };
        alarm_insert(2000, handler_portb, &ns[0]);
        alarm_insert(4000, handler_portb, &ns[1]);
        
        while(1);
    }

The code above defines a function named `handler_portb`, which sole task is to write a magical number to
PORTB. Then in the body of main, after boring initialization and doing some other stuff, we:

1. Schedule a call to `handler_portb` to happen 2s later than the call to `alarm_insert` itself. The argument
   to be passed to `handler_portb` when the event is triggered is the *address* (pointer) of ns[0].
2. Schedule another call to `handler_portb`, this time happening 4s after the scheduling itself and giving as
   argument a pointer to ns[1].

Notice the type of function `handler_portb`: It's not a coincidence. It's the type that any handler function
must have in order to be used with avr-alarm. This is explained in more details in the API section.

How it works - just the amount you need to know to be able to use it
--------------------------------------------------------------------
The fundamental principle of avr-alarm is pretty simple: We use a periodic event with a known frequency to
**count time**, and then - at each time unit - we see **if there's any event scheduled** to happen at that
moment. The counting of time in avr-alarm uses the **compare match interrupt of TIMER1**. Therefore, when
using avr-alarm in your application, TIMER1 **should not** be used for anything else.

AVR-Alarm actually does its job repeatedly at every milisecond - that is its *resolution* - meaning that you
cannot schedule an event to happen at a moment with a precision less than a milisecond. The job of avr-alarm
is to manage a **queue** of events that are scheduled to happen, and at each milisecond check if the time has
come for the event in front of the queue to finally happen...

AVR-Alarm uses a *relative ordered queue* - ordered by the *remaining time until event execution*. You can
google around for what a relative queue is, it's quite interesting :)  Anyways, what this MEANS is that
whenever the time value attached to an event reaches ZERO, it's time for the event to get executed.

At this time, the handler function pointer given at the time when `alarm_insert` was called is then called,
and the handler function is passed a void pointer, which was the third argument in the call to `alarm_insert`.
This passing of the pointer to the handler function is the technique avr-alarm uses to allow generic parameter
passing to the event handlers...
