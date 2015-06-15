/* 
	File : core_portme.c
*/
/*
	Author : Shay Gal-On, EEMBC
	Legal : TODO!
*/
#include "coremark.h"
#include "core_portme.h"
#include "spr-defs.h"
#include "board.h"
#include "support.h"
#include "common.h"

/*
#if VALIDATION_RUN
volatile ee_s32 seed1_volatile = 0x3415;
volatile ee_s32 seed2_volatile = 0x3415;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PERFORMANCE_RUN
volatile ee_s32 seed1_volatile = 0x0;
volatile ee_s32 seed2_volatile = 0x0;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PROFILE_RUN
volatile ee_s32 seed1_volatile = 0x8;
volatile ee_s32 seed2_volatile = 0x8;
volatile ee_s32 seed3_volatile = 0x8;
#endif
volatile ee_s32 seed4_volatile = ITERATIONS;
volatile ee_s32 seed5_volatile = 0;
*/

volatile ee_s32 seed1_volatile;
volatile ee_s32 seed2_volatile;
volatile ee_s32 seed3_volatile;
volatile ee_s32 seed4_volatile = 0;	// Iterations defaults to 0
volatile ee_s32 seed5_volatile = 0;

/* Porting : Timing functions
   How to capture time and convert to seconds must be ported to whatever is 
   supported by the platform. e.g. Read value from on board RTC, read value 
   from cpu clock cycles performance counter etc. Sample implementation for 
   standard time.h and windows.h definitions included.
*/
CORETIMETYPE barebones_clock()
{
	//#error "You must implement a method to measure time in 
	//barebones_clock()! This function should return current time.\n"
	return timestamp;
}

/* Define : TIMER_RES_DIVIDER
   Divider to trade off timer resolution and total time that can be measured.

   Use lower values to increase resolution, but make sure that overflow does 
   not occur. If there are issues with the return value overflowing, increase 
   this value.
*/
#define GETMYTIME(_t) (*_t=barebones_clock())
#define MYTIMEDIFF(fin,ini) ((fin)-(ini))
#define TIMER_RES_DIVIDER 1	/* Unused */
#define SAMPLE_TIME_IMPLEMENTATION 1
#define EE_TICKS_PER_SEC TICKS_PER_SEC

/** Define Host specific (POSIX), or target specific global time variables. */
static CORETIMETYPE start_time_val, stop_time_val;

/* Function : start_time
   This function will be called right before starting the timed portion of the 
   benchmark.

   Implementation may be capturing a system timer (as implemented in the 
   example code) or zeroing some system parameters - e.g. setting the cpu 
   clocks cycles to 0.
*/
void start_time(void)
{
	GETMYTIME(&start_time_val);
}

/* Function : stop_time
   This function will be called right after ending the timed portion of the 
   benchmark.
   
   Implementation may be capturing a system timer (as implemented in the 
   example code) or other system parameters - e.g. reading the current value of
   cpu cycles counter.
*/
void stop_time(void)
{
	GETMYTIME(&stop_time_val);
}

/* Function : get_time
   Return an abstract "ticks" number that signifies time on the system.
   
   Actual value returned may be cpu cycles, milliseconds or any other value,
   as long as it can be converted to seconds by <time_in_secs>.
   This methodology is taken to accomodate any hardware or simulated platform.
   The sample implementation returns millisecs by default, 
   and the resolution is controlled by <TIMER_RES_DIVIDER>
*/
CORE_TICKS get_time(void)
{
	CORE_TICKS elapsed =
	    (CORE_TICKS) (MYTIMEDIFF(stop_time_val, start_time_val));
	return elapsed;
}

/* Function : time_in_secs
   Convert the value returned by get_time to seconds.
   
   The <secs_ret> type is used to accomodate systems with no support for 
   floating point. Default implementation implemented by the EE_TICKS_PER_SEC 
   macro above.
*/
secs_ret time_in_secs(CORE_TICKS ticks)
{
	secs_ret retval = ((secs_ret) ticks) / (secs_ret) EE_TICKS_PER_SEC;
	return retval;
}

ee_u32 default_num_contexts = 1;

/* Function : portable_init
   Target specific initialization code 
   Test for some common mistakes.
*/
void portable_init(core_portable * p, int *argc, char *argv[])
{

	// In ORPmon - UART already initialised: uart_init(DEFAULT_UART);
	if (sizeof(ee_ptr_int) != sizeof(ee_u8 *)) {
		ee_printf
		    ("ERROR! Please define ee_ptr_int to a type that holds a pointer!\n");
		while (1) ;	// FINISH
	}
	if (sizeof(ee_u32) != 4) {
		ee_printf
		    ("ERROR! Please define ee_u32 to a 32b unsigned type!\n");
		while (1) ;	// FINISH
	}
	// In ORPmon - things already initialised
	// Clear timer variable
	//clear_timer_ticks();
	// init timer
	//enable_timer();

	//

	// Parse argv and figure out what kind of run we do
	if (argc > 0) {
		switch ((char)argv[0][0]) {
		case 'p':
			//PERFORMANCE_RUN
			seed1_volatile = 0x0;
			seed2_volatile = 0x0;
			seed3_volatile = 0x66;
			break;
		case 'o':
			//PROFILE_RUN
			seed1_volatile = 0x8;
			seed2_volatile = 0x8;
			seed3_volatile = 0x8;
			break;
		default:
			//VALIDATION_RUN
			seed1_volatile = 0x3415;
			seed2_volatile = 0x3415;
			seed3_volatile = 0x66;
			break;
		}
	}

	if (argc > 1)
		seed4_volatile = strtoul(argv[1], 0, 0);	// Iterations as second argument
	else
		seed4_volatile = 0;	// Zero (test runs a minimum amount of time)

	seed5_volatile = 0;

	p->portable_id = 1;

}

/* Function : portable_fini
	Target specific final code 
*/
void portable_fini(core_portable * p)
{
	p->portable_id = 0;
}