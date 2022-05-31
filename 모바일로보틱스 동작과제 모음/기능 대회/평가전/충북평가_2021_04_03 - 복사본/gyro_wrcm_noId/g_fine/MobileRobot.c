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

ar ab645(30);

Drift(0,20,-20,30,40);	wa = -20;
Drift(5,20,-50,30,0);	wa = -70;
Ot(10,-90,-20,30,0);	cw

CPsd(12,8,5,3,18,0,40,0);
CPsd(10,8,5,5,18,0,40,0);
CPsd(8,8,1000,3,18,25,40,0);

Drift(1,41,-90,30,0);	
CPsd(12,1,5,3,11,0,40,50);
CPsd(10,1,10,5,11,0,40,0);
Drift(1,33,-90,30,0);	

CPsd(12,1,5,3,11,0,40,50);
CPsd(10,1,10,5,11,0,40,-50);


case 6:


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
