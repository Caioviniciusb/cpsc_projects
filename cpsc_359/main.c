#include <unistd.h>
#include <stdio.h>
#include "gpio.h"
#include "uart.h"
#include "fb.h"
#include "dk.h"
#include "square.h"
#include "spider.h"
#include "ball.h"
#include "gameOver.h"


//main
int main(){
	int dkPosX = 270;
	int dkPosY = 690;
	int flag = 1;

 	void printf(char *str){
              uart_puts(str);
    }

	fb_init();

	initGPIO();
	//fillScreen(0);

	// int a = 1;
	// char b = itoa(a);
	// printf(b);


	//drawRect(256,0,1024,718,0xB,0);

	int a = 0;

	// game just starts when we press start
	while (a != 5){
		a = ReadSnes();
		//printf("in loop");
	}
		
		if(flag != 0){

			while(flag == 1){
				flag = scene1();
			}
			while(flag == 2){
				flag = scene2();
			}
		}

		if (flag == 0){
			gameOver();
		}
}

