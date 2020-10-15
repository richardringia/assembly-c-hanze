#include "AVR_TTC_scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16E6

#define HIGH 0x1
#define LOW  0x0

const uint8_t data = 0;
const uint8_t clock = 1;
const uint8_t strobe = 2;

uint8_t readButtons(void);

// The array of tasks
sTask SCH_tasks_G[SCH_MAX_TASKS];

// 7 Segment encoded digits from 0 to 9
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

// Current digit
volatile uint32_t currentDigit = 0;

volatile unsigned char updateDisplayTaskId;


// read value from pin
int read(uint8_t pin)
{
	if (PINB & _BV(pin)) { // if pin set in port
		return HIGH;
		} else {
		return LOW;
	}
}

/*------------------------------------------------------------------*-

  SCH_Dispatch_Tasks()

  This is the 'dispatcher' function.  When a task (function)
  is due to run, SCH_Dispatch_Tasks() will run it.
  This function must be called (repeatedly) from the main loop.

-*------------------------------------------------------------------*/

void SCH_Dispatch_Tasks(void)
{
   unsigned char Index;

   // Dispatches (runs) the next task (if one is ready)
   for(Index = 0; Index < SCH_MAX_TASKS; Index++)
   {
      if((SCH_tasks_G[Index].RunMe > 0) && (SCH_tasks_G[Index].pTask != 0))
      {
         (*SCH_tasks_G[Index].pTask)();  // Run the task
         SCH_tasks_G[Index].RunMe -= 1;   // Reset / reduce RunMe flag

         // Periodic tasks will automatically run again
         // - if this is a 'one shot' task, remove it from the array
         if(SCH_tasks_G[Index].Period == 0)
         {
            SCH_Delete_Task(Index);
         }
      }
   }
}

/*------------------------------------------------------------------*-

  SCH_Add_Task()

  Causes a task (function) to be executed at regular intervals 
  or after a user-defined delay

  pFunction - The name of the function which is to be scheduled.
              NOTE: All scheduled functions must be 'void, void' -
              that is, they must take no parameters, and have 
              a void return type. 
                   
  DELAY     - The interval (TICKS) before the task is first executed

  PERIOD    - If 'PERIOD' is 0, the function is only called once,
              at the time determined by 'DELAY'.  If PERIOD is non-zero,
              then the function is called repeatedly at an interval
              determined by the value of PERIOD (see below for examples
              which should help clarify this).


  RETURN VALUE:  

  Returns the position in the task array at which the task has been 
  added.  If the return value is SCH_MAX_TASKS then the task could 
  not be added to the array (there was insufficient space).  If the
  return value is < SCH_MAX_TASKS, then the task was added 
  successfully.  

  Note: this return value may be required, if a task is
  to be subsequently deleted - see SCH_Delete_Task().

  EXAMPLES:

  Task_ID = SCH_Add_Task(Do_X,1000,0);
  Causes the function Do_X() to be executed once after 1000 sch ticks.            

  Task_ID = SCH_Add_Task(Do_X,0,1000);
  Causes the function Do_X() to be executed regularly, every 1000 sch ticks.            

  Task_ID = SCH_Add_Task(Do_X,300,1000);
  Causes the function Do_X() to be executed regularly, every 1000 ticks.
  Task will be first executed at T = 300 ticks, then 1300, 2300, etc.            
 
-*------------------------------------------------------------------*/

unsigned char SCH_Add_Task(void (*pFunction)(), const unsigned int DELAY, const unsigned int PERIOD)
{
   unsigned char Index = 0;

   // First find a gap in the array (if there is one)
   while((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
   {
      Index++;
   }

   // Have we reached the end of the list?   
   if(Index == SCH_MAX_TASKS)
   {
      // Task list is full, return an error code
      return SCH_MAX_TASKS;  
   }

   // If we're here, there is a space in the task array
   SCH_tasks_G[Index].pTask = pFunction;
   SCH_tasks_G[Index].Delay =DELAY;
   SCH_tasks_G[Index].Period = PERIOD;
   SCH_tasks_G[Index].RunMe = 0;

   // return position of task (to allow later deletion)
   return Index;
}

/*------------------------------------------------------------------*-

  SCH_Delete_Task()

  Removes a task from the scheduler.  Note that this does
  *not* delete the associated function from memory: 
  it simply means that it is no longer called by the scheduler. 
 
  TASK_INDEX - The task index.  Provided by SCH_Add_Task(). 

  RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL

-*------------------------------------------------------------------*/

unsigned char SCH_Delete_Task(const unsigned char TASK_INDEX)
{
   // Return_code can be used for error reporting, NOT USED HERE THOUGH!
   unsigned char Return_code = 0;

   SCH_tasks_G[TASK_INDEX].pTask = 0;
   SCH_tasks_G[TASK_INDEX].Delay = 0;
   SCH_tasks_G[TASK_INDEX].Period = 0;
   SCH_tasks_G[TASK_INDEX].RunMe = 0;

   return Return_code;
}

/*------------------------------------------------------------------*-

  SCH_Init_T1()

  Scheduler initialisation function.  Prepares scheduler
  data structures and sets up timer interrupts at required rate.
  You must call this function before using the scheduler.  

-*------------------------------------------------------------------*/

void SCH_Init_T1(void)
{
   unsigned char i;

   for(i = 0; i < SCH_MAX_TASKS; i++)
   {
      SCH_Delete_Task(i);
   }

   // Set up Timer 1
   // Values for 1ms and 10ms ticks are provided for various crystals

   // Hier moet de timer periode worden aangepast ....!
   OCR1A = (uint16_t)625;   		     // 10ms = (256/16.000.000) * 625
   TCCR1B = (1 << CS12) | (1 << WGM12);  // prescale op 64, top counter = value OCR1A (CTC mode)
   TIMSK1 = 1 << OCIE1A;   		     // Timer 1 Output Compare A Match Interrupt Enable
}

/*------------------------------------------------------------------*-

  SCH_Start()

  Starts the scheduler, by enabling interrupts.

  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronised.

  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! 
 
-*------------------------------------------------------------------*/

void SCH_Start(void)
{
      sei();
}

/*------------------------------------------------------------------*-

  SCH_Update

  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in SCH_Init_T1().

-*------------------------------------------------------------------*/

ISR(TIMER1_COMPA_vect)
{
   unsigned char Index;
   for(Index = 0; Index < SCH_MAX_TASKS; Index++)
   {
      // Check if there is a task at this location
      if(SCH_tasks_G[Index].pTask)
      {
         if(SCH_tasks_G[Index].Delay == 0)
         {
            // The task is due to run, Inc. the 'RunMe' flag
            SCH_tasks_G[Index].RunMe += 1;

            if(SCH_tasks_G[Index].Period)
            {
               // Schedule periodic tasks to run again
               SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
               SCH_tasks_G[Index].Delay -= 1;
            }
         }
         else
         {
            // Not yet ready to run: just decrement the delay
            SCH_tasks_G[Index].Delay -= 1;
         }
      }
   }
}

// ------------------------------------------------------------------

// write value to pin
void write(uint8_t pin, uint8_t val)
{
	if (val == LOW) {
		PORTB &= ~(_BV(pin)); // clear bit
		} else {
		PORTB |= _BV(pin); // set bit
	}
}

// shift out value to data
void shiftOut (uint8_t val)
{
	uint8_t i;
	for (i = 0; i < 8; i++)  {
		write(clock, LOW);   // bit valid on rising edge
		write(data, val & 1 ? HIGH : LOW); // lsb first
		val = val >> 1;
		write(clock, HIGH);
	}
}

// shift in value from data
uint8_t shiftIn(void)
{
	uint8_t value = 0;
	uint8_t i;

	DDRB &= ~(_BV(data)); // clear bit, direction = input
	
	for (i = 0; i < 8; ++i) {
		write(clock, LOW);   // bit valid on rising edge
		value = value | (read(data) << i); // lsb first
		write(clock, HIGH);
	}
	
	DDRB |= _BV(data); // set bit, direction = output
	
	return value;
}

void sendCommand(uint8_t value)
{
	write(strobe, LOW);
	shiftOut(value);
	write(strobe, HIGH);
}

// Display a digit
void updateDisplay()
{
	uint32_t digit = currentDigit;

	// Split whole digit into single digits [0 to 9]
	uint8_t digitArray[8];
	for(uint8_t i = 0; i < 8; i++) {
		digitArray[i] = digit % 10;    // Save current single digit
		
		digit /= 10;				       // Calculate next digit
	}
	
	// Command: set auto increment mode
	sendCommand(0x40);
	
	// Command arguments
	write(strobe, LOW);
	
	// Starting at address 0
	shiftOut(0xc0);
	
	for(uint8_t i = 0; i < 8; i++) {
		// Display digit
		shiftOut(digits[digitArray[7-i]]);
		
		// Set LED
		shiftOut(0x00);
	}
	
	write(strobe, HIGH);

	currentDigit++;
}

void checkButtons()
{
	uint8_t buttons = readButtons();

	uint8_t swA = 0;
	uint8_t swB = 1;

	uint8_t mskSwA = 1 << swA;
	uint8_t mskSwB = 1 << swB;

	if(buttons & mskSwA) {
		currentDigit = 0;
		updateDisplayTaskId = SCH_Add_Task(updateDisplay,0,1);
	}

	if(buttons & mskSwB) {
		currentDigit = 0;
		SCH_Delete_Task(updateDisplayTaskId);
	}
}

uint8_t readButtons(void)
{
	uint8_t buttons = 0;
	write(strobe, LOW);
	shiftOut(0x42); // key scan (read buttons)

	DDRB &= ~(_BV(data)); // clear bit, direction = input

	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t v = shiftIn() << i;
		buttons |= v;
	}

	DDRB |= _BV(data); // set bit, direction = output
	write(strobe, HIGH);
	return buttons;
}

int main()
{
	DDRB=0xff; // set port B as output

	sendCommand(0x89);  // activate and set brightness to medium
	
	SCH_Init_T1();
	
	SCH_Add_Task(checkButtons, 0, 1);
	
	SCH_Start(); // start the scheduler

	// Loop
	while (1) {
		SCH_Dispatch_Tasks();
	}
	
	return 0;
}
