/*
 * Accelerometer interface with PIC18F4550
 * http://www.electronicwings.com
 * 
 */

#include <pic18f4550.h>
#include "Configuration_header_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>									/* Include math header file */
#include "ADC_Header_File.h"
#include "USART_Header_File.h"

/*This count Provide delay of 1 ms for 8MHz Frequency */
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<=val;i++)
            for(j=0;j<165;j++);
}
void SendSerial(const char* str, double value, char unit)
{
	char buffer[10];
	sprintf(buffer,"%.2f",value);
	USART_SendString(str);	/* Send Name string */
	USART_SendString(buffer);/* Send value */
	USART_TxChar(unit);	/* Send unit char */
	USART_TxChar('\t');	/* Send tab char */
	MSdelay(10);
}

int main(void)
{
	int ADC_X_VALUE,ADC_Y_VALUE,ADC_Z_VALUE;
	double Axout,Ayout,Azout,theta, psy, phi,roll,pitch,yaw;
	OSCCON = 0x72;		/* Internal Osc. frequency 8 MHz */
	USART_Init(9600);	/* Initialize USART with 9600 Baud rate */
	ADC_Init();		/* Initialize ADC */

	while(1)
	{
		ADC_X_VALUE = ADC_Read(0);/* Read X, Y, Z axis ADC value */
		ADC_Y_VALUE = ADC_Read(1);
		ADC_Z_VALUE = ADC_Read(2);

		/* Convert values in g unit */
		Axout = (((double)(ADC_X_VALUE*5)/1.024)-1700.0)/330.0; 
		Ayout = (((double)(ADC_Y_VALUE*5)/1.024)-1700.0)/330.0;
		Azout = (((double)(ADC_Z_VALUE*5)/1.024)-1700.0)/330.0;

		/* Calculate angles */
		theta = atan(Axout/(sqrt((pow (Ayout,2.0))+(pow (Azout,2.0)))))*57.29577951; 
		psy =  atan(Ayout/(sqrt((pow (Axout,2.0))+(pow (Azout,2.0)))))*57.29577951;
		phi =  atan((sqrt((pow (Ayout,2.0))+(pow (Axout,2.0))))/Azout)*57.29577951;

		roll = (atan2(Ayout,Azout))*57.29577951+180;
		pitch =  (atan2(Azout,Axout))*57.29577951+180;
		yaw =  (atan2(Axout,Ayout))*57.29577951+180;
		
		SendSerial("Axout = ",Axout,'g');/* Send All value serially */
		SendSerial("Ayout = ",Ayout,'g');
		SendSerial("Azout = ",Azout,'g');

		SendSerial("Theta = ",theta,248);
		SendSerial("Psy = ",psy,248);
		SendSerial("Phi = ",phi,248);

		SendSerial("Roll = ",roll,248);
		SendSerial("Pitch = ",pitch,248);
		SendSerial("Yaw = ",yaw,248);
        
		USART_SendString("\r\n");
	}
}