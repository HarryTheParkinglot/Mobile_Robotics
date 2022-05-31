/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

#define ar acf =cacf = 0;
#define set pos[0]= pos[1] = pos[2] = gyro = rearGyro = 0;
#define lcs lcd_clear_screen();
#define cw wa = 0;
#define V3 while(!Cmd(0,19));
#define SV1 while(!Cmd(0,18));
#define wait if(key ){SS(); while(SW2); while(!SW2);}

int key= 0, key_puck = 0;

int main(void){
    volatile int yaw=0;
    char str[21];

    Interface_init();


		
	write_gyro(0x1E, 1);	//지자기 센서 보정 시작
	write_gyro(0x1E, 5);	//지자기 센서 보정 시작
	write_gyro(0x1E, 20);	//지자기 센서 보정 시작
	write_gyro(0x1E, 16);	//지자기 센서 보정 시작
	//write_gyro(0x1E, 17);	//지자기 센서 보정 종료
	//write_gyro(0x1E, 20);	//포즈센서 초기화
    
	Camera_init();
	Setting(13);
	TM(80);
	V3
	  
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);
	set
	while(1){
	   if(SW1){
	   		key--;
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);
	   }
	   if(SW3){
	   		key++;
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);
	   }
	   if(SW2){
	   		lcs
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);
			switch(key){
case 0:
case 1:
//
Avoid(0,12,16,0,50,50,60);
Ad(0,40,-90,40,-50,100);







//


SS();
while(SW2);
while(!SW2);
break;
			}
	   }
	   if(key){
	   	lcd(0,0,"%03d %03d %03d %03d %03d",g_psd[2],g_psd[1],g_psd[0],g_psd[8],g_psd[7]);
	   	lcd(1,0,"%03d %03d     %03d %03d",g_psd[3],g_psd[4],g_psd[5],g_psd[6]);
	   }
	   else{
	   	lcd(0,0,"%03d %03d %03d %03d %03d",psd[2],psd[1],psd[0],psd[8],psd[7]);
	   	lcd(1,0,"%03d %03d     %03d %03d",psd[3],psd[4],psd[5],psd[6]);
	   }
	   key_puck  = abs(key) % 9;
	   
	   lcd(2,0,"C:%d X%3d Y%3d S%2d",key_puck,Cmd(key_puck,102),Cmd(key_puck,103),Cmd(key_puck,104));
	   lcd(3,0,"%d%d%d%d%d|%d%d G:%03d",IR_4,IR_1,IR_2,IR_3,IR_5,IND_1,IND_2,(int)gyro);
	   lcd(3,16,"K:%02d",key);
    }
}


