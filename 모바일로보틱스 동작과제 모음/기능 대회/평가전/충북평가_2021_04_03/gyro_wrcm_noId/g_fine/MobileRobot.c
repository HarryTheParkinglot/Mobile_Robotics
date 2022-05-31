/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"


#define cw wa = 0;
#define ar acf =cacf= 0;
#define set pos[0] = pos[1] = pos[2] = 0; gyro = rearGyro = 0;
#define V3 while(!Cmd(0,19));
#define SV1 while(!Cmd(0,18));
#define reset bar_1 = bar_2 = bar_3 = bar_4 = bar_5 = sbar_1 = sbar_2= 0;
#define wait if(key) {SS(); while(SW2); while(!SW2);}
#define lcs lcd_clear_screen();
int key= 0, key_puck = 0;
int col[3];
#define ab45(y) wa = 90; AB(0,0,4,196,5,170,30,y);	cw
#define ab345(y) wa = 90; AB(3,122,4,196,5,170,30,y);	cw
#define ab645(y) wa = 90; AB(6,150,4,196,5,170,30,y);	cw
#define ab13(y) AB(0,0,1,165,3,157,30,y);	


int gate[4] = {0},goal,cm = 0;

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
Drift(1,22,-70,30,0);	wa = -70;
Ot(15,-90,-20,30,0);	cw

goal = T(101,3);

lcs lcd(1,1,"goal : %d",goal);

CPsd(12,1,5,3,12,0,40,0);
CPsd(10,1,7,5,12,0,40,-50);

Axis(0,-17,0,30,40);
ab45(30);

ar ab345(30);
Axis(0,-2,0,10,120);



Drift(28,0,70,30,40);	wa = -20;
Ot(-20,90,90,30,0);	

CPsd(12,6,-5,3,14,0,40,0);
CPsd(10,6,-10,5,14,0,40,0);
sbar_2=  0;
CPsd(6,6,-1000,3,14,25,40,0);

gate[1] = sbar_2;
if(key == 2) gate[1] = 1;


Ad(0,-15,40,40,-50,40);	wa = 20;

lcs lcd(2,1,"gt[1] : %d",gate[1]);

Drift(-5,25,70,25,35);	cw


set
CPsd(12,8,5,3,16,0,40,50);
CPsd(10,8,5,5,16,0,40,0);
CPsd(8,8,1000,3,16,25,40,0);

gate[0] = (pos[1] > 40);
lcs lcd(3,0,"gt[0] : %d",gate[0]);
Drift(2,37,-90,30,0);	wa = -90;
Ot(5,-90,0,35,0);	cw

Avoid(0,12,16,0,10,40,0);
Drift(2,35,-90,40,-50);




if(gate[0]){
	cm = 30;
	Axis(0,2,0,10,120);
	ar ab345(30);
}
else{
	ar ab13(30);
}

if(T(101,3) == goal){
	set
	CPsd(12,1,4,3,12,0,40,50);
	CPsd(10,1,5 + cm,5,12,0,40,-50);

	Axis(-1,-pos[1],0,30,140);
}


if(gate[0]){
	Axis(0,2,0,10,120);
	ar ab345(30);
	Axis(-1,-2,0,10,120);
}
else{
	ar ab13(30);
}

if(gate[1] == 0){
	Drift(30,0,70,30,40);	wa=  -20;
	Ot(-19,90,90,30,0);

	CPsd(12,6,-5,3,12,0,40,0);
	CPsd(10,6,-5,5,12,0,40,0);
	CPsd(6,6,-1000,5,12,20,40,0);
	Avoid(5,12,16,20,-1000,40,0);
	Ot(-12,-90,0,30,0);
	Drift(-15,0,-50,30,0);	wa= 20;

	CPsd(12,3,-3,3,18,0,40,0);
	CPsd(10,3,-5,5,18,0,40,0);
	CPsd(-4,3,-1000,5,18,20,40,0);
	Ot(-13,90,0,30,0);	
	Drift(15,0,50,30,0);	wa = -20;

	CPsd(12,6,-3,3,14,0,40,0);
	CPsd(10,6,-5,5,14,0,40,0);
	CPsd(-5,6,-1000,5,14,20,40,0);
	Ot(-12,-90,-10,30,0);	wa = 60;

	CPsd(12,4,-5,3,14,0,40,0);
	CPsd(10,4,-5,5,14,0,40,0);
	CPsd(-3,4,-1000,5,14,20,40,0);
	Ad(0,-10,30,40,-50,40);	cw

	ar ab345(30);
}

else{
	if(gate[0]){
		Drift(30,0,70,30,40);	wa=  -20;
		Ot(-18,90,90,30,0);

		CPsd(12,6,-5,3,12,0,40,0);
		CPsd(10,6,-9,5,12,0,40,0); //52
		Ot(-20,-90,-40,30,0);	wa = 30;
		Drift(0,-5,-10,30,0);	wa = 20;

	}	
	else{
		Drift(40,0,90,30,40);	
		Ot(-10,90,10,30,0);	wa= 100;
		Ot(12,-90,10,30,0);	wa= 20;
	}
	CPsd(12,5,-3,3,14,0,40,0);
	CPsd(10,5,-5,5,14,0,40,0);
	CPsd(-4,5,-1000,5,14,20,40,0);
	Ot(-14,-90,-40,30,0);	wa = -20;
	Drift(-15,-1,-50,30,0);	wa = 20;

	CPsd(12,3,-3,3,16,0,40,0);
	CPsd(10,3,-5,5,16,0,40,0);
	CPsd(-4,3,-1000,3,16,20,40,0);
	Ot(-12,90,10,30,0); wa = -60;

	CPsd(12,5,-3,3,16,0,40,0);
	CPsd(10,5,-5,5,16,0,40,0);
	CPsd(-6,5,-1000,3,16,20,40,0);
	Ad(0,-10,-30,40,-50,40);	cw		
	
}






if(gate[1]){
	case 4:
	ar ab645(30);
	Drift(0,17,-10,30,40);	wa = -10;
	Ot(13,-90,0,30,0);	wa = 80;
	Ot(4,90,10,25,0);	cw

	CPsd(12,1,5,3,14,0,40,0);
	CPsd(10,1,5,5,14,0,40,0);
	CPsd(1,1,1000,3,14,20,40,0);
	Drift(2,25,-70,30,0);	wa = -70;
	Ot(15,90,-20,30,-40);	cw

	ar ab645(30);

}

else{
	case 5:
	ar ab345(30);
	Drift(0,13,20,30,40);	wa = 20;
	Ot(15,90,70,30,0);	cw
	Ot(8,-90,0,30,0);	
	Axis(26,0,0,40,-50);	

	ar ab645(30);
}



case 6:
//goal = 2;
ar ab645(30);


Push();
rv();
Push();

ar ab645(30);
Axis(-15,0,0,50,60);	wa = 90;
Ot(-25,90,-20,40,0);	wa = -20;
Avoid(5,12,16,25,-1000,50,0);
Ot(-15,-90,0,45,0);	
Ad(-5,0,-70,50,-60,80);	cw

ar ab645(30);




//-- GetIn Push



case 7:
case 8:


ar ab645(30);

bar_2=  0;
Drift(0,20,-20,30,40);	wa = -20;

gate[2] = bar_2;

if(key == 7) gate[2] = 1;


if(gate[2]){
	Drift(5,20,-50,30,0);	wa = -70;
	Ot(10,-90,-20,30,0);	cw

	CPsd(12,8,5,3,18,0,40,0);
	CPsd(10,8,10,5,18,0,40,0);
	CPsd(8,8,1000,3,18,25,40,0);

	gate[3] = (g_psd[2] > 20);

	Drift(1,43,-90,30,0);	
	
	CPsd(12,1,5,3,11,0,40,50);
	CPsd(10,1,10,5,11,0,40,0);

	Drift(2,34,-90,30,0);
	
	if(T(101,3) == goal) cm = 15;
	else cm = 0;
	CPsd(12,1,5,3,11,0,40,50);
	CPsd(10,1,7 + cm,5,11,0,40,-50);

	Axis(0,-5 - cm,0,30,40);
	ab45(30);

	ar ab345(30);

	Axis(0,-2,0,10,120);
	Drift(28,0,70,30,40);	wa = -20;
	Ot(-21,90,90,30,0);
	CPsd(12,6,-5,3,16,0,40,0);
	CPsd(10,6,-10,5,16,0,40,0);
	CPsd(6,6,-1000,3,16,0,40,0);
	Avoid(5,13,16,20,-1000,40,0);	

	AB(5,181,0,0,0,0,0,30);

	if(gate[3] == 0){
		At(90,100);	SS();
		wa = 70;
		Drift(0,13,20,25,35);	wa = 70;
		Ot(5,90,0,30,0);	cw

		Avoid(0,11,15,0,10,40,0);
		Drift(2,39,-90,30,0);

		CPsd(12,1,5,3,14,0,40,50);
		CPsd(10,1,5,5,14,0,40,0);
		CPsd(1,1,1000,5,14,20,40,0);

		Drift(-2,38,90,30,0);	
	}
	else{
		At(20,30);	cw
		CPsd(12,8,5,3,18,0,40,50);
		CPsd(10,8,10,5,18,0,40,0);
		Drift(-2,28,90,30,0);	wa = 90;
		Ot(5,90,0,30,0);	cw	

		Avoid(0,10,14,0,10,40,0);
	
		Drift(2,39,-90,30,-40);
	
		
		CPsd(12,8,5,3,18,0,40,50);
		CPsd(10,8,5,5,18,0,40,0);

		Drift(-2,38,90,30,0);	
	}


	CPsd(12,8,5,3,15,0,40,50);
	CPsd(10,8,10,5,15,0,40,0);

	Drift(-1,34,90,30,0);

	if(T(101,3) == goal) cm =15;
	else cm = 0;

	CPsd(12,8,5,3,15,0,40,50);
	CPsd(10,8,5 + cm,5,15,0,40,-50);

	Axis(0,-5 - cm,0,30,40);
	ab45(30);

	ar ab645(30);

	Axis(0,-2,0,10,120);
	Drift(-29,0,-70,30,40);	wa = 20;
	Ot(-21,-90,-90,30,0);
	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-10,5,18,0,40,0);
	CPsd(3,3,-1000,3,18,25,40,0);
	Avoid(4,16,18,22,-1000,40,0);	
	Ot(-14,-90,-40,25,0);	wa = -20;
	Drift(-15,-2,-50,25,0);	wa = 20;
	//////

	CPsd(12,3,-3,3,16,0,40,0);
	CPsd(10,3,-5,5,16,0,40,0);
	CPsd(-4,3,-1000,3,16,20,40,0);
	Ot(-12,90,10,30,0); wa = -60;

	CPsd(12,5,-3,3,16,0,40,0);
	CPsd(10,5,-5,5,16,0,40,0);
	CPsd(-6,5,-1000,3,16,20,40,0);
	Ad(0,-10,-30,40,-50,40);	cw	

	ar ab645(30);

	Axis(0,10,0,40,50);
	Ot(25,-90,0,40,0);	wa= 90;
	Ot(25,90,90,35,-45);	cw

	ar ab645(30);


}

else{
	Axis(0,15,0,40,-50);
	case -6:	
	wa = -20;
	CPsd(12,6,-5,3,14,0,40,50);
	CPsd(10,6,-5,5,14,0,40,0);
	CPsd(-5,6,-1000,5,14,20,40,0);
	Ot(-12,-90,30,30,0);	wa= -80;

	CPsd(12,3,5,3,14,0,40,0);
	CPsd(10,3,5,5,14,0,40,0);
	CPsd(-2,3,1000,5,14,20,40,0);
	Ot(10,-90,0,35,0);	wa= 10;
	Ot(22,90,-10,40,0);	wa = -90;

	Ot(14,-90,0,40,0);	cw
	Axis(0,15,0,50,0);

	CPsd(12,8,5,3,16,0,45,0);
	CPsd(10,8,5,5,16,0,45,0);
	CPsd(8,8,1000,3,16,25,45,0);
	Drift(2,35,-90,35,0);	wa = -90;
	Ot(8,-90,0,30,0);	cw
	
	case 9:

	CPsd(12,8,5,3,16,0,40,(key == 9) * 50);
	CPsd(10,8,5,5,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);
	

	Drift(0,25,-30,30,0);	wa= -30;
	Ot(20,-90,10,30,0);	wa  = 70;
	Ot(10,90,20,30,0);	cw


/////////////////////////////
	
	CPsd(12,1,5,3,14,0,40,0);
	CPsd(10,1,5,5,14,0,40,0);
	CPsd(1,1,1000,3,14,20,40,0);

	gate[3] = (g_psd[7] > 20);

	Drift(-1,39,90,30,0);	
	
	CPsd(12,8,5,3,15,0,40,50);
	CPsd(10,8,10,5,15,0,40,0);

	Drift(-2,34,90,30,0);
	
	if(T(101,3) == goal) cm = 15;
	else cm = 0;
	CPsd(12,8,5,3,15,0,40,50);
	CPsd(10,8,7 + cm,5,15,0,40,-50);

	Axis(0,-5 - cm,0,30,40);
	ab45(30);

	ar ab645(30);

	Axis(0,-2,0,10,120);
	Drift(-28,0,-70,30,40);	wa = 20;
	Ot(-21,-90,-90,30,0);
	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-5,5,18,0,40,0);
	CPsd(3,3,-1000,3,18,0,40,0);
	Avoid(4,13,16,20,-1000,40,0);	

	AB(4,180,0,0,0,0,0,30);
	//15
	if(gate[3] == 0){
		At(-90,100);	SS();
		wa = -70;
		Drift(0,13,-20,25,35);	wa = -70;
		Ot(5,-90,0,30,0);	cw

		Avoid(0,10,14,0,10,40,0);
		Drift(-2,39,90,30,0);
///---
		CPsd(12,8,5,3,18,0,40,50);
		CPsd(10,8,5,5,18,0,40,0);
		CPsd(8,8,1000,5,18,25,40,0);
///
		Drift(2,41,-90,30,0);	
	}
	else{
		At(-20,30);	cw
		CPsd(12,1,5,3,14,0,40,50);
		CPsd(10,1,7,5,14,0,40,0);
		Drift(2,30,-90,30,0);	wa = -90;
		Ot(5,-90,0,30,0);	cw	

		Avoid(0,10,14,0,10,40,0);
	
		Drift(-2,39,90,30,-40);
	
		
		CPsd(12,1,5,3,14,0,40,50);
		CPsd(10,1,5,5,14,0,40,0);

		Drift(2,35,-90,30,0);	
	}


	CPsd(12,1,5,3,11,0,40,50);
	CPsd(10,1,10,5,11,0,40,0);

	Drift(1,34,-90,30,0);

	if(T(101,3) == goal) cm =15;
	else cm = 0;

	CPsd(12,1,5,3,11,0,40,50);
	CPsd(10,1,5 + cm,5,11,0,40,-50);

	Axis(0,-5 - cm,0,30,40);
	ab45(30);

	ar ab345(30);

	Axis(0,-2,0,10,120);
	Drift(29,0,70,30,40);	wa = -20;
	Ot(-21,90,90,30,0);
	CPsd(12,6,-5,3,14,0,40,0);
	CPsd(10,6,-10,5,14,0,40,0);
	CPsd(6,6,-1000,3,14,0,40,0);
	Avoid(5,14,18,20,-1000,40,0);	
	Ot(-15,90,40,25,0);	wa = 20;
	Drift(15,-1,50,25,0);	wa = -20;
	//////18 48

	CPsd(12,6,-3,3,18,0,40,0);
	CPsd(10,6,-5,5,18,0,40,0);
	CPsd(-5,6,-1000,3,18,20,40,0);
	Ot(-12,-90,-10,30,0); wa = 60;

	CPsd(12,4,-3,3,18,0,40,0);
	CPsd(10,4,-5,5,18,0,40,0);
	CPsd(-3,4,-1000,3,18,20,40,0);
	Ad(0,-10,30,40,-50,40);	cw	

	ar ab345(30);



case 10:
	Drift(0,10,20,30,40);	wa = 20;
	Ot(20,90,-20,30,0);	cw

	MovePsd(0,1,-1000,0,0,26,40,0);	wa = -90;
	Ot(10,-90,0,30,0);	cw

	CPsd(12,1,5,3,12,0,40,0);
	CPsd(10,1,5,5,12,0,40,0);
	CPsd(1,1,1000,3,12,20,40,0);
	Drift(2,25,-70,30,0);	wa=  -70;
	Ot(15,90,-20,30,-40);	cw
	
	ar ab645(30);
}


///---Rt





if(gate[1] == 0){
	case -10:
	ar ab645(30);
	At(-70,70);	wa = -70;
	Drift(0,10,-19,25,35);	wa = -90;
	Ot(8,-90,0,30,0);	cw
	
	CPsd(12,8,5,3,16,0,40,0);
	CPsd(10,8,5,3,16,0,40,0);
	CPsd(8,8,1000,3,16,25,40,0);
	Drift(1,35,-90,30,0);	wa = -90;
	Ot(8,-90,0,30,0);	cw
	
	case -2:
	CPsd(12,8,5,3,16,0,50,60 * (key == -2));
	CPsd(10,8,5,3,16,0,50,0);
	CPsd(8,8,1000,3,16,25,50,0);
	
	Drift(0,24,-30,40,0);	wa= -30;
	Ot(20,-90,10,40,0);	wa  = 70;
	Ot(10,90,0,40,0);	wa = -20;
	

	case -1:
	wa = -20;
	Drift(2,34,-70,35,40 * (key == -1));			cw

	CPsd(12,1,3,3,11,0,40,50);
	CPsd(10,1,5,5,11,0,40,0);
	CPsd(1,1,1000,3,11,20,40,0);
	Drift(1,30,-80,30,0);	wa=  -80;

	Avoid(2,12,16,13,1000,35,0);
	Ot(6,90,10,30,0);	wa = 20;

	CPsd(12,5,-5,3,11,0,40,0);
	CPsd(10,5,-5,5,11,0,40,0);
	CPsd(-4,5,-1000,5,11,20,40,0);
	Axis(0,-15,0,40,-50);

}
else{
	case -4:
	ar ab645(30);
	Axis(-15,0,0,40,50);	wa = 90;
	Ot(-25,90,20,30,0);	wa = 20;
	Avoid(4,12,16,18,-1000,40,0);
	Ot(-12,90,50,25,0);	wa= -20;

	CPsd(12,6,-3,3,14,0,40,0);
	CPsd(10,6,-3,5,14,0,40,0);
	CPsd(-5,6,-1000,5,14,20,40,0);
	Ot(-11,-90,0,35,0);
	Drift(-15,0,-50,25,0);	wa = 20;

	CPsd(12,3,-3,3,15,0,40,0);
	CPsd(10,3,-5,5,15,0,40,0);
	CPsd(3,3,-1000,3,15,25,40,0);
	Axis(0,-10,0,40,0);

	CPsd(12,5,-3,3,11,0,40,0);
	CPsd(10,5,-5,5,11,0,40,0);
	CPsd(-4,5,-1000,3,11,20,40,0);
	Axis(0,-16,0,40,-50);
}

LED_ON(3);
_delay_ms(2000);
LED_OFF(3);

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
	Setting(12);
	wait
	SV1
	V1();
	V3
	
	for(int i = 0 ; i < 9 ; i++){
		if(!pro[i][0]) continue;

		if(abs(pro[i][1] - 60) < 25) col[0] = pro[i][0];
		if(abs(pro[i][1] - 123) < 25) col[1] = pro[i][0];
		if(abs(pro[i][1] - 185) < 25) col[2] = pro[i][0];
	}

	lcs lcd(1,1,"col : %d %d %d",col[0],col[1],col[2]);
	
	wait
	if(col[0] == goal || col[1] == goal || col[2] == goal){
		set
		Axis(-5,23,0,30,140);
		int x = pos[0];
		int y = pos[1];
		
		if(col[0] == goal){
			Axis(-10,0,0,20,130);
			ar TB(col[0],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);
			Axis(10,0,0,20,130);
		}
				
		if(col[1] == goal){

			ar TB(col[1],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);

		}
				
		if(col[2] == goal){
			Axis(10,0,0,20,130);
			ar TB(col[2],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);
			Axis(-10,0,0,20,130);
		}

		Axis(4,-21,0,30,140);

		ar ab645(30);
	}
	memset(col,0,sizeof(int) * 3);
	At(70,100);	wa = 70;
	

	SV1
	V1();
	V3

	for(int i = 0 ; i < 9 ; i++){
		if(!pro[i][0]) continue;

		if(abs(pro[i][1] - 46) < 25) col[0] = pro[i][0];
		if(abs(pro[i][1] -116) < 25) col[1] = pro[i][0];
		if(abs(pro[i][1] - 181) < 25) col[2] = pro[i][0];
	}

	lcs lcd(1,1,"col : %d %d %d",col[0],col[1],col[2]);

	
	wait
	if(col[0] == goal || col[1] == goal || col[2] == goal){
		set
		Ad(-11,19,20,30,140,20);	cw
		int x = pos[0];
		int y = pos[1];
		
		if(col[0] == goal){
			Axis(-10,0,0,20,130);
			ar TB(col[0],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);
			Axis(10,0,0,20,130);
		}
				
		if(col[1] == goal){

			ar TB(col[1],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);

		}
				
		if(col[2] == goal){
			Axis(10,0,0,20,130);
			ar TB(col[2],0,141,120);
			
			Axis(0,15,0,30,140);
			Axis(0,-15,0,30,140);
			Axis(-10,0,0,20,130);
		}

		Ad(-y,-x,-90,30,140,60);

	}
	else
		At(-70,100);	
	
	cw
	ar ab645(30);	
	memset(col,0,sizeof(int) * 3);
}

void rv(){
	ar ab645(30);
	
	Drift(0,10,-20,40,50);	wa = -20;
	Ot(19,-90,-70,35,0);	cw
	Ot(20,90,-70,35,0);		wa= 20;
	Drift(0,-10,-20,40,-50);	cw

	ar ab645(30);
}
