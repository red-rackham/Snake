/**
 * Snake for Arduino with a 8x8 LED matrix and joystick
 * 
 * (C) 2018 by Jakob Bolliger (jakob.git@jpost.ch)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 */



#include "Arduino.h"
#include "LedControlMS.h"

int snakeX[64] = {3, 2, 1};
int snakeY[64] = {2, 2, 2};
int size = 2;
int dtime = 300;
char dir ='e';
char dirPre = 'e';
bool pray = false;
int prayX = 9;
int prayY = 9;

const int X_pin = 0; // analog pin connected to X output of joystick (VRx)
const int Y_pin = 1; // analog pin connected to Y output of joystick (VRy)

#define NBR_MTX 2
LedControl lc=LedControl(12,11,10, NBR_MTX);

String digits= "1234567890";
int digitCounter=0;
/* we always wait a bit between updates of the display */
unsigned long delaytime=300;

void setup()
{
	  Serial.begin (9600);
	  Serial.println("Setup");
	  digitCounter=0;
	  for (int i=0; i< NBR_MTX; i++){
	    lc.shutdown(i,false);
	  /* Set the brightness to a medium values */
	    lc.setIntensity(i,8);
	  /* and clear the display */
	    lc.clearDisplay(i);
	  }
}

void loop()
{

	joystick();
//	Serial.println("looooop");
	snake();
	joystick();
	hunt();
	joystick();
	display();
	joystick();
	delay(dtime);
}

void hunt(){
	if(!pray){
		long randP = random(0, 6);
		if (randP == 5){
			setPray();
			Serial.print("Pray: ");
			Serial.print(prayX);
			Serial.println(prayY);
		}

	}
}

void setPray(){
	bool freeSpot = false;
	while(!freeSpot){

		prayX = random(0, 8);
		prayY = random(0, 8);

		for(int i=0; i<=size; i++){
			if(snakeX[i] == prayX){
				if(snakeY[i] == prayY){
					freeSpot = false;
				} else {
					freeSpot = true;
				}
			} else {
				freeSpot = true;
			}
		}
	}
	pray = true;
}

void joystick(){


	int x= analogRead(X_pin);
	int y= analogRead(Y_pin);

	if(x < 400){
		dir= 'w';
	} else if(x>600) {
		dir= 'e';
	}  else if(y<400){
		dir= 'n';
	}  else if(y>600){
		dir= 's';
	}
	Serial.println(dir);
}

void snake() {
	int tailX = snakeX[size];
	int tailY = snakeY[size];

		for(int i=size; i>0; i--){
			snakeX[i] = snakeX[i-1];
			snakeY[i] = snakeY[i-1];
		}

		if(dir =='n'){
			if(dirPre != 's'){
				goNorth();
			} else {
				goSouth();
			}
		}

		if(dir == 's'){
			if(dirPre != 'n'){
				goSouth();
			} else {
				goNorth();
			}
		}

		if(dir == 'e'){
			if(dirPre != 'w'){
				goEast();
			} else {
				goWest();
			}
		}

		if(dir == 'w'){
			if(dirPre != 'e'){
				goWest();
			} else {
				goEast();
			}
		}

		if(pray){
			if(prayX == snakeX[0] && prayY == snakeY[0]){
				size++;
				snakeX[size] = tailX;
				snakeY[size] = tailY;
				pray = false;
			}

		}

		for(int i=1; i <=size; i++){
			if(snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]){
				Serial.println("game over");
				gameOver();
			}
			}


//		for(int i=0; i<=size; i++){
//			Serial.print(i);
//			Serial.print(": ");
//			Serial.print(snakeX[i]);
//			Serial.print(" / ");
//			Serial.println(snakeY[i]);
//		}

}

void goNorth(){
	if(snakeY[0] == 7){
		snakeY[0] = 0;
	} else {
		snakeY[0]++;
	}
	dirPre = 'n';
}

void goSouth(){
	if(snakeY[0] == 0){
		snakeY[0] = 7;
	} else {
		snakeY[0]--;
	}
	dirPre = 's';
}

void goEast(){
	if(snakeX[0] == 7){
		snakeX[0] = 0;;
	} else {
		snakeX[0]++;
	}
	dirPre = 'e';
}

void goWest(){
	if(snakeX[0] == 0){
		snakeX[0] = 7;
	} else {
		snakeX[0]--;
	}
	dirPre = 'w';
}

void display(){
	lc.clearAll();

	for(int i=0; i<= size; i++){
		lc.setLed(0,snakeX[i],snakeY[i],true);
	}
	if(pray){
		lc.setLed(0,prayX,prayY,true);
	}
}

void gameOver(){

	cyrcle3();
	lc.clearAll();
	lc.writeString(0,"game over");
	cyrcle3();
	cyrcle3neg();
	lc.clearAll();
	restart();
}

void restart(){
	snakeX[0] = 3;
	snakeY[0] = 2;
	snakeX[1] = 2;
	snakeY[1] = 2;
	snakeX[2] = 1;
	snakeY[2] = 2;
	size = 2;
	dir ='e';
	dirPre = 'e';
	bool pray = false;
}

void cyrcle3(){
	int dtime=20;

	for(int i=3; i>=0; i--){

		for(int y=6-i; y>i-1; y--){
			lc.setLed(0,i,y,true);
			delay(dtime);
		}

		for(int x=i+1; x<=7-i; x++){
			lc.setLed(0,x,i,true);
			delay(dtime);
		}
		for(int y=i+1; y<=7-i; y++){
			lc.setLed(0,7-i,y,true);
			delay(dtime);
		}
		for(int x=7-i; x>i-1; x--){
			lc.setLed(0,x,7-i,true);
			delay(dtime);
		}


	}
}

void cyrcle3neg(){
	int dtime=20;

	for(int i=3; i>=0; i--){

		for(int y=6-i; y>i-1; y--){
			lc.setLed(0,i,y,false);
			delay(dtime);
		}

		for(int x=i+1; x<=7-i; x++){
			lc.setLed(0,x,i,false);
			delay(dtime);
		}
		for(int y=i+1; y<=7-i; y++){
			lc.setLed(0,7-i,y,false);
			delay(dtime);
		}
		for(int x=7-i; x>i-1; x--){
			lc.setLed(0,x,7-i,false);
			delay(dtime);
		}


	}
}
