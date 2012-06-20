AVR-Alarm: Asynchronous time-triggered actions for AVRs
=======================================================
The avr-alarm library allows its users to **schedule** the handling of events happening at arbitrary moments
during program execution. Let's just start with a quick and dirty example:

```c
    #include <stdlib.h>
    #include <stdint.h>
    #include <avr/interrupt.h>
    #include <avr-alarm/alarm.h>

    void handler_portb(void* n_uint8) { PORTB = *((uint8_t*) n_uint8); }

    void main(void) {
        alarm_init(1);
        alarm_timer_start();

        // do some stuff

        uint8_t ns[2] = { 23, 42 };
        alarm_insert(2000, handler_portb, &ns[0]);
        alarm_insert(4000, handler_portb, &ns[1]);

        while(1);
    }
```

The code above defines a function named `handler_portb`, which sole task is to write a (variable) number to
PORTB. Then, in the body of main, after some boring initialization and doing some other stuff, we:

 1. Schedule a call to `handler_portb` to happen 2 s later than the call to `alarm_insert` itself. The argument
    to be passed to `handler_portb` when the event is triggered is the *address* (pointer) of ns\[0\].
 2. Schedule another call to `handler_portb`, this time happening 4 s after the scheduling itself and giving as
    argument a pointer to ns\[1\].

Notice the type of the function `handler_portb`: It's not a coincidence. It's the type that any handler
function must have in order to be used with avr-alarm. This is explained in more details in the API section.


How it works - just the amount you need to know to be able to use it
--------------------------------------------------------------------
The fundamental principle of avr-alarm is pretty simple: We use a periodic event with a known frequency to
**count time**, and then - at each time unit - we see **if there's any event scheduled** to happen at that
moment. The counting of time in avr-alarm uses the **compare match interrupt of TIMER1**. Therefore, when
using avr-alarm in your application, TIMER1 **should not** be used for anything else.

AVR-Alarm actually does its job repeatedly at every **n** miliseconds - that is its *resolution* - meaning that you
cannot schedule an event to happen at a moment with a precision less than **n** miliseconds. The job of avr-alarm
is to manage a **queue** of events that are scheduled to happen, and at each **n** miliseconds check if the time has
come for the event in front of the queue to finally happen...

AVR-Alarm uses a *relative ordered queue* - ordered by the *remaining time until event execution*. You can
google around for what a relative queue is, it's quite interesting :)  Anyways, what this MEANS is that
whenever the time value attached to an event reaches ZERO, it's time for the event to get executed.

At this time, the handler function pointer given at the time when `alarm_insert` was called is then called,
and the handler function is passed a void pointer, which was the third argument in the call to `alarm_insert`.
This passing of the pointer to the handler function is the technique avr-alarm uses to allow generic parameter
passing to the event handlers...


Public API
----------
These are the types and functions one must know in order to use avr-alarm correctly:

### type: time\_ms\_t
This is the type to be used whenever a time argument is needed when dealing with avr-alarm's functions. This
is just a typedef for an unsigned integer, but the name of the type itself reminds the programmer that values
with this type shall be time **in miliseconds**. Here's the definition of `time_ms_t`:
```c
    typedef uint16_t time_ms_t;
```

### type: handler\_t
The type of an event handler function. An event handler function is a function that receives **one** void
pointer as argument and has return type *void*. Here's the definition of `handler_t`:
```c
    typedef void (*handler_t)(void*);
```

### function: void alarm\_init(time_ms_t period)
Initializes the data structures used by avr-alarm to handle event scheduling and time counting. It creates and
initializes the queue used to store scheduled events and it configures the timer used by the alarm
implementation (TIMER1). Sets the resolution of the timer to *period* miliseconds.

 1. **Important:** This function must be called before any call to any other function of avr-alarm.
 2. **Important:** This function configures the timer, but **does not** start it. Therefore, the client has
    to call alarm\_timer\_start once in the beginning of the program to start time counting.


### function: void alarm\_timer\_start(void)
Starts the underlying timer, that from now on will fire once every **n** miliseconds. Only at this moment does avr-alarm
consider that time is "moving". If any event has been scheduled before a call to alarm\_timer\_start(), then
*t0* (the initial time instant) is defined as being the instant when alarm\_timer\_start was called.


### function: void alarm\_insert(time\_ms\_t timeout, handler\_t handler, void\* arg\_ptr)
Schedules a new event to be triggered *timeout* ms after the call to `alarm_insert` has finished - or after
the call to `alarm_timer_start` (in case it still has not been called).

After *timeout* ms have elapsed, the function pointer handler will be dereferenced and called, with arg\_ptr
being passed to it as an argument.


### function: time\_ms\_t alarm\_until\_last\_deadline(void)
This function returns the amount of time that has to elapse until the last deadline among all events currently
in the queue is reached - in miliseconds. The time returned corresponds to *t_last - t_0*, where:

 * t\_last is the instant when the deadline of the last scheduled event will be reached.
 * t\_0 is the instant when `alarm_until_last_deadline` itself was called, OR the instant when
   `alarm_timer_start` will be called (in case it hasn't been called yet).


Limitations and other remarks
-----------------------------
 1. **Be careful when using the void pointer to pass information to the handler:** The way we have chosen to
    allow generic parameter passing to the handler functions was to make a handler function always get
    a void\* as argument when called. This allow you to pass-by-reference a value of any type to the
    handlers... In the body of the handler - however - you are advised to convert **as soon as possible**
    the received void\* into a pointer of the intended argument type and **never again in the body of
    the handler** use the raw void\*.

    When passing an argument to a handler function, there's an implicit contract that must be obeyed:

    > The type of the ACTUAL pointer parameter passed at the call site of alarm\_insert must match the
    > target of the cast which happens in the first line of the handler's body.

 2. **Using excessively long handler functions might lead to memory corruption and deadlocks:**
    This has to do with the way in which avr-alarm deals with its strict timing constraints: when an event
    is triggered (just as its handler function is about to be called) *interrupts are re-enabled*.

    Then, in case a handler function is unable to return *before the next event is triggered*, the contexts
    of *several handlers can accumulate on the stack*. If handlers start accumulating without ever
    finishing, the system will become unresponsive until a stack overflow occurs and corrupts the whole
    memory. This will then **make the AVR CPU go berserk**.

    Therefore, **the recommended® length of an event handler is not above 3000 instructions**.

 3. **Using non-reentrant handler functions might cause race conditions:** You should manipulate global
    variables with great care in your handler functions. An event handler can have its execution
    interrupted and intermixed with other handlers, therefore, having handlers manipulate global
    variables is always **unsafe**.


Installing and using (including and linking)
--------------------------------------------
You can compile and install avr-alarm at a certain PREFIX in your filesystem by just typing:

    make install PREFIX=[prefix]

To use avr-alarm in an application (or other library), you must add the path of avr-alarm's header
($PREFIX/include) to the compiler's include path, and link your application with the static lib file
(libavr-alarm.a) found under $PREFIX/lib. In the case that you are using a Makefile based on the template
at [avr-utils](http://github.com/joaopizani/avr-utils), you can just add the following to your paths.def:
```bash
    AVRALARM_ROOT=[PREFIX where you installed avr-alarm]
    AVRALARM_INCLUDE=${AVRALARM_ROOT}/include
    AVRALARM_LIBS=${AVRALARM_ROOT}/lib/avr-alarm
    AVRALARM_INCFLAGS=-I${AVRALARM_INCLUDE}
    AVRALARM_LIBFLAGS=-Wl,--whole-archive -L${AVRALARM_LIBS} -lavr-alarm -Wl,--no-whole-archive
```

Observation: the *-Wl,--whole-archive* and *-Wl,--no-whole-archive* options are needed because the library
includes interrupt handlers, which without these options wouldn't be linked in. Finally, you should
append AVRALARM\_INCFLAGS and AVRALARM\_LIBFLAGS, respectively, to the variables EXT\_INCFLAGS and EXT\_LIBFLAGS,
which should be present at the end of your paths.def. Like this:
```bash
    EXT_INCFLAGS=${LIB1_INCFLAGS} ${LIB2_INCFLAGS} ${AVRALARM_INCFLAGS} ...
    EXT_LIBFLAGS=${LIB1_LIBFLAGS} ${LIB2_LIBFLAGS} ${AVRALARM_LIBFLAGS} ...
```
