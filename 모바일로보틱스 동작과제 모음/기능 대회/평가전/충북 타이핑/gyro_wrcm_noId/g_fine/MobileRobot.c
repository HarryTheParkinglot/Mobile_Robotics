/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

#define tx 141
#define ty 101
#define cw wa = 0;
#define ar acf =cacf= 0;
#define set pos[0] = pos[1] = pos[2] = 0; gyro = rearGyro = 0;
#define V3 while(!Cmd(0,19));
#define SV1 while(!Cmd(0,18));
#define reset bar_1 = bar_2 = bar_3 = bar_4 = bar_5 = sbar_1 = sbar_2= 0;
#define wait if(key) {SS(); while(SW2); while(!SW2);}
#define lcs lcd_clear_screen();
int key= 0, key_puck = 0;

#define ab45(y)    wa = 90; AB(0,0,4,191,5,173,30,0); cw
#define ab345(y) wa = 90; AB(3,126,4,191,5,173,30,y); cw
#define ab645(y) wa = 90; AB(6,144,4,191,5,173,30,y); cw
#define ab13(y) AB(0,0,1,174,3,172,30,y);	

int goal,gate[4] = {0},cm = 0;
int col[3] = {0};
int main(void)
{

    Interface_init();

	write_gyro(0x1E, 1); // 지자기 OFF
	write_gyro(0x1E, 5); // 자이로 500dps	
	write_gyro(0x1E, 20); // 능동적 진동성분제거 OFF

	write_gyro(0x1E, 16); // 자이로 보정

	Camera_init();
	Setting(11);
	TM(50);
	V3

	_delay_ms(500);

	set

	LED_ON(3);
	_delay_ms(100);
	LED_OFF(3);

	while(1)
	{		
	    if(SW1)
		{
			key--;
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);

		}
	    if(SW3)
		{
			key++;
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);

		}
		
		
	    if(SW2)
		{
			lcd_clear_screen();
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);

			switch(key)
			{

case 0:
case 1:
case 2:


ar ab345(30);
CPsd(12,1,5,3,12,0,40,50);
CPsd(10,1,5,5,12,0,40,0);

Drift(2,28,-90,30,0);	wa = -90;
Ot(10,-90,0,35,0);		cw


goal = T(101,3);

CPsd(12,1,5,3,12,0,40,0);
CPsd(10,1,12,5,12,0,40,-50);

Axis(0,-15,0,30,40);
ab45(30);
ar ab345(30);

Axis(0,-2,0,10,120);

Drift(28,0,70,30,40);	wa = -20;
Ot(-20,90,90,30,0);	

sbar_2= 0 ;
CPsd(12,6,-5,3,12,0,40,0);
CPsd(10,6,-5,5,12,0,40,0);
CPsd(6,6,-1000,3,12,20,40,0);
Ad(0,-15,40,40,-50,40);	wa = 20;

gate[1] = sbar_2;

if(key == 1) gate[1] = 0;
if(key == 2) gate[1] = 1;


Drift(-5,25,70,25,35);	cw

set

CPsd(12,8,5,3,14,0,40,50);
CPsd(10,8,5,5,14,0,40,0);
CPsd(8,8,1000,3,14,20,40,0);

gate[0] = (pos[1] > 40);

Drift(2,39,-90,25,0);	wa = - 90;
Ot(3,-90,0,25,0);	cw

Avoid(0,12,16,0,10,40,0);
Ad(1,35,-90,40,-50,80);	

if(gate[0]){
	cm = 30;
	Axis(1,3,0,10,120);
	ar ab345(30);
}
else{
	ar ab13(30);
}

if(T(101,3) == goal){
	set
	CPsd(12,1,5,3,12,0,30,40);
	CPsd(10,1,5 + cm,5,12,0,30,-40);
	Axis(0,-pos[1],0,30,140);
}

if(gate[0]){
	ar ab345(30);
	Axis(0,-2,0,10,120);
}
else{
	ar ab13(30);
}


if(gate[1] == 0){
	Drift(29,-1,70,30,40);	wa = -20;
	Ot(-20,90,90,30,0);	

	CPsd(12,6,-3,3,12,0,40,0);
	CPsd(10,6,-5,5,12,0,40,0);
	CPsd(6,6,-1000,3,12,20,40,0);

	Avoid(5,12,16,20,-1000,40,0);
	Ot(-14,-90,0,30,0);
	Drift(-15,0,-50,25,0); wa = 20;

	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(-4,3,-1000,5,18,20,40,0);

	Ot(-14,90,10,30,0);	wa = 30;
	Drift(15,0,40,25,0);	wa = -20;

	CPsd(12,6,-3,3,14,0,40,0);
	CPsd(10,6,-5,5,14,0,40,0);
	CPsd(-5,6,-1000,3,14,20,40,0);

	Ot(-14,-90,30,30,0);	wa = -80;

	CPsd(12,3,5,3,12,0,40,0);
	CPsd(10,3,5,5,12,0,40,0);
	CPsd(-2,3,1000,5,12,20,40,0);
	Ad(0,10,-10,40,-50,40);	cw
	
	ar ab345(30);
}

else{
	if(gate[0] == 0){
		Drift(40,-1,90,30,40);	
		Ot(-10,90,10,30,0);	wa = 100;
		Ot(12,-90,10,30,0);	wa = 20;
	}
	else{
		Drift(30,-1,70,30,40);	wa= -20;	
		Ot(-20,90,90,30,0);	
		CPsd(12,6,-5,3,12,0,40,0);
		CPsd(10,6,-7,5,12,0,40,0);

		Ot(-20,-90,-40,30,0);	wa = 30;
		Drift(0,-5,-10,30,0);	wa = 20;
	}

	CPsd(12,5,-3,3,14,0,40,0);
	CPsd(-4,5,-1000,5,14,20,40,0);
	Ot(-14,-90,-40,30,0);	wa = -20;

	Drift(-15,0,-50,25,0);	wa = 20;

	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-5,5,18,0,40,0);
	CPsd(-4,3,-1000,5,18,20,40,0);
	Ot(-14,90,-30,30,0);	wa = 80;

	CPsd(12,6,5,3,16,0,40,0);
	CPsd(10,6,5,5,16,0,40,0);
	CPsd(-7,6,1000,5,16,20,40,0);
	Ad(0,10,10,40,-50,40);	cw

	ar ab645(30);
}




if(gate[1] == 0){
	case 3:
	ar ab345(30);
	Drift(0,12,20,30,40);	wa = 20;
	Ot(15,90,70,30,0);	cw
	Ot(5,-90,0,30,0);
	Axis(30,0,0,40,-50);	cw

	ar ab645(30);
}

else{

	case 4:
	ar ab645(30);
	Drift(0,18,-10,30,40);	wa = -10;
	Ot(10,-90,0,30,0);	wa = 80;
	Ot(5,90,10,30,0);	cw
	

	CPsd(12,1,5,3,12,0,40,0);
	CPsd(10,1,5,5,12,0,40,0);
	CPsd(1,1,1000,3,12,20,40,0);
	Drift(2,25,-70,30,0);	wa = -70;
	Ot(15,90,-20,30,-40); 	cw

	ar ab645(30);
}


case -2:
if(key == -2) goal = 2;


Push();

Drift(0,10,-20,30,40);	wa = -20;
Ot(20,-90,-70,30,0);	cw

Ot(20,90,-70,30,0);		wa = 20;
Drift(0,-10,-20,30,-40);	cw

ar ab645(30);

Push();

ar ab645(30);

//




//=---------------=
//Move to Push Push

case 5:

	ar ab645(30);
	Drift(-15,0,-20,30,40);	wa = 70;
	Ot(-25,90,0,30,0);	wa = -20;
	Avoid(5,12,16,25,-1000,40,0);
	Ot(-15,-90,0,30,0);		
	Ad(-5,0,-70,30,-40,100);	cw

	ar ab645(30);



///GetOut




//===================GETOUT=======================
if(gate[1]){

	case 6:

	ar ab645(30);
	Axis(-20,0,0,30,40);	wa = -90;
	Ot(20,90,20,30,0);	wa = 20;
	Avoid(4,12,16,20,-1000,40,0);

	Ot(-14,90,50,25,0);	wa = -20;

	CPsd(12,6,-5,3,12,0,40,0);
	CPsd(-5,6,-1000,5,12,20,40,0);

	Ot(-13,-90,0,35,0);	
	Drift(-15,-1,-50,25,0);	wa = 20;
	
	CPsd(12,3,-3,3,16,0,40,0);
	CPsd(3,3,-1000,5,16,25,40,0);
	Axis(0,-10,0,40,0);
	CPsd(12,5,-5,3,11,0,40,0);
	CPsd(10,5,-5,5,11,0,40,0);
	CPsd(-4,5,-1000,5,11,20,40,0);
	Axis(0,-15,0,40,-50);
}

else{
	case 7:

	ar ab645(30);
	At(-70,80);	wa = -70;
	Drift(0,10,-20,30,0);	wa = -90;
	Ot(8,-90,0,30,0);	cw

	CPsd(12,8,5,3,16,0,40,0);
	CPsd(10,8,5,5,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);
	Drift(2,38,-90,30,0);	wa = -90;
	Ot(5,-90,0,30,0);	cw

	CPsd(12,8,5,3,16,0,40,0);
	CPsd(10,8,5,5,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);

	Drift(0,23,-30,30,0);	wa = -30;
	Ot(20,-90,10,30,0);	wa = 70;
	Ot(10,90,0,30,0);	wa = -20;

	Drift(2,38,-70,30,0);	cw

	
	CPsd(12,1,3,3,12,0,40,50);
	CPsd(1,1,1000,3,12,20,40,0);

	Drift(1,28,-80,30,0);	wa = -80;
	Avoid(2,12,16,12,1000,40,0);
	Ot(6,90,10,30,0);	wa = 20;


	CPsd(12,5,-5,3,11,0,40,0);
	CPsd(10,5,-5,5,11,0,40,0);
	CPsd(-4,5,-1000,5,11,20,40,0);
	Axis(0,-15,0,40,-50);
}


case -1:
ar ab645(30);
Axis(0,10,0,40,50);
Ot(25,-90,0,30,0);	wa = 90;
Ot(25,90,90,30,-40);	cw
ar ab645(30);

SS();

break;
case -4:
ar ab345(30);
Drift(0,8,20,30,40);	wa =20;
Ot(20,90,-20,30,0);	cw

MovePsd(0,1,-1000,0,0,26,40,0);	wa = -90;
Ot(10,-90,0,30,0);	cw

CPsd(12,1,5,3,12,0,40,0);
CPsd(1,1,1000,3,12,20,40,0);
Drift(2,25,-70,30,0);	wa = -70;
Ot(15,90,-20,30,-40	);cw
ar ab645(30);

//




case 8:
case 9:
case 10:
if(key) goal = 1;

ar ab645(30);
sbar_1 = 0;
Drift(0,20,-20,30,40);	wa = -20;


gate[2] = 1 - sbar_2;

if(key == 9) gate[2] = 0;
if(key == 10)	gate[2] = 1;

if(gate[2]){
	Drift(2,20,-50,30,0);	wa = -70;
	Ot(10,-90,-20,35,0);	cw

	CPsd(12,8,5,3,18,0,40,0);
	CPsd(10,8,10,5,18,0,40,0);
	CPsd(8,8,1000,3,18,25,35,0);

	gate[3] = (g_psd[2] > 30);
	
	Drift(2,41,-90,30,0);	wa = -90;
	Ot(5,-90,0,30,0);	cw
	
	CPsd(12,1,5,3,11,0,35,0);
	CPsd(10,1,10,5,11,0,35,0);
	Drift(2,30,-91,30,0);	wa=  -90;
	Ot(5,-90,0,30,0);	cw 
	
	CPsd(12,1,5,3,11,0,35,0);
	
	if(T(-102,3) == goal) cm = 15;
	else cm = 0;

	lcd(1,1,"goal ; %d puck : %d",goal,T(-102,3));

	CPsd(10,1,5 + cm,5,11,0,35,-45);

	
	Axis(0,-cm-5,0,30,40);
	ab45(30);

	ar ab345(30);

	Axis(0,-2,0,10,120);

	Ad(45,0,90,40,150,80);
	
case 11:
case 12:
if(key == 11) gate[2] = 0;
if(key == 12) gate[2] = 1;

	ar ab345(30);
	
	Drift(20,-1,70,30,40); wa= -20;

	CPsd(12,6,-5,3,16,0,40,0);
	CPsd(10,6,-10,5,16,0,40,0);
	CPsd(6,6,-1000,3,16,20,40,0);
	Avoid(5,16,18,20,-1000,40,0);
	AB(5,194,0,0,0,0,0,40);

	wait

	if(gate[3] == 0){
		set
		At(90,60);	wa = 70;
		SS();
		Drift(0,12,20,30,40);	wa = 90;
		Ot(5,90,0,35,0);	cw

		Avoid(0,12,16,0,10,40,0);
		Drift(3,40,-90,35,0);
		
	}
	else{
		At(20,20);	cw SS();
		CPsd(12,1,3,3,14,0,40,50);
		CPsd(1,1,1000,3,14,20,40,0);
		Drift(-2,35,90,30,0);	wa= 90;
		Ot(5,90,0,35,0);	cw
		Avoid(0,12,16,0,10,40,0);
		Drift(2,40,-90,35,0);
	}
	
	CPsd(12,1,3,3,14,0,40,50);
	CPsd(10,1,5,5,14,0,40,0);
	CPsd(1,1,1000,3,14,20,40,0);

	Drift(-2,34,90,30,0);	wa = 90;
	Ot(5,90,0,30,0);	cw

	CPsd(12,8,5,3,15,0,40,0);
	CPsd(10,8,5,5,15,0,40,0);
	
	Drift(-1,33,91,30,0);	wa = 90;
	Ot(5,90,0,30,0);	cw

	CPsd(12,8,5,3,15,0,35,0);
	
	if(T(102,3) == goal) cm = 15;
	else cm = 0;

	lcd(1,1,"goal ; %d puck : %d",goal,T(-102,3));

	CPsd(10,8,5 + cm,5,15,0,35,-45);

	
	Axis(0,-cm-5,0,30,40);
	ab45(30);


	ar ab645(30);
	Axis(0,-2,0,10,120);
	
	Drift(-33,0,-70,30,40);		wa = 20;
	Ot(-20,-90,-90,30,0);	

	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-10,5,18,0,40,0);
	CPsd(3,3,-1000,3,18,25,40,0);
	Avoid(4,14,18,25,-100,40,0);
	Ot(-19,-90,-90,40,0);
	
	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-5,5,18,0,40,0);
	CPsd(-4,3,-1000,3,18,20,40,0);
	Ot(-14,90,-30,30,0);	wa = 80;

	
	CPsd(12,6,5,3,18,0,40,0);
	CPsd(10,6,5,5,18,0,40,0);
	CPsd(-7,6,1000,3,18,20,40,0);
	Ad(0,10,10,40,-50,20);	cw

	ar ab645(30);
}

else{
	Axis(0,10,0,40,-50);

	CPsd(12,6,-5,3,12,0,40,50);
	CPsd(10,6,-5,5,12,0,40,0);
	CPsd(-5,6,-1000,5,12,20,40,0);
	Ot(-12,-90,30,30,0);	wa = -80;

	CPsd(12,3,5,3,14,0,40,0);
	CPsd(10,3,5,5,14,0,40,0);
	CPsd(-2,3,1000,5,14,20,40,0);
	
	Ad(0,10,-10,40,-50,20);	cw

	ar ab345(30);


	Drift(0,12,20,30,40);	wa = 20;
	Ot(15,90,70,30,0);	cw
	Ot(7,-90,0,30,0);
	Axis(28,0,0,40,-50);

	ar ab645(30);

	
	At(-70,60);	wa = -70;
	Drift(0,10,-20,30,40);	wa = -90;
	Ot(8,-90,0,30,0);	cw

	CPsd(12,8,5,3,16,0,40,0);
	CPsd(10,8,5,5,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);
	Drift(2,35,-90,30,0);	wa = -90;
	Ot(8,-90,0,30,0);	cw

	CPsd(12,8,5,3,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);

	Drift(0,24,-30,30,0);	wa = -30;
	Ot(20,-90,30,30,0);	wa = 90;
	Ot(10,90,0,30,0);	cw

	CPsd(12,1,5,3,14,0,40,0);
	CPsd(10,1,10,5,14,0,40,0);
	CPsd(1,1,1000,5,14,20,40,0);

	Drift(-2,34,90,30,0);	wa = 90;
	Ot(5,90,0,30,0);	cw

	CPsd(12,8,5,3,15,0,40,0);
	CPsd(10,8,5,5,15,0,40,0);
	
	Drift(-1,33,91,30,0);	wa = 90;
	Ot(5,90,0,30,0);	cw

	CPsd(12,8,5,3,15,0,35,0);
	
	if(T(102,3) == goal) cm = 15;
	else cm = 0;

	lcd(1,1,"goal ; %d puck : %d",goal,T(-102,3));

	CPsd(10,8,5 + cm,5,15,0,35,-45);

	
	Axis(0,-cm-5,0,30,40);
	ab45(30);


	ar ab645(30);
	Axis(0,-2,0,10,120);

	Ad(-45,0,-90,40,150,100);	


case 13:
case 14:

if(key == 13) gate[3] = 0;
if(key == 14) gate[3] = 1;

	ar ab645(30);

//////

	Drift(-20,0,-70,30,40);	wa = 20;

	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-10,5,18,0,40,0);
	CPsd(3,3,-1000,3,18,25,40,0);

	Avoid(4,16,18,20,-1000,40,0);
	AB(4,194,0,0,0,0,0,40);

	wait

	if(gate[3] == 0){
		set
		At(-90,60);	wa = -70;
		SS();
		Drift(0,13,-17,30,40);	wa = -90;
		Ot(5,-90,0,30,-40);	cw
		wait
		Avoid(0,12,16,0,10,40,50);
		Drift(-3,40,90,35,0);
		
	}
	else{
		At(-20,20);	cw SS();
		CPsd(12,8,3,3,18,0,40,50);
		CPsd(8,8,1000,3,18,25,40,0);
		Drift(2,39,90,30,0);	wa= 90;
		Ot(5,90,0,35,0);	cw
		Avoid(0,12,16,0,10,40,0);
		Drift(-2,40,90,35,0);
	}
	
	CPsd(12,8,3,3,18,0,40,50);
	CPsd(10,8,5,5,18,0,40,0);
	CPsd(8,8,1000,3,18,25,40,0);

	Drift(2,37,-90,30,0);	wa = -90;
	Ot(5,-90,0,30,0);	cw

	CPsd(12,1,5,3,11,0,40,0);
	CPsd(10,1,5,5,11,0,40,0);
	
	Drift(1,35,-91,30,0);	wa = -90;
	Ot(5,-90,0,30,0);	cw

	CPsd(12,1,5,3,11,0,35,0);
	
	if(T(102,3) == goal) cm = 15;
	else cm = 0;

	lcd(1,1,"goal ; %d puck : %d",goal,T(-102,3));

	CPsd(10,1,5 + cm,5,11,0,35,-45);

	
	Axis(0,-cm-5,0,30,40);
	ab45(30);


	ar ab345(30);
	Axis(0,-2,0,10,120);
	
	Drift(33,-1,70,30,40);		wa = -20;
	Ot(-20,90,90,30,0);	

	CPsd(12,6,-5,3,14,0,40,0);
	CPsd(10,6,-5,5,14,0,40,0);
	CPsd(6,6,-1000,3,14,25,40,0);
	Avoid(5,14,18,25,-100,40,0);
	Ot(-19,90,90,40,0);
	
	CPsd(12,6,-5,3,14,0,40,0);
	CPsd(10,6,-5,5,14,0,40,0);
	CPsd(-5,6,-1000,3,14,20,40,0);
	Ot(-14,-90,30,30,0);	wa = -80;

	
	CPsd(12,3,5,3,14,0,40,0);
	CPsd(10,3,5,5,14,0,40,0);
	CPsd(-2,3,1000,5,14,20,40,0);
	Ad(0,10,-10,40,-50,20);	cw

	ar ab345(30);
}



while(!SW2);
while(SW2);

			}
		}

		if(key){
			lcd(0,0,"%03d %03d %03d %03d %03d",g_psd[2],g_psd[1],g_psd[0],g_psd[8],g_psd[7]);
			lcd(1,0,"%03d %03d      %03d %03d",g_psd[3],g_psd[4],g_psd[5],g_psd[6]);
		}
		else{
			lcd(0,0,"%03d %03d %03d %03d %03d",psd[2],psd[1],psd[0],psd[8],psd[7]);
			lcd(1,0,"%03d %03d      %03d %03d",psd[3],psd[4],psd[5],psd[6]);
			}
		key_puck = abs(key) %9;
	
		get_gyro();
		lcd(2,0,"C%d X%3d Y%3d S%2d",key_puck,Cmd(key_puck,102),Cmd(key_puck,103),Cmd(key_puck,104));		
		lcd(3,0,"%d%d%d%d%d|%d%d G%04d",IR_4,IR_1,IR_2,IR_3,IR_5,IND_1,IND_2, (int)gyro);
		lcd(3,16,"k:%02d",key);
	}
}	


void Push(){
	ar ab645(30);
	Setting(12);

	SV1
	V1();
	V3
	
	for(int i = 0 ; i < 9 ; i++){
		if(!pro[i][0]) continue;

		if(abs(pro[i][1] - 47) < 20) col[0] = pro[i][0];
		if(abs(pro[i][1] - 116) < 20) col[1] = pro[i][0];
		if(abs(pro[i][1] - 186) < 20) col[2] = pro[i][0];
	}
	
	
	lcs lcd(1,1,"%d %d %d",col[0],col[1],col[2]);
	wait
	if(col[0] == goal || col[1] == goal || col[2] == goal){
		Axis(-4,22,0,20,130);
		
		Setting(11);
		ar	TB(col[1],0,tx,ty);

		if(col[0] == goal){
			Axis(-10,0,0,20,130);
			ar	TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
			Axis(10,0,0,20,130);
		}
		if(col[1] == goal){
			ar	TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
		}
		if(col[2] == goal){
			Axis(10,0,0,20,130);
			ar	TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
			Axis(-10,0,0,20,130);
		}

		Axis(2,-18,0,30,40);
		ab645(30);
	}
	
	memset(col,0,sizeof(int)*3);
	
	At(70,100);	wa = 70;
	

	SV1
	V1();
	V3
	for(int i = 0 ; i < 9 ; i++){
		if(!pro[i][0]) continue;

		if(abs(pro[i][1] - 48) < 20) col[0] = pro[i][0];
		if(abs(pro[i][1] - 129) < 20) col[1] = pro[i][0];
		if(abs(pro[i][1] - 195) < 20) col[2] = pro[i][0];
	}
	
	

	lcs lcd(1,1,"%d %d %d",col[0],col[1],col[2]);
	wait
	if(col[0] == goal || col[1] == goal || col[2] == goal){
	
		Ad(-12,20,20,30,140,30);	cw
		
		Setting(11);
		ar	TB(col[1],0,tx,ty);

		if(col[0] == goal){
			Axis(-10,0,0,20,130);
			ar TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
			Axis(10,0,0,20,130);
		}
		if(col[1] == goal){
			ar	TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
		}
		if(col[2] == goal){
			Axis(10,0,0,20,130);
			ar	TB(goal,0,tx,ty);
			Axis(0,13,0,20,130);
			Axis(0,-13,0,20,130);
			Axis(-10,0,0,20,130);
		}

	
		Ad(-18,-8,-90,20,20,130);	cw

		ar ab645(30);
	}

	else{
		At(-70,100);	cw

		ar ab645(30);
	}
	memset(col,0,sizeof(int)*3);
		
}


