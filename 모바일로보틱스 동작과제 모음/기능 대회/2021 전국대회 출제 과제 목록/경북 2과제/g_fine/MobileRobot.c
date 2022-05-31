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



#define ab45    wa = 90; AB(0,0,4,212,5,177,50,0); cw
//#define ab845(y) wa = 90; AB(8,150,4,184,5,167,50,y); cw
//#define ab145(y) wa = 90; AB(1,190,4,184,5,167,50,y); cw
#define ab245(y) wa = 90; AB(2,214,4,212,5,177,50,y); cw
#define ab745(y) wa = 90; AB(7,167,4,212,5,177,50,y); cw

#define ab345(y) wa = 90; AB(2,100,4,212,5,177,50,y); cw
#define ab745(y) wa = 90; AB(7,160,4,212,5,177,50,y); cw

#define ab86 AB(0,0,6,136,8,112,30,0);
#define ab13 AB(0,0,1,104,3,73,30,0);

#define ab865(y) AB(5,195,6,173,8,125,30,y); cw
#define ab134(y) AB(4,188,1,165,3,158,30,y); cw

int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
#define M 50

int gate = 0;
int node[8];
int map[8] = {0
};

int mapGoal[8] = {0
};
int afterMapGoal[8] = {0};

int g_route[M],route[M],link[8][4],min,flg = 0,check[8];
int history[M][8],dir,nowLine = 0,getCheck[4] = {0};
int null[2];


void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 2  > 0) str[0] = p-2;
   if(p % 2  > 0) str[1] = p-1;
   if(p / 2  < 3) str[2] = p+2;
   if(p % 2 == 0) str[3] = p+1;
}

int Sorted(){
   for(int i = 0 ; i < 8 ; i++){
      if(node[i] <= 0) continue;
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}

int InHistory(int *list,int dp){
   for(int i = 0 ; i < dp ; i++){
      for(int j = 0 ; j < 8 ; j++){
         if(node[j] != history[i][j]) break;
         if(j == 7) return 1;
      }
   }
   return 0;
}

void Sort(int lp, int p, int dp ,int *node){
   if(dp >= min || flg) return;
   
   if(InHistory(node,dp)) return;
   
   route[dp] = p;

   if(Sorted()){
      min = dp;
      flg= 1;
      memcpy(g_route,route,sizeof(int) * (min + 1));
      return;
   }
   
   for(int i = 0 ; i < 4 && !flg; i++){
      int np = link[p][i];
      if(np < 0 || np == lp) continue;
      
      swap(node[p],node[np]);
      Sort(p,np,dp+1,node);
      swap(node[p],node[np]);
   }
}



int GetDir(int p, int np){
	for(int i = 0  ; i < 4;  i++)
		if(link[p][i] == np) return i;
}

int GetAng(int wd){
	int rd = wd - dir;

	if(abs(rd) == 3) rd/=-3;

	return rd * 90;
}


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

// 2 1 3
// 8 6

case 0:
case 1:

   Axis(2,2,0,10,120);
   ar ab245(20);	SS();
   At(-25,40);	wa = -25;
wait

	bar_2= 0;
	for(int i = 0 ; i < 4 ; i++){		
	
		if(T(101,3)){
			map[6 - (2*i)] = Scan(211,103);
			mapGoal[6 - (2*i)] = Scan(49,163);
		}

		if(i != 3){
			CPsd(12,2,5,3,8,0,40,50);
			CPsd(10,2,55,5,8,0,40,-50);
		}
		else{
			CPsd(12,2,5,3,8,0,40,50);
			CPsd(10,2,20,5,8,0,40,0);
			Drift(3,35,-65,40,-50);	cw
		}
	
		if(!mapGoal[6 - (2*i)]) afterMapGoal[6 - (2 * i)] = bar_2 - (i == 0);
		display();
	}


	CPsd(12,1,5,3,12,0,50,60);
	CPsd(10,1,105,5,12,0,50,0);
	Drift(3,40,-90,50,-60);
	ar ab245(30);
	
	At(-25,60);	wa = -25;
	for(int i = 0 ; i < 4 ; i++){		
		bar_2 = 0;
	
	if(T(101,3)){
		map[1 + (2*i)] = Scan(211,103);
		mapGoal[1 +  (2*i)] = Scan(49,163);
	}
		
		if(i != 3){
			CPsd(12,2,5,3,8,0,40,50);
			CPsd(10,2,55,5,8,0,40,-50);
		}
		else{
			CPsd(12,2,5,3,8,0,40,50);
			CPsd(10,2,20,5,8,0,40,0);
			Drift(3,35,-65,40,-50);	cw
		}
		
		if(!mapGoal[1 + (i * 2)]) afterMapGoal[1 + (i * 2)] = (bar_2 != 3)? 3 - bar_2 : 3;
		bar_2 = 0;
		display();
	}
	
	ar ab245(30);

display();

case -1:
if(key == -1){
	memset(map,0,sizeof(map));
	memset(afterMapGoal,0,sizeof(map));	
	memset(mapGoal,0,sizeof(map));
	map[0] = 1;
	map[1] = 1;
	map[2] = 2;
	map[3] = 2;
	map[4] = 3;
	map[5] = 3;
	mapGoal[0] = 3;
	mapGoal[1] = 3;
	mapGoal[4] = 1;
	afterMapGoal[3] = 1;
	mapGoal[5] = 2;
	mapGoal[6] = 2;
	mapGoal[7] = 1;
	afterMapGoal[7] = 2;
}


for(int i = 0 ,cnt = 0,val = 0; i < 8 ; i++){
  Link(i,link[i]);
  if(!map[i]) {
     null[cnt++] = i;
     map[i] = val--;
  }
}

for(int i = 0; i < M ; i+=2){
	min = i;
	flg = 0;
	memcpy(node,map,sizeof(map));
	for(int j = 0 ; j < 2 ; j++){
		map[null[j]] = -1;
		Sort(-1,null[j],0,map);		
		map[null[j]] = 0;
		if(flg) goto exit;
	}
	
}
exit:
LED_ON(3);
_delay_ms(200);
LED_OFF(3);

lcd(3,0,"MIN : %d | FLG : %d",min,flg);


GetIn(g_route[0]);
for(int i = 0 ; i < min ; i++){
  lcs
  lcd(0,0,"%d %d",map[0],map[1]);
  lcd(1,0,"%d %d",map[2],map[3]);
  lcd(2,0,"%d %d",map[4],map[5]);
  lcd(3,0,"%d %d P%d NP%d",map[6],map[7],g_route[i],g_route[i+1]);


  MovePtoP(g_route[i],g_route[i+1],map,link);
}

SS();
LED_ON(3);
_delay_ms(1000);
LED_OFF(3);


GetOut(g_route[min]);

//


case 4:

gate = -1;
	
ar ab245(30);

set

Avoid(0,14,18,0,50,50,60);

if(g_psd[2] > 30)	gate = 2;

if(gate < 0) {
	Avoid(0,14,18,0,10,50,0); 
	Drift(0,10,-20,40,-50); 	wa= -20;
	if(g_psd[2] > 30) gate = 1;
	else gate = 0;
}
else {
	Drift(0,10,-20,40,-50);	wa = -20;
}

Avoid(5,14,18,0,-pos[1] + 10,40,50);
Drift(0,-10,20,40,-50);	cw

lcs
lcd(1,1,"gate; %d",gate);


for(int i = 0; i < 8 ; i++){
	if(afterMapGoal[i]){
		GoC(gate,nowLine);
		GetPuck(afterMapGoal[i]);
		GoA(gate,i%2); 
		PutPuck(i);
	}
}

if(nowLine == 1){
	ar ab245(30);
	Avoid(0,12,16,0,115,50,60);
	Drift(3,40,-90,40,-50);
	ar ab245(30);
}

SS();
LED_ON(3);
_delay_ms(2000);
LED_OFF(3);







while(!SW2);
while(SW2);

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
		key_puck = abs(key) %9;

		get_gyro();
		lcd(2,0,"C%d X%3d Y%3d S%2d",key_puck,Cmd(key_puck,102),Cmd(key_puck,103),Cmd(key_puck,104));		
		lcd(3,0,"%d%d%d%d%d|%d%d G%04d",IR_4,IR_1,IR_2,IR_3,IR_5,IND_1,IND_2, (int)gyro);
		lcd(3,16,"k:%02d",key);
	}
}


void GetIn(int n){
	ar ab245(20);

	if(n % 2 == 0){
		ar ab245(20);
		Avoid(0,12,16,0,115,50,60);
		Drift(3,43,-90,40,-50);
		ar ab245(30);

		if(n == 6)
			Axis(38,33,0,40,150);
		else{
			CPsd(12,1,5,3,12,0,50,60);
			CPsd(10,1,60 * (3 - n/2) - 8,5,12,0,50,0);
			Ot(35,-90,0,40,-50);
		}

		dir = 0;
	}

	else{
	
		At(-90,100);
		//ar ab745(30);

		if(n == 7){
			Axis(-35,35,0,40,150);
		}
		else{
			CPsd(12,8,5,3,16,0,50,60);
			CPsd(10,8,60 * (3-n/2) - 5,5,16,0,50,0);
			Ot(35,90,0,40,-50); 
		}
		dir = 0;
	}
}

void MLC(){
	MoveLineCheck(0,2,0,0,10,120);
	MoveLineCheck(0,-4,0,0,10,120);
	MoveLineCheck(0,6,0,0,10,120);
	MoveLineCheck(0,-8,0,0,10,120);
	MoveLineCheck(0,10,0,0,10,120);
	MoveLineCheck(0,-12,0,0,10,120);
	MoveLineCheck(0,14,0,0,10,120);
}

void display(){
	lcs

	lcd(0,0,"%d %d|%d %d|%d %d",map[0],map[1],mapGoal[0],mapGoal[1],afterMapGoal[0],afterMapGoal[1]);
	lcd(1,0,"%d %d|%d %d|%d %d",map[2],map[3],mapGoal[2],mapGoal[3],afterMapGoal[2],afterMapGoal[3]);
	lcd(2,0,"%d %d|%d %d|%d %d",map[4],map[5],mapGoal[4],mapGoal[5],afterMapGoal[4],afterMapGoal[5]);
	lcd(3,0,"%d %d|%d %d|%d %d",map[6],map[7],mapGoal[6],mapGoal[7],afterMapGoal[6],afterMapGoal[7]);
	
}


int Scan(int x, int y){
	SS();
	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3
	
	
		for(int i = 0 ;  i < 9 ; i++){
			if(!pro[i][0]) continue;
			if(abs(pro[i][1] - x) < 30 && abs(pro[i][2] - y) < 30) return pro[i][0];
		}
	}
	return 0;
}

void SwapMove(int p, int wp, int *node){
	
	
	MoveLine(1,0,1000,40,50);
	Od2(0,25,-180,40,-50,100,120);
	MLC();

	set
	MoveLine(10,0,20 + (dir%2) * 30,40,50);
	MoveLine(2,0,30,40,-50);

	Axis(0,-pos[1],0,30,140);	
	MLC();

	dir  = (dir + 2) % 4;

}

void MovePtoP(int p, int np, int *node, int link[][4]){
	int ndir = GetDir(p,np);	
	
	Axis(0,0,GetAng(ndir),150,130);
	MLC();
	dir = ndir;
	
	if(node[np] > 0)
		SwapMove(p,np,node);

	else {
		MoveLine(1,0,1000,40,50);
		Axis(0,25,0,40,-50);
	}

	swap(node[p],node[np]);
}	

void GetOut(int point){
	if(point % 2) nowLine = 1;
	else nowLine = 0;

	if(point < 6){
		Axis(0,0,GetAng(0),100,120);
		
		if(point % 2){
			wa = 90;
			Ot(35,-90,20,40,50);	wa = 20;
			CPsd(12,5,-5,3,14,0,50,0);
			CPsd(10,5,-60 * (3-(point / 2)) + 5,5,14,0,50,-60);
			At(70,100);	cw
			Axis(2,2,0,10,120);
			ar ab245(30); SS();
		}
		else{
			wa = -90;
			Ot(35,90,-20,30,40);	wa = -20;
			CPsd(12,4,-5,3,18,0,50,0);
			CPsd(10,4,-60 * (3-(point / 2)) + 30,5,18,0,50,0);
			CPsd(-5,4,-1000,5,18,20,50,0);
			Ad(0,-12,-70,50,-60,100);
			Axis(-2,2,0,10,120);
			ar ab745(30); SS();
		}
	}

	else if(point == 6){
		Axis(0,0,GetAng(3),100,120);	wa = 90;
		Axis(32,-32,0,30,140);		
		Axis(-2,2,0,10,120);
		ar ab745(30);
	
	}
	else{
		Axis(0,0,GetAng(1),100,120);
		Axis(-32,-32,0,30,140);
		Axis(2,2,0,10,120);
		ar ab245(30);
	}

	SS();
}

void GoC(int gate, int line){
	
	if(line == 0){
		ar ab745(30);
		if(gate > 0){
			CPsd(12,8,3,3,18,0,40,50);
			CPsd(10,8,10,5,18,0,40,0);
			CPsd(8,8,1000,3,18,25,40,0);
			Drift(2,44,-90,35,-45);	
		}
		else{
			Drift(2,48,-90,40,150);	
		}
	}
	
	else if(line == 1){
		ar ab245(30);
		if(gate < 2){
			CPsd(12,1,3,3,14,0,40,50);
			CPsd(10,1,10,5,14,0,40,0);
			CPsd(1,1,1000,3,14,25,40,0);
			Drift(-2,38,90,35,-45);	
		}
		else{
			Drift(-2,48,90,40,150);	
		}
	}

	Avoid(0,10,15,0,40,40,50);
	Ot(10,90,10,40,0);	wa = -80;
	
	CPsd(12,3,5,3,14,0,50,0);
	CPsd(10,3,10,5,14,0,50,0);
	CPsd(-2,3,1000,3,14,20,50,0);
	Drift(0,10,-10,50,-60);	cw
	
	Axis(2,2,0,10,120);
	
	ar ab245(30);
	
}

void GoA(int gate, int line){
	
	ar ab745(20);
	
	if(gate < 2){
		CPsd(12,8,3,3,18,0,40,50);
		CPsd(10,8,10,5,18,0,40,0);
		CPsd(8,8,1000,3,18,25,40,0);
		Drift(2,44,-90,35,-45);	
	}
	else{
		Drift(2,48,-90,40,150);	
	}
	
	Avoid(0,10,15,0,25,40,50);
	
	if(line == 0){
		Drift(0,20,60,25,0);		wa = 60;
		Ot(10,90,0,35,0);	cw	wa = -30;
		
		CPsd(12,2,5,3,12,0,40,0);
		CPsd(10,2,5 + 30 * (gate),5,12,0,40,0);
		CPsd(-1,2,1000,5,12,28,40,0);
		Drift(3,23,-60,35,-45);	cw
		Axis(2,2,0,10,120);
		ar ab245(30);
	}
	else{
		Drift(0,20,-60,25,0);		wa = -60;
		Ot(10,-90,0,35,0);	cw	wa = 30;
		
		CPsd(12,7,5,3,16,0,40,0);
		CPsd(10,7,5 + 30 * (2 - gate),5,16,0,40,0);
		CPsd(-8,7,1000,5,16,28,40,0);
		Drift(-3,23,60,35,-45);	cw
		Axis(-2,2,0,10,120);
		ar ab745(30);
	}

	nowLine = line;
}


void GetPuck(int wantColor){
	Setting(12);
	V3
	set
	
	ar ab245(20);
	int index;
	for(int i = 0 ; i < 4 ; i++){
		
		MoveLineCheck(0,1000,0,0,40,50 * (i == 0));

		if(T(-103,3)== wantColor && !getCheck[i]){
			Axis(5,0,0,40,-50);
			MoveLineCheck(0,-10,0,0,10,120);

			MoveLine(10,0,25,40,50);
			Od2(-5,10,-180,40,-50,60,130);		
			index = i;
			getCheck[i] = 1;
			break;
		}
		else{
			Axis(10,0,0,40,0);
		}
	}

	if(index == 0){
		MovePsd(0,7,1000,0,0,10,40,50);
		Axis(5,0,0,40,-50);

		Drift(-3,35,90,20,130); 
		Axis(-2,2,0,10,120);
		ar ab745(30);
	}
	else{
		Ot(10,-90,-10,40,50);	wa = -10;
		Axis(40 * index - 20,0,0,40,0);
		MovePsd(0,7,1000,-50,0,10,40,0);
		Axis(5,0,0,40,-50);
		
		Axis(0,0,90,60,130);	wa =-10;
		Drift(20,0,10,30,140);	cw
		Axis(-2,2,0,10,120);
		ar ab745(30);
	}

}

void PutPuck(int index){
	if(index % 2== 0){
		ar ab245(30);
	}
	if(index % 2){
		ar ab745(30);
	}


	if(index == 6){
		At(-40,60);
		wa = -40;
		
		Axis(17,17,0,30,140);
		Axis(-17,-17,0,30,140);
		At(-50,60);	cw
	}
	
	else if(index == 7){
		At(40,60);
		wa = 40;
		Axis(-17,17,0,30,140);
		Axis(17,-17,0,30,140);
		At(50,60);	cw
	}

	else if(index % 2 == 0){
		
		ar ab245(30);
		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,60 * (3 - index / 2) - 10,3,12,0,50,0);
		Ot(20,-90,-45,40,-50);	
		
		Axis(0,-18,0,35,45);
		Ot(-10 ,45,0,35,0);	wa = -45;
		Drift(0,-10,25,40,0);	wa = -20;

		CPsd(12,4,-5,3,16,0,50,10);
		CPsd(10,4,-60 * (3 - index / 2) + 30,3,16,0,50,0);
		CPsd(-5,4,-1000,5,16,22,50,0);
		Ad(0,-10,-70,50,-60,100);	cw	SS();
	}
		
	else{
		ar ab745(30);

		CPsd(12,8,5,3,16,0,50,60);
		CPsd(10,8,60 * (3 - index / 2) - 10,3,16,0,50,0);
		Ot(20,90,45,40,-50);	
		
		Axis(0,-18,0,35,45);
		Ot(-10 ,-45,0,35,0);	wa = 45;
		Drift(0,-10,-25,40,0);	wa = 20;

		CPsd(12,5,-5,3,12,0,50,10);
		CPsd(10,5,-60 * (3 - index / 2) + 30,3,12,0,50,0);
		CPsd(-4,5,-1000,5,12,22,50,0);
		Ad(0,-10,70,50,-60,100);	cw SS();
	}


	if(index % 2 == 0){
		Axis(-2,2,0,10,120);
		ar ab745(20);
	}
	else if(index % 2){
		Axis(2,2,0,10,120);
		ar ab245(20);
	}
}
