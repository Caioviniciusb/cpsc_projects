#include <unistd.h>
#include <stdio.h>
#include "gpio.h"
#include "uart.h"
#include "functions.h"
#include "spider.h"
#include "square.h"
#include "ball.h"
#include "dk.h"
#include "heart.h"
#include "squareBig.h"
#include "wall.h"
#include "trophy.h"
#include "star.h"
#include "gameOver.h"
// #include "zero.h"
// #include "one.h"
// #include "two.h"
// #include "three.h"
// #include "four.h"
// #include "five.h"
// #include "six.h"
// #include "seven.h"
// #include "eight.h"
// #include "nine.h"



struct pack{
	int PosX;
	int PosY;
};

struct staticObj{
	int PosX;
	int PosY;
};



struct enemies{
	int PosX;
	int PosY;
	int wall;
};

struct enemies spider1;
struct enemies spider2;
struct enemies spider3;
struct enemies spider4;

struct enemies spike1;
struct enemies spike2;
struct enemies spike3;
struct enemies spike4;

struct pack pack1;

struct staticObj wall1;

//method for writing to latch
void writeLATCH(int bit){

	if (bit == 1){
		gpio[7] = 1 << 9;
	} else {
		gpio[10] = 1 << 9;		
	}
}

//method for writing to clock
void writeCLOCK(int bit){

	if (bit == 1){
                gpio[7] = 1 << 11;
        } else {
                gpio[10] = 1 << 11;
        }
}

//method to wait in micro seconds
void wait(int time){

	unsigned *clo = (unsigned*)CLO_REG;
	unsigned c = *clo + time; 

	while (c > *clo);
}

//method to initialize the clock latch and data lines
void initGPIO(){

	INP_GPIO( CLK );
	OUT_GPIO( CLK );
	
	INP_GPIO( LAT );
	OUT_GPIO( LAT );

	INP_GPIO( DAT );
}

// reads the data line and returns value 
int readDATA(){
	int v;

	v = ((gpio[13] >> 10) & 1);	
	return v;
}	

//method to print any buttons that were pressed
int returnButtons(){

	for (int i=0; i<=11; i++){

                    if (buttons[i] == 0){

								if (i == 3){
									return 5; //START PRESSED

                                }else if (i == 4){
										// if(checker == 0){
											// checker = 1;
											return(1);//up
										// } else {
										// 	return(0);
										// }
                                 } else if (i == 5){
										// if(checker == 0){
										// checker = 1;
                                         return(2);//down
								 		// } else {
										// 	return(0);
										// }
                                 } else if (i == 6){
										// if(checker == 0){
										// checker = 1;
                                        return(3);//left
										// } else {
										// 	return(0);
										// }
                                 } else if (i == 7){
										// if(checker == 0){
										// checker = 1;
                                         return(4);//right
										// } else {
										// 	return(0);
										// }
                                 } else if(i == 9) {
                                    	return(0);//nothing
                            	 }


	    			}	
	}
	return(0);
}


// change so its just returning 1 number - the button that is being pressed 
int ReadSnes(){
	
	while (1){
	
		for (int i=0; i<=15; i++){//resets buttons array back to all unpressed
			buttons[i] = 1;
			buttonChecker[i] = 1;
		}
		
		writeCLOCK(1);//pulse clock
		writeLATCH(1);//pulse latch
	
		wait(12);//wait 12 micro seconds
		writeLATCH(0);//unpulse latch

		check = 0;

		for(int j = 0; j<16; j++){
				
			wait(6);//wait 6 micro seconds

			writeCLOCK(0);//write 0 to clock to unpulse

			wait(6);//wait 6 micro seconds

			int b = readDATA();// reads data line and returns if button is pressed 

			buttons[j] = b;//reads input from data line into button array
			
			writeCLOCK(1);//pulse clock
		}


		//if(buttons[3] == 0){//checks to see if start button is pressed, terminates program when pressed
			//printf("Program is Terminating\n\n");
			//return(0);
		//}


		for(int i=0; i< 16; i++){//checks to see if there has been a new button change
			if(buttons[i] == buttonChecker[i]){
				check++;
			}
		}


		return(returnButtons());		
		// if(check != 16){//prints the buttons pressed as long as there has been a change in button presses
        //     return(returnButtons());
		// }

		for(int i = 0; i < 16; i++){//copies the buttons pressed into buttonChecker array after the first print
			buttonChecker[i] = buttons[i];
		}
		
	}
}

//0 720
int moveSpiderV(int spiderPosX, int spiderPosY, int wall){
	if(wall == 0){
		if(spiderPosY+32 > 710){
			wall = 1;
			return(wall);
		} else {
				drawImage(square.pixel_data,square.width, square.height, spiderPosX, spiderPosY-4);
				drawImage(spider.pixel_data, spider.width, spider.height, spiderPosX, spiderPosY);
				return(wall);
		}
	} else if(wall == 1){
		if(spiderPosY < 6){
			wall = 0;
			return(wall);
		} else{
				drawImage(square.pixel_data,square.width, square.height, spiderPosX, spiderPosY+4);
				drawImage(spider.pixel_data, spider.width, spider.height, spiderPosX, spiderPosY);
				return(wall);
		}
	}
}


int moveSpiderH(int spiderPosX, int spiderPosY, int wall){
		if(wall == 0){
			if(spiderPosX+32 > 1020){
				wall = 1;
				return(wall);
			} else {
				drawImage(square.pixel_data,square.width, square.height, spiderPosX-1, spiderPosY);
				drawImage(spider.pixel_data, spider.width, spider.height, spiderPosX+1, spiderPosY);
				return(wall);
			}
		}else if(wall == 1){
			if(spiderPosX < 260){
				wall = 0;
				return(wall);
			} else {
				drawImage(square.pixel_data,square.width, square.height, spiderPosX+3, spiderPosY);			
				drawImage(spider.pixel_data, spider.width, spider.height, spiderPosX-1, spiderPosY);
				return(wall);
			}
		}

}



void GeneralHitBox(int spiderPosX, int spiderPosY){
			//top check
		if((dkPosY+32 > spiderPosY && dkPosY +32 < spiderPosY+32) && (dkPosX > spiderPosX-32 && dkPosX < spiderPosX+32) ){
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			dkPosX = 260;
			dkPosY = 686;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives --;
			displayHearts(dkLives);
		}
		//bottom check
		if((dkPosY-32 < spiderPosY && dkPosY-32 > spiderPosY-32) && (dkPosX > spiderPosX-32 && dkPosX < spiderPosX+32) ){
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			dkPosX = 260;
			dkPosY = 686;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives--;
			displayHearts(dkLives);
		}
		
		//left check
		if((dkPosX+32 > spiderPosX && dkPosX+32 < spiderPosX+32) && (dkPosY > spiderPosY-32 && dkPosY< spiderPosY+32) ){
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);	
			dkPosX = 260;
			dkPosY = 686;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives --;
			displayHearts(dkLives);		
		}
		
		//right check
		if((dkPosX-32 < spiderPosX && dkPosX-32 > spiderPosX-32) && (dkPosY > spiderPosY-32 && dkPosY < spiderPosY+32) ){
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			dkPosX = 260;
			dkPosY = 686;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives --;
			displayHearts(dkLives);	
		}
}

void staticHitBox(int hitboxX, int hitboxY){
		if((dkPosY+32 > hitboxY && dkPosY +32 < hitboxY+32) && (dkPosX > hitboxX-32 && dkPosX < hitboxX+32) ){
			drawImage(wall.pixel_data,wall.width, wall.height, wall1.PosX, wall1.PosY);
			dkPosY = dkPosY-2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY); 
		}
		//bottom check
		if((dkPosY-32 < hitboxY && dkPosY-32> hitboxY-32) && (dkPosX > hitboxX-32 && dkPosX < hitboxX +32) ){
			drawImage(wall.pixel_data,wall.width, wall.height, wall1.PosX, wall1.PosY);
			dkPosY = dkPosY+2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
		}
		//left check
		if((dkPosX+32 > hitboxX && dkPosX+32 < hitboxX+32) && (dkPosY > hitboxY-32 && dkPosY< hitboxY+32) ){
			drawImage(wall.pixel_data,wall.width, wall.height, wall1.PosX, wall1.PosY);
			dkPosX = dkPosX-2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);	
		}
		
		//right check
		if((dkPosX-32 < hitboxX && dkPosX-32 > hitboxX-32) && (dkPosY > hitboxY-32 && dkPosY < hitboxY+32) ){
			drawImage(wall.pixel_data,wall.width, wall.height, wall1.PosX, wall1.PosY);
			dkPosX = dkPosX+2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
		}
}

void packHitBox(int hitboxX, int hitboxY){

	if((dkPosY+32 > hitboxY && dkPosY +32 < hitboxY+32) && (dkPosX > hitboxX-32 && dkPosX < hitboxX+32) ){
			drawImage(square.pixel_data, square.width, square.height, pack1.PosX, pack1.PosY);
			dkPosY = dkPosY-2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY); 
			dkLives++;
			displayHearts(dkLives);
		}
		//bottom check
		if((dkPosY-32 < hitboxY && dkPosY-32> hitboxY-32) && (dkPosX > hitboxX-32 && dkPosX < hitboxX +32) ){
			drawImage(square.pixel_data, square.width, square.height, pack1.PosX, pack1.PosY);
			dkPosY = dkPosY+2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives++;
			displayHearts(dkLives);
		}
		//left check
		if((dkPosX+32 > hitboxX && dkPosX+32 < hitboxX+32) && (dkPosY > hitboxY-32 && dkPosY< hitboxY+32) ){
			drawImage(square.pixel_data, square.width, square.height, pack1.PosX, pack1.PosY);
			dkPosX = dkPosX-2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives++;
			displayHearts(dkLives);	
		}
		
		//right check
		if((dkPosX-32 < hitboxX && dkPosX-32 > hitboxX-32) && (dkPosY > hitboxY-32 && dkPosY < hitboxY+32) ){
			drawImage(square.pixel_data, square.width, square.height, pack1.PosX, pack1.PosY);
			dkPosX = dkPosX+2;
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
			dkLives++;
			displayHearts(dkLives);
		}
}

		


void moveDK(){
		int a = ReadSnes();
		if(a == 1 && dkPosY > 4){//up
			//printf("UP\n\n");
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			//drawImage(b1.pixel_data, b1.width, b1.height, 0, 0);
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY-2);
			dkPosY -= 2;
		}else if(a == 2 && dkPosY+32 < 714){//down
			//printf("DOWN\n\n");/
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			//drawImage(b1.pixel_data, b1.width, b1.height, 0, 0);			
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY+2);
			dkPosY += 2;
		}else if(a == 3 && dkPosX > 260){//left
			//printf("LEFT\n\n");
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			//drawImage(b1.pixel_data, b1.width, b1.height, 0, 0);			
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX-2, dkPosY);
			dkPosX -= 2;
		}else if(a == 4 && dkPosX+32 < 1020){//right
			//printf("RIGHT\n\n");
			drawImage(square.pixel_data,square.width, square.height, dkPosX, dkPosY);
			//drawImage(b1.pixel_data, b1.width, b1.height, 0, 0);			
			drawImage(dk.pixel_data, dk.width, dk.height, dkPosX+2, dkPosY);
			dkPosX += 2;
		} else if(a == 0){
			//printf("STAY\n\n");
		}

}

void displayHearts(int numHeart){
	int size = 100;


	for (int i = 0; i < numHeart+1; i ++){
		drawImage(squareBig.pixel_data,squareBig.width, squareBig.height, 96, size);
		size += 64;
	}

	size = 100;

	for(int i =0; i < numHeart; i++){
		drawImage(heart.pixel_data, heart.width, heart.height, 96, size);
		size += 64;
	}

}


int trophyHit(int trophyPosX, int trophyPosY){
        // //top check
        // if((dkPosY+32 > trophyPosX && dkPosY+32 < trophyPosY+32) && (dkPosX > trophyPosX-32 && dkPosX < trophyPosX+32) ){
        //     return 1;
        // }
        //bottom check
        if((dkPosY-32 < trophyPosY && dkPosY-32 > trophyPosY-32) && (dkPosX > trophyPosX-32 && dkPosX < trophyPosX+32) ){
            return 1;
        }

        //left check
        if((dkPosX+32 > trophyPosX && dkPosX+32 < trophyPosX+32) && (dkPosY > trophyPosY-32 && dkPosY< trophyPosY+32) ){
            return 1;
        }

        // //right check
        // if((dkPosX-32 < trophyPosX && dkPosX-32 > trophyPosX-32) && (dkPosY > trophyPosY-32 && dkPosY < trophyPosY+32) ){
        //     return 1;
        // }
    return 0;
}

void timer(unsigned time){
    void printf(char *str){
              uart_puts(str);
    }


	unsigned previousC = time %10;
	unsigned rest = time/10;
	unsigned previousB = rest%10;
	rest = rest/10;
	unsigned previousA = rest%10;

	unsigned *clo = (unsigned*)CLO_REG;
	//ogTime = *clo;


	//if((time - *clo) >= 10000){
	if((*clo - time) <= 1000){
		gameTime--;
		
    	unsigned c = gameTime %10;
    	unsigned part = gameTime/10;
    	unsigned b = part%10;
    	part = part/10;
    	unsigned a = part%10; 
		if(previousA != a){
			if(valuePackTime  == 1500){
				drawImage(star.pixel_data, star.width, star.height, 450, 450);	
			}
			valuePackTime++;
			
		// 	if(a == 0){
		// 		drawImage(zero.pixel_data, zero.width, zero.height, 1030, 10);		
		// 	} else if( a == 1){
		// 		drawImage(one.pixel_data, two.width, two.height, 1030, 10);			
		// 	} else if( a== 2){
		// 		drawImage(two.pixel_data, two.width, two.height, 1030, 10);				
		// 	} else if( a== 3){
		// 		drawImage(three.pixel_data, three.width, three.height, 1030, 10);				
		// 	} else if(a == 4){
		// 		drawImage(four.pixel_data, four.width, four.height, 1030, 10);				
		// 	} else if(a == 5){
		// 		drawImage(five.pixel_data, five.width, five.height, 1030, 10);	
		// 	} else if(a == 6){
		// 		drawImage(six.pixel_data, six.width, six.height, 1030, 10);				
		// 	} else if(a == 7){
		// 		drawImage(seven.pixel_data, seven.width, seven.height, 1030, 10);				
		// 	} else if(a == 8){
		// 		drawImage(eight.pixel_data, eight.width, eight.height, 1030, 10);				
		// 	} else if(a == 9){
		// 		drawImage(nine.pixel_data, nine.width, nine.height, 1030, 10);				
		// 	}
		// }

		// if(previousB != b){
		// 	if(b == 0){
		// 		drawImage(zero.pixel_data, zero.width, zero.height, 1090, 10);		
		// 	} else if( b == 1){
		// 		drawImage(one.pixel_data, two.width, two.height, 1090, 10);			
		// 	} else if( b== 2){
		// 		drawImage(two.pixel_data, two.width, two.height, 1090, 10);				
		// 	} else if( b== 3){
		// 		drawImage(three.pixel_data, three.width, three.height, 1090, 10);				
		// 	} else if(b == 4){
		// 		drawImage(four.pixel_data, four.width, four.height, 1090, 10);				
		// 	} else if(b == 5){
		// 		drawImage(five.pixel_data, five.width, five.height, 1090, 10);	
		// 	} else if(b == 6){
		// 		drawImage(six.pixel_data, six.width, six.height, 1090, 10);				
		// 	} else if(b == 7){
		// 		drawImage(seven.pixel_data, seven.width, seven.height, 1090, 10);				
		// 	} else if(b == 8){
		// 		drawImage(eight.pixel_data, eight.width, eight.height, 1090, 10);				
		// 	} else if(b == 9){
		// 		drawImage(nine.pixel_data, nine.width, nine.height, 1090, 10);				
		// 	}	
		// }

		// if(c == 0){
		// 	drawImage(zero.pixel_data, zero.width, zero.height, 1170, 10);		
		// } else if( c == 1){
		// 	drawImage(one.pixel_data, two.width, two.height, 1170, 10);			
		// } else if( c== 2){
		// 	drawImage(two.pixel_data, two.width, two.height, 1170, 10);				
		// } else if( c== 3){
		// 	drawImage(three.pixel_data, three.width, three.height, 1170, 10);				
		// } else if(c == 4){
		// 	drawImage(four.pixel_data, four.width, four.height, 1170, 10);				
		// } else if(c == 5){
		// 	drawImage(five.pixel_data, five.width, five.height, 1170, 10);	
		// } else if(c == 6){
		// 	drawImage(six.pixel_data, six.width, six.height, 1170, 10);				
		// } else if(c == 7){
		// 	drawImage(seven.pixel_data, seven.width, seven.height, 1170, 10);				
		// } else if(c == 8){
		// 	drawImage(eight.pixel_data, eight.width, eight.height, 1170, 10);				
		// } else if(c == 9){
		// 	drawImage(nine.pixel_data, nine.width, nine.height, 1170, 10);				
		// }


		}	printf("timer decreased\n\n");
	}
}



int scene1(){

	fillScreen(0);
	drawRect(256,0,1024,718,0xB,0);

	int a = 0;
	int control = 0;
	int menuFLag;
	spider1.PosX = 988;
	spider1.PosY = 500;
	
	spider2.PosX = 260;
	spider2.PosY = 200;

	spider3.PosX = 500;
	spider3.PosY = 10;

	spider4.PosX = 800;
	spider4.PosY = 10;

	spike1.PosX = 600;
	spike1.PosY = 600;

	spike2.PosX = 850;
	spike2.PosY = 400;

	spike3.PosX = 900;
	spike3.PosY = 100;
	
	//wall1.PosX = 450;
	//wall1.PosY = 450;
	
	pack1.PosX = 450;
	pack1.PosY = 450;

	drawImage(trophy.pixel_data, trophy.width, trophy.height, trophyX, trophyY);

	drawImage(dk.pixel_data, dk.width, dk.height, dkPosX, dkPosY);
	drawImage(ball.pixel_data, ball.width, ball.height, spike1.PosX, spike1.PosY);
	drawImage(ball.pixel_data, ball.width, ball.height, spike2.PosX, spike2.PosY);
	drawImage(ball.pixel_data, ball.width, ball.height, spike3.PosX, spike3.PosY);

	//drawImage(wall.pixel_data, wall.width, wall.height, wall1.PosX, wall1.PosY);

	drawImage(spider.pixel_data, spider.width, spider.height, spider1.PosX, spider1.PosY);
	drawImage(spider.pixel_data, spider.width, spider.height, spider2.PosX, spider2.PosY);
	drawImage(spider.pixel_data, spider.width, spider.height, spider3.PosX, spider3.PosY);
	drawImage(spider.pixel_data, spider.width, spider.height, spider4.PosX, spider4.PosY);

	displayHearts(dkLives);


	while(1){

		if (control == 5){
			menuFLag = menuScreen();

			if (menuFLag == 0){
				return 1;	// return 1 to variable in main and restarts scene 1
			} else if (menuFLag == 1){
				return 0;	// game over, should just come back to main menu actually. Change
			}
		}

		unsigned *timeog = (unsigned*)CLO_REG;
		//timer(gameTime);

		moveDK();
		control = ReadSnes();
		GeneralHitBox(spider1.PosX, spider1.PosY);
		GeneralHitBox(spider2.PosX, spider2.PosY);
		GeneralHitBox(spider3.PosX, spider3.PosY);
		GeneralHitBox(spider4.PosX, spider4.PosY);

		GeneralHitBox(spike1.PosX, spike1.PosY);
		
		//staticHitBox(wall1.PosX, wall1.PosY);

		packHitBox(pack1.PosX, pack1.PosY);

		
		if (dkLives == 0){
			return 0;
		}
		if(trophyHit(trophyX,trophyY)){
			return 2;
		}

		a = moveSpiderH(spider1.PosX, spider1.PosY, spider1.wall);
		spider1.wall = a;
		if(spider1.wall == 0){
			spider1.PosX = spider1.PosX + 1;
		} else {
			spider1.PosX = spider1.PosX - 1;
		}

		a = moveSpiderH(spider2.PosX, spider2.PosY, spider2.wall);
		spider2.wall = a;
		if(spider2.wall == 0){
			spider2.PosX = spider2.PosX + 1; 
		} else {
			spider2.PosX = spider2.PosX - 1;
		}

		a = moveSpiderV(spider3.PosX, spider3.PosY, spider3.wall);
		spider3.wall = a;
		if(spider3.wall == 0){
			spider3.PosY = spider3.PosY + 1;
		} else {
			spider3.PosY = spider3.PosY - 1;
		}

		a = moveSpiderV(spider4.PosX, spider4.PosY, spider4.wall);
		spider4.wall = a;
		if(spider4.wall == 0){
			spider4.PosY = spider4.PosY + 1;
		} else {
			spider4.PosY = spider4.PosY - 1;
		}
		timer(*timeog);
	}

}



int scene2(){
	fillScreen(0);
	drawRect(256,0,1024,718,0xB,0);

	return 3;
}

int menuScreen(){

	fillScreen(0);
	drawRect(256,0,1024,718,0xB,0);

	int a = 0;

	// waits until start or up is pressed 
	while (a != 5 && a != 1){
		a = ReadSnes();

	}
	if (a == 5){
		return 0;

	} if (a == 1){
		return 1;
	}
}

int gameOver(){
	fillScreen(0);
	drawRect(256,0,1024,718,0xB,0);

	drawImage(gameOverIm.pixel_data, gameOverIm.width, gameOverIm.height, 525, 300);

	return 0;
}