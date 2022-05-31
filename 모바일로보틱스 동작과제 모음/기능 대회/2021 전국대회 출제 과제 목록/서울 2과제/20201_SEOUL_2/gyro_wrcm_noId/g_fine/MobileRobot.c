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

int key= 0, key_puck;

#define ab45    wa = 90; AB(0,0,4,184,5,167,50,0); cw
#define ab345(y) wa = 90; AB(3,126,4,184,5,167,50,y); cw
#define ab645(y) wa = 90; AB(6,144,4,184,5,167,50,y); cw

#define ab865(y) AB(5,195,6,173,8,125,30,y); cw

#define ab134(y) AB(4,188,1,165,3,158,30,y); cw
#define ab86 AB(0,0,6,136,8,112,50,0);

#define ab0 AB(0,166,0,0,0,0,0,30); cw

#define tx 130
#define ty 134

int map[25] = {
0,0,0,0,0,
0,1   ,2,0,0,
0,3,0,0,0,
0,2,0,0,0,
0,0,0,0,0
};

int call = 0,rTime = 0;
int remainCnt = 0,insideCnt = 0;
int seetPointList[4] = {0},seetCnt = 0;
int dir,dis[4][3];
int posList[16],posCnt = 0;
int maxPos,mPos,min = 0,flg = 0;
int spot[5][3],g_spot[5][3];

int Finish(int *node){
   for(int i = 0 ; i < seetCnt ; i++)
      if(node[seetPointList[i]]) return 0;   
   return 1;
}

int ccw(int p1, int p2, int p3) {
    double d1[2] = { p1 % 5, p1 / 5 };
    double d2[2] = { p2 % 5, p2 / 5 };
    double d3[2] = { p3 % 5, p3 / 5 };

    return (d2[0] - d1[0]) * (d3[1] - d1[1]) - (d2[1] - d1[1]) * (d3[0] - d1[0]); 
}

int IsInsideOfTriangle(int p, int* tri) {
    int c[3];
    for (int i = 0; i < 3; i++) {
        c[i] = ccw(tri[i], tri[(i + 1) % 3], p);
        if (!c[i])	return 0;
    }
    return c[0] < 0 == c[1] < 0 && c[1] < 0 == c[2] < 0;
}


void TriangleDFS(int posIndex, int countIndex, int tcnt){  
	call++;
	
	if(call > 1000) {
		flg = (flg == 1)? 1 : 2;
		return;
	}
	
	if(tcnt > min) return;
	
    if(countIndex == 0){
		if(Finish(map)){
        	flg = 1;
        	min = tcnt;
        	memcpy(g_spot,spot,sizeof(spot));
        	return;
        }
    }
      
    if(tcnt < min && countIndex < 3){    //아직 점 3개 Select 안된 경우 
		for(int i = posIndex ; i >= 0 && i < 16 ; i += dir){				
			spot[tcnt][countIndex] = posList[i];
			
			if(countIndex == 0){
				mPos = 0;
				
				for(int j = 0 ; j < seetCnt ; j++)
					if(map[mPos] < map[seetPointList[j]]) mPos = seetPointList[j]; 
				
				if(i / 5 == mPos / 5 || i / 5 == mPos / 5) continue;
				
				if(mPos == 0) return;
				if(dir > 0 && (posList[i] >= 15 || mPos <= posList[i])) return;
				if(dir < 0 && (posList[i] <= 9 || mPos >= posList[i])) return;
			}
			
			else if(countIndex == 1){
				if(mPos % 5 == spot[tcnt][0] % 5 && mPos % 5 == spot[tcnt][1] % 5) continue;
           		if(mPos / 5 == spot[tcnt][0] / 5 && mPos / 5 == spot[tcnt][1] / 5) continue;
           		if(abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][0]/5 + spot[tcnt][0]%5) && abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][1]/5 + spot[tcnt][1]%5)) continue;
        		if(abs(mPos / 5 - mPos % 5) == abs(spot[tcnt][0]/5 - spot[tcnt][0]%5) && abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][1]/5 - spot[tcnt][1]%5)) continue;
    		}
					
			else if(countIndex == 2){
				memset(dis,0,sizeof(dis));
				
				for(int j = 0 ; j < 3 ; j++)
					dis[tcnt][j] += (abs(spot[tcnt][j]/5 - spot[tcnt][(j+1%3)]/5) + abs(spot[tcnt][j]%5 - spot[tcnt][(j+1)%3]%5));
				
				if(dis[tcnt][0] == 1 || dis[tcnt][1] == 1 || dis[tcnt][2] == 1)
					if(dis[tcnt][0] + dis[tcnt][1] + dis[tcnt][2] < 12) continue;
					
				else if(dis[tcnt][0] + dis[tcnt][1] + dis[tcnt][2] < 10) continue;
           	}
           	TriangleDFS(i+dir,countIndex+1,tcnt); //다음 점부터 검사     
   		}
	}
     
    else{   //세 점의 Select 완료된 경우          
        for(int j = 0 ; j < tcnt ; j++){
           for(int i = 0 ; i < 3 ; i++){
              if(spot[tcnt][i] != spot[j][i]) break; //중복제거 
              if(i == 2) return;
           }
       	}
		insideCnt = remainCnt = 0;
		
		if(!IsInsideOfTriangle(mPos,spot[tcnt])) return;
			
		for(int i = 0 ; i < seetCnt ; i++)
			if(map[seetPointList[i]]) remainCnt ++; 
		
		for(int t = 0 ; t < 2 ; t++){
            for(int i = 0 ; i < seetCnt ; i++){
				if(!IsInsideOfTriangle(seetPointList[i],spot[tcnt])) continue;
				if(!map[seetPointList[i]]) return;
				insideCnt ++;
				
				if(!t) continue;
				map[seetPointList[i]] --;		
			}
			
			if(insideCnt < ((remainCnt > 3)? 3 : remainCnt - (remainCnt != 1))) return;
		}
			
		TriangleDFS((dir > 0)? 0 : 15,0,tcnt+1); 
      	
      	for(int i = 0 ; i < seetCnt ; i++){
			if(!IsInsideOfTriangle(seetPointList[i],spot[tcnt])) continue;
			map[seetPointList[i]] ++;
		}
    }
}

void Rotate(int *str){
	int copy[25];
	
	for(int i = 0 ; i < 5 ; i++)
		for(int j = 0 ; j < 5 ; j++)
			copy[i*5 + j] = str[(4 - j)*5 + i];

	memcpy(str,copy,sizeof(copy));
}

int GetRotatedVal(int val, int time){
	for(int i = 0 ; i < time ; i++)
		val = (4 - val % 5)*5 + val/5;
	return val;
}


int putPos[25];

int main(void)
{


   Interface_init();

   write_gyro(0x1E, 1); // 지자기 OFF
   write_gyro(0x1E, 5); // 자이로 500dps   
   write_gyro(0x1E, 20); // 능동적 진동성분제거 OFF

   write_gyro(0x1E, 16); // 자이로 보정

   
   Camera_init();
   Setting(11);
   TM(80);
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
         _delay_ms(70);
         LED_ON(3);
         _delay_ms(100);
         LED_OFF(3);
      }
       if(SW3)
      {
         key++;
         _delay_ms(70);
         LED_ON(3);
         _delay_ms(100);
         LED_OFF(3);
      }
      
      
       if(SW2)
      {
         lcd_clear_screen();

         switch(key)
         {

case 0:
case 1:

//45벽 1-14, 8-20, 4-18, 6-16, 3-19, 5-15
//40벽 1-11, 8-15, 4-14, 6-12, 3-15, 5-11

//180


//// Home 위치로 이동 
ar ab45(30);
Drift(0,50,30,60,70);	wa=30;
Axis(0,97,0,60,0);
Ot(30,90,10,55,0); wa = 40;
Axis(-15,0,0,60,0);
Drift(-47,0,-40,50,-60);	cw //25

case 2:

ar ab134(20);


// 스캔 시작 
//=======
Axis(0,5,0,50,60);
Ot(28,-90,0,45,0);



MoveLineCheck(0,1000,0,0,40,0);
Axis(5,0,0,40,-50);
MoveLineCheck(0,-1000,0,0,20,130);

//

map[6 + 10] = Scan(151,166);

MoveLine(0,0,15,30,140);

map[6 + 5] = Scan(141,95);
map[6] = Scan(134,18);

Ot(-15,90,0,30,40);


MoveLineCheck(0,1000,0,0,40,0);
Axis(5,0,0,40,-50);
MoveLineCheck(0,-1000,0,0,20,130);

//==========

map[17] = Scan(151,166);

MoveLine(0,0,15,30,140);

map[12] = Scan(141,95);
map[7] = Scan(134,18);

Ot(-15,90,0,30,40);
MoveLineCheck(0,1000,0,0,40,0);
Axis(5,0,0,40,-50);
MoveLineCheck(0,-1000,0,0,20,130);


//=======

map[8 + 10] = Scan(151,166);

MoveLine(0,0,15,30,140);

map[8 + 5] = Scan(141,95);
map[8] = Scan(134,18);





for(int i = 0 ; i < 3 ; i++){
	lcd(i,0,"%d %d %d",map[(5 *i) + 6],map[(5 *i) + 6 + 1],map[(5 *i) + 6 + 2]);
}




Axis(40,-45,0,50,160);


ar ab865(20);
SS();


case 5:
	for(int t = 0 ;t < 4 ; t++){
		rTime = t;
		seetCnt = posCnt = flg = 0;
		
		
		
		for(int i = 0 ; i < 25 ; i++){
			if(map[i])	seetPointList[seetCnt++] = i;
			if(i / 5 == 0 || i / 5 == 4 || i % 5 == 0 || i % 5 == 4) posList[posCnt++] = i;
			if(!i || map[maxPos] < map[i]) maxPos = i;
		}
		
		min = map[maxPos] + 1;
		
		for(int i = 0 ; i < 2 ; i++){
			call = 0;
			dir = (i)? 1 : -1;
			
		   	TriangleDFS((dir > 0)? 0 : 15,0,0);
		   	
		   	if(flg == 1) goto exit;
	   	}
	   	
	   	Rotate(map);
	}

exit:

	

LED_ON(3);
_delay_ms(100);
LED_OFF(3);

lcs lcd(1,1,"FLG  : %d",flg);
lcd(3,0,"M  : %d",min);
wait


case 3:



for(int p = 0 ; p < 25 ; p++){
	for(int i = 0  ; i < min ; i++){
		for(int j = 0  ; j < 3 ; j++){
			if(p == g_spot[i][j]) 
				putPos[p] = 1;
		}
	}
}

for(int i = 0, cnt = 0  ; i < 25 ; i++){
	if(putPos[i]){
		getPuck(cnt++);
		PutPuck(i);
	}
}

GoHome();

LED_ON(3);
_delay_ms(2000);
LED_OFF(3);


lcs
lcd(0,15,"min:%d",min);

for(int i = 0 ; i < min ; i++){
	lcd(i,0,"%d %d %d",g_spot[i][0],g_spot[i][1],g_spot[i][2]);
}	


SS();




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


int Scan(int x, int y){
	SS();


	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3	
	
		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0] || pro[i][0] > 3) continue;
			if(abs(pro[i][1] - x) < 30 && abs(pro[i][2] - y) < 30) {return pro[i][0];}	
		}
	}
	return 0;
}


void PutPuck(int index){
	ar ab865(20);
	SS();
	ar ab865(20);
	SS();
	
	
	int x = (4 - (index % 5)) * 35;
	int y = (4 - (index / 5)) * 35;
	

	

	if(index == 4 || index == 9 || index == 14 || index == 19){
		CPsd(12,8,5,3,16,0,50,60);
		CPsd(10,8,y + 3,5,16,0,50,0);
		Ot(12,90,0,45,-55);

		Axis(0,-15,0,25,25);
		Ot(-5,90,10,25,0);	wa = -80;
		Ot(-5,-90,10,25,0);	wa = 20;	
		

		CPsd(12,5,-5,3,14,0,35,0); y -= 35;
		CPsd(10,5,(y > 25)? -y + 25 : 0 ,5,14,0,40,0);
		CPsd(-4,5,-1000,5,14,25,40,0);
		Ad(0,-10,-20,40,-50,30);	cw	

	}
	else if(index == 0 || index == 1 || index == 2 || index == 3){
		CPsd(12,8,10,3,18,0,60,70);
		CPsd(10,8,180 - 10 - 40,5,18,0,60,0);
		Drift(-3,40,90,50,-60);

		ar ab645(30);
		
		int spd = (index % 5 != 3)? 50 : 30;
		CPsd(12,8,10,3,18,0,spd,spd + 10);
		CPsd(10,8,x - 30,5,18,0,spd,0);
		Ot(5,90,0,spd - 10,-spd); 
	

		if(index == 3){
			Axis(0,(x > 25)? -x + 25 : 0,0,40,50);
		
			ab45(50);
			ar ab645(30);
		
			Drift(-30,1,-70,45,55);	wa = 20;
			CPsd(12,5,-5,3,14,0,60,0);
			CPsd(10,5,-130,3,14,0,60,0);
			CPsd(4,5,-1000,5,14,25,60,0);
			Ad(0,-5,-20,60,-70,30);	cw
		}
		else{
			int r = 107 - (35 * (index % 5));
			
			Axis(0,-15,0,50,60);
			Ot(-r,-90,-30,50,0);	wa = 60;

			CPsd(12,4,-5,3,14,0,50,0);
			CPsd(10,4,-20 - (35 * (index % 5)),5,14,0,50,0);
			CPsd(-3,4,-1000,5,14,25,50,0);
			Ad(0,-10,-60,50,-60,60);	cw
		}
	}
					
	else if(index >= 20 && index != 24){
		Drift(-3,44,90,40,150);	
		ar ab45(30);
		MLC();
		ar ab45(30);
		
			
		MoveLine(10,0,21 + 35 * (3 - (index % 5)),50,160);
		
		Axis(0,-15 - 35 * (3 - (index % 5)),0,40,50);
		ab45(50);
		ar ab45(30);

		Drift(-39,2,-90,30,140);	
	}
	
	
	else if(index % 5 == 0){
		Axis(-160,2,0,60,170);
		ar ab134(30);
		
		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,y + 1,5,12,0,50,0);
		Ot(12,-90,0,45,-55);

		Axis(0,-15,0,25,25);
		Ot(-5,-90,-10,25,0);	wa = 80;
		Ot(-5,90,-10,25,0);	wa = -20;	 
		
		CPsd(12,4,-5,3,16,0,35,0); y -= 35;
		CPsd(10,4,(y > 25)? -y + 25 : 0 ,5,16,0,40,0);
		CPsd(-5,4,-1000,5,16,25,40,0);
		Ad(0,-10,20,40,-50,30);	cw	
		ar ab134(30);

		Axis(160,2,0,60,170);


	}

	else{
		Axis(-10,21,0,30,140);
		Axis(0,-21,0,30,140);
		Axis(10,0,0,30,140);
	}

	SS(); ar ab865(20); SS();
}



void getPuck(int n) {
	ar ab865(30); SS();
	if(n<6) {
		
		Drift(-15,0,-10,40,50);	wa=90;
		
		Ot(-33,90,-60,40,0); wa=-70;
		if(n!=5) {
			Axis(0,-65-(20*n),0,50,-60); cw
			Setting(13);
			ar TB(1,0,tx,ty);

			Axis(0,10,0,30,40);
			Ad(-5,20,90,30,-40,60); wa=-70;
			Ot(20,-90,0,40,50); wa=20;
			Axis(0,35+(20*n),0,50,0);
	
			Ot(20,-90,-20,40,0); cw
			Axis(17,0,0,40,-50);
			ar ab865(30);
			
		}
		else {
			Drift(0,-125,-30,50,0); wa=-10;
			MovePsd(0,7,1000,0,0,25,50,0);
			Drift(12,0,10,35,-45); cw
			ar TB(1,0,tx,ty);

			Axis(0,15,0,30,40);
			Drift(-3,28,90,25,0);
			CPsd(12,8,10,3,18,0,60,70);
			CPsd(10,8,63,5,18,0,60,0);
			Ot(30,90,20,45,0); wa=-70;
			Ot(10,-90,0,40,0); wa=20;
			Axis(0,40,0,50,0);

			Ot(20,-90,-20,40,0); cw
			Axis(20,0,0,40,-50);
			ar ab865(30);		
		}	
	}

// 30 // 60 
// 28

	else{
		ar ab865(30);
		At(20,40);	wa = -70; SS();
		Axis(0,110-23,0,55,65);
		Ot(27,90,0,50,0); wa = 20;
		
		if(n != 11){
			Axis(0,-50-(20*(n%6)),0,50,0); cw
			Ad(0,-10,50,40,-50,80);	cw
			Setting(13);
			ar TB(1,0,tx,ty);

			Axis(0,10,0,30,40);
			Ad(5,20,-90,30,-40,60); wa=-20; SS();
			Ot(20,-90,0,40,50); 
			Axis(60,0,0,50,0);	wa = 70;
			
			Ot(20,90,0,50,0); wa = -20; // 35 50
			Axis(0,20 + (20 * (n % 6)),0,60,0);
			
			Ot(20,-90,0,50,0);	
			Drift(17,0,20,45,-55);	cw
			
			ar ab865(30);
			
		}
		else{
			Axis(0,-125,0,60,0);
			MovePsd(0,4,0,-1000,0,25,60,0);
			Ad(0,-12,70,50,-60,80);		cw
			ar TB(1,0,tx,ty);

			Axis(0,15,0,30,40);
			Drift(3,28,-90,25,0);
			CPsd(12,1,10,3,16,0,60,70);
			CPsd(10,1,60,5,16,0,60,0);
			Ot(20,-90,0,45,0); wa = 90;
			Drift(0,20,-20,40,0);	wa =70;
			Axis(0,60,0,50,0);	
			Ot(20,90,0,40,0); wa=-20;
			Axis(0,40,0,50,0);
			Ot(20,-90,0,40,0);
			Drift(17,0,20,30,-40);	cw
				
				
		}
	}

	ar ab865(30);
	SS();

}

//3
void GoHome(int index){
	//index != 20일때만 
	
	ar ab865(30);
	
	
	Drift(-20,0,20,30,40);	wa = 20;
	Axis(-26,0,0,40,0);		wa = -70;
	Ot(20,90,0,35,0);		wa= 20;
	
	MoveLineCheck(0,0,-1000,0,50,0);
	Axis(0,-8,0,50,-60);

	LED_ON(3);
	_delay_ms(2000);
	LED_OFF(3);

}

void MLC(){
	MoveLineCheck(0,-2,0,0,10,120);
	MoveLineCheck(0,4,0,0,10,120);
	MoveLineCheck(0,-6,0,0,10,120);
	MoveLineCheck(0,8,0,0,10,120);
	MoveLineCheck(0,-10,0,0,10,120);
	MoveLineCheck(0,12,0,0,10,120);
	MoveLineCheck(0,-14,0,0,10,120);
	MoveLineCheck(0,16,0,0,10,120);

}

void DIS(){
	lcs
	for(int i = 0 ; i < 4 ; i++){
		lcd(i,0,"%d %d %d %d %d",map[i*5 + 0],map[i*5 + 1],map[i*5 + 2],map[i*5 + 3],map[i*5 + 4]);
	}
	wait		
}
