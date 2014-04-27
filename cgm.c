#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "emax_es08a_servo.h"
#include "bbb_ubuntu.h"

void init (void);
int openFile(char * sFileName);


int main ()
{
	int fdP8_13_Duty, fdP9_14_Duty;
	char sAngle [10];
	int iAngle;
	unsigned char i;
	
	init ();

	printf("int size: %u\n", sizeof(iAngle));

	//fdP8_13_Duty = openFile(BBB_PWM_P8_13_DUTY);
	fdP8_13_Duty = open (BBB_PWM_P8_13_DUTY, O_RDWR);
	if (!fdP8_13_Duty) perror(BBB_PWM_P8_13_DUTY);
	
	//fdP9_14_Duty = openFile(BBB_PWM_P9_14_DUTY);
	fdP9_14_Duty = open (BBB_PWM_P9_14_DUTY, O_RDWR);
	if (!fdP9_14_Duty) perror(BBB_PWM_P9_14_DUTY);

	while (1)
	{
		puts ("Servo 1 (0-180): ");
		gets (sAngle);
		iAngle = atoi(sAngle);
		iAngle = (iAngle * 9000) + 480000;
		write (fdP8_13_Duty, sAngle, strlen(sAngle));
		puts ("Servo 2 (0-180): ");
		gets (sAngle);
		write (fdP9_14_Duty, sAngle, strlen(sAngle));
	}

	close (fdP8_13_Duty);
	close (fdP9_14_Duty);

	return 0;
}


void init (void)
{
	int fdSlotConfig;
	int fdP8_13_Polarity, fdP8_13_Period;
	int fdP9_14_Polarity, fdP9_14_Period;

	char * cPwmSetup = BBB_PWM_SETUP;
	char * cPwmP8_13 = BBB_PWM_P8_13;
	char * cPwmP9_14 = BBB_PWM_P9_14;

	printf("Initializing PWM ports...");

	fdSlotConfig = open (BBB_CONFIG_SLOTS,O_RDWR);
	if (!fdSlotConfig) perror(BBB_CONFIG_SLOTS);

	if (!(write (fdSlotConfig, cPwmSetup, strlen(cPwmSetup)))) perror(BBB_PWM_SETUP); 
	if (!(write (fdSlotConfig, cPwmP8_13, strlen(cPwmP8_13)))) perror(BBB_PWM_P8_13);
	if (!(write (fdSlotConfig, cPwmP9_14, strlen(cPwmP9_14)))) perror(BBB_PWM_P9_14);

	system("sleep 1");

	fdP8_13_Polarity = open (BBB_PWM_P8_13_POLARITY, O_RDWR);
	if (!fdP8_13_Polarity) perror(BBB_PWM_P8_13_POLARITY);

	fdP8_13_Period 	= open (BBB_PWM_P8_13_PERIOD, O_RDWR);
	if (!fdP8_13_Period) perror(BBB_PWM_P8_13_PERIOD);

	fdP9_14_Polarity = open (BBB_PWM_P9_14_POLARITY, O_RDWR);
	if (!fdP9_14_Polarity) perror(BBB_PWM_P9_14_POLARITY);

	fdP9_14_Period	= open (BBB_PWM_P9_14_PERIOD, O_RDWR);
	if (!fdP9_14_Period) perror(BBB_PWM_P9_14_PERIOD);


	write (fdP8_13_Polarity, "0", 1);
	write (fdP8_13_Period, EMAX_ES08A_PWM_PERIOD, 8);
	system("sleep 1");
	write (fdP9_14_Polarity, "0", 1);
	write (fdP9_14_Period, EMAX_ES08A_PWM_PERIOD, 8);

	close (fdSlotConfig);
	close (fdP8_13_Polarity);
	close (fdP8_13_Period);
	close (fdP9_14_Polarity);
	close (fdP9_14_Period);
	
	printf("complete.\n");
}


int openFile(char * sFileName)
{
	int fd;
	
	fd = open (sFileName, O_RDWR);	
	if (!fd) perror(sFileName);

	return fd;
}
	
