#ifndef __led_matrix__
#define __led_martix__

#include <util/delay.h>


/***************************************

Configure Connections

****************************************/

#define HC595_PORT   PORTB
#define HC595_DDR    DDRB

#define HC595_DS_POS PB0      //Data pin (DS) pin location
#define HC595_DS_POS2 PB4      //Data pin (DS) pin location

#define HC595_SH_CP_POS PB1      //Shift Clock (SH_CP) pin location 
#define HC595_ST_CP_POS PB2      //Store Clock (ST_CP) pin location

/***************************************
Configure Connections ***ENDS***
****************************************/

//Initialize HC595 System

void HC595Init()
{
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));
   HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS2));
}


//Low level macros to change data (DS)lines
#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))

#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))


#define HC595DataHigh2() (HC595_PORT|=(1<<HC595_DS_POS2))

#define HC595DataLow2() (HC595_PORT&=(~(1<<HC595_DS_POS2)))

//Sends a clock pulse on SH_CP line
void HC595Pulse()
{
   //Pulse the Shift Clock

   HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH

   HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW

}

//Sends a clock pulse on ST_CP line
void HC595Latch()
{
   //Pulse the Store Clock

   HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
   _delay_loop_2(10);

   HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
   _delay_loop_2(10);
}


/*

Main High level function to write a single byte to
Output shift register 74HC595. 

Arguments:
   single byte to write to the 74HC595 IC

Returns:
   NONE

Description:
   The byte is serially transfered to 74HC595
   and then latched. The byte is then available on
   output line Q0 to Q7 of the HC595 IC.

*/
void HC595Write(uint8_t data)
{
	//Send each 8 bits serially

	//Order is MSB first
	for(uint8_t i=0;i<8;i++)
	{
		//Output the data on DS line according to the
		//Value of MSB
		if(data & 0b10000000)
		{
			//MSB is 1 so output high

			HC595DataHigh();
		}
		else
		{
			//MSB is 0 so output high
			HC595DataLow();
		}

		HC595Pulse();  //Pulse the Clock line
		data=data<<1;  //Now bring next bit at MSB position

	}

	//Now all 8 bits have been transferred to shift register
	//Move them to output latch at one
	HC595Latch();
}

void HC595Write2(uint8_t data, uint8_t data2)
{
   //Send each 8 bits serially

   //Order is MSB first
   for(uint8_t i=0;i<8;i++)
   {
      //Output the data on DS line according to the
      //Value of MSB
      if(data & 0b10000000)
      {
         //MSB is 1 so output high

         HC595DataHigh();
      }
      else
      {
         //MSB is 0 so output high
         HC595DataLow();
      }
	  
	  
	  if(data2 & 0b10000000)
	  {
		  //MSB is 1 so output high

		  HC595DataHigh2();
	  }
	  else
	  {
		  //MSB is 0 so output high
		  HC595DataLow2();
	  }

      HC595Pulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position
      data2=data2<<1;

   }

   //Now all 8 bits have been transferred to shift register
   //Move them to output latch at one
   HC595Latch();
}
/*

Simple Delay function approx 0.5 seconds

*/

void Wait()
{
   for(uint8_t i=0;i<2;i++)
   {
      _delay_loop_2(0);
   }
}
#endif

