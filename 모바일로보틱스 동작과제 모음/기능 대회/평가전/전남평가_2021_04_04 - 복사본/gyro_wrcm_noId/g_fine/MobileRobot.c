/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"
int cnt2;
int index = 0,nd = 0;
#define cw wa = 0;
#define ar acf =cacf= 0;
#define set pos[0] = pos[1] = pos[2] = 0; gyro = rearGyro = 0;
#define V3 while(!Cmd(0,19));
#define SV1 while(!Cmd(0,18));
#define reset bar_1 = bar_2 = bar_3 = bar_4 = bar_5 = sbar_1 = sbar_2= 0;
#define wait if(key) {SS(); while(SW2); while(!SW2);}
#define lcs lcd_clear_screen();

#define ab45(y) wa = 90; AB(0,0,4,196,5,170,30,y);   cw
#define ab345(y) wa = 90; AB(3,122,4,196,5,170,30,y);   cw
#define ab645(y) wa = 90; AB(6,150,4,196,5,170,30,y);   cw
#define ab13(y) AB(0,0,1,165,3,157,30,y);   

int key= 0, key_puck = 0;
int movePoint = 0;
int tmp = 0,bgate = -1;
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;
#define M 50
int p,movePoint;
int map[9] = {0
};
int mapGoal[9] = {0
};
int node[9];

int flg = 0, min;
int route[M],g_route[M];
int link[9][4] = {0};

int arr[9] = {0,1,2,5,4,3,6,7,8};

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   if(p / 3 - 1 >= 0) str[0] = p-3;
   if(p % 3 - 1 >= 0) str[1] = p-1;
   if(p / 3 + 1 < 3) str[2] = p+3;
   if(p % 3 + 1 < 3) str[3] = p+1;
}

int Sorted(){
   for(int i = 0 ; i < 9 ; i++)
      if(node[i] != mapGoal[i]) return 0;
   return 1;
}

void Sort(int lp, int p, int dp, int *node){
   if(dp >= min  || flg) return;
   
   route[dp] = p;
   
   if(Sorted()){
      memcpy(g_route,route,sizeof(int) * (dp + 1));
      min = dp;
      
      flg = 1;
      return;
   }
   
   for(int i = 0 ; i < 4;  i++){
      int np = link[p][i];
      if(np < 0 || np == lp) continue;
      
      swap(p,np);
      Sort(p,np,dp+1,node);
      swap(p,np);
   }
   return;
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

case 0:
case 1:

p = 0;
for(int i = 0  ; i < 5 && !p ; i++){
	p = T(101,3);
}

if(p){
	At(15,20);	wa= 15;

	CPsd(12,5,-5,3,11,0,40,50);
	CPsd(10,5,-5,5,11,0,40,0);
	CPsd(5,5,-1000,3,11,0,40,0);

	Axis(0,-30,0,40,0);
	Ot(-10,90,0,35,0);
	Drift(20,0,60,30,0);	wa= -15;
	
	Avoid(5,11,17,40,-1000,40,0);
	CPsd(12,4,-5,3,11,0,40,0);
	CPsd(10,4,-5,5,11,0,40,0);
	CPsd(-5,4,-1000,5,11,20,40,0);
	Ad(0,-10,15,40,-50,30);	cw

	ar ab345(30);

	Avoid(0,12,16,0,79,40,50);
	movePoint = 1;
}

else{
	CPsd(12,1,5,3,11,0,40,50);
	CPsd(10,1,5,5,11,0,40,0);
	CPsd(1,1,1000,3,11,20,40,0);

	Ad(2,38,-90,40,-50,80);	
	movePoint = 0;
}

for(int i = 0 ; i < 20 ; i++){
	bgate = -1;
	if(g_psd[1] > 15) bgate = 2;
	else if(g_psd[2] > 10) bgate = 1;
	else if(g_psd[3] > 20) bgate = 0;

	lcs lcd(1,1,"bgate : %d",bgate);

	
	if(bgate >= 0)	{if(movePoint) Axis(0,10,0,40,-50); break;}

	CPsd(12,1,5,3,12,0,40,50 * !movePoint);
	CPsd(10,1,70 - 5 - 35,5,12,0,40,0);
	Drift(2,33,-90,35,-45);

	ar ab345(30);

	
	Avoid(0,12,16,0,79,40,50);
	movePoint = 1;
}


Drift(0,-10,15,30,40);	wa= 15;
Avoid(4,11,17,40,-1000,40,0);
CPsd(12,3,-5,3,17,0,40,0);
CPsd(10,3,-5,5,17,0,40,0);
CPsd(-4,3,-1000,5,17,20,40,0);
Ad(0,-10,-15,40,-50,30);	cw


ar ab345(30);



wait





//Scan

case 3:
case 4:
case 5:
if(key == 3) bgate=  0;
if(key == 4) bgate=  1;
if(key == 5) bgate=  2;


ar ab345(30);

Ad(0,25,-10,30,140,20);	wa= -10;

map[0] = Scan(0,0);
map[0] = Scan(0,0);

Ad(0,32,-80,30,140,100);	cw

ar ab45(30);

Ad(0,40,50,30,140,80);	

map[1] = Scan(0,0);



At(-10,20);

map[2] = Scan(0,0);
map[4] = Scan(0,0);

At(-20,40);	

map[5] = Scan(0,0);

At(-40,60);

map[7] = Scan(0,0);
map[8] = Scan(0,0);


wa = -20;

Axis(0,110-40,0,40,50);
Ad(1,42,-70,35,-45,100);	cw

ar ab13(30);


CPsd(12,1,5,3,14,0,30,40);
CPsd(10,1,10,5,14,0,30,-40);


mapGoal[8] = Scan(0,0);
mapGoal[7] = Scan(0,0);
mapGoal[6] = Scan(0,0);

if(bgate != 2)	
	mapGoal[5] = Scan(0,0);


if(bgate == 0){
	Axis(0,30,0,30,140);
	mapGoal[4] = Scan(0,0);


	Axis(0,70,0,40,150);
}

else if(bgate == 1){
	Axis(0,70,0,40,150);
	mapGoal[3] = Scan(0,0);
	Axis(0,30,0,30,140);
}

else{
	Axis(0,30,0,30,140);

	mapGoal[4] = Scan(0,0);

	Axis(0,30,0,30,140);

	mapGoal[3] = Scan(0,0);
	
	Axis(0,40,0,40,150);
}

mapGoal[2] = Scan(0,0);
mapGoal[1] = Scan(0,0);
mapGoal[0] = Scan(0,0);

Ad(0,-10,-15-15,30,140,60);	wa = -15;

map[0] = Scan(0,0);

Axis(0,30,0,40,50);
Drift(3,35,-75,35,-45);

ar ab345(30);


//wait

lcs 
lcd(1,1,"%d %d %d / %d %d %d",map[index],map[index+1],map[index+2],mapGoal[index],mapGoal[index+1],mapGoal[index+2]); index += 3;
lcd(2,1,"%d %d %d / %d %d %d",map[index],map[index+1],map[index+2],mapGoal[index],mapGoal[index+1],mapGoal[index+2]); index += 3;
lcd(3,1,"%d %d %d / %d %d %d",map[index],map[index+1],map[index+2],mapGoal[index],mapGoal[index+1],mapGoal[index+2]);

wait







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
	for(int i = 0 ; i < 5 ; i++){
		key_puck = 3;
		SV1
		V1();
		V3
		
	

		for(int k = 0; k  < 9 ; k++){
			if(!pro[k][0]) continue;
			lcs lcd(2,0,"C%d X%3d Y%3d S%2d",key_puck,Cmd(key_puck,102),Cmd(key_puck,103),Cmd(key_puck,104));      wait
			if(abs(pro[k][1] - x) < 20 && abs(pro[k][2] - y) < 20) return pro[k][0];
		}
	}

	return 0;
}

void GetPuck(){
	ar ab345(30);

	int puck = 0, gate = -1;

	Drift(0,35,-20,30,40);	

	p = 0;
	for(int i = 0 ; i < 5 && !p ; i++)
		p = T(101,3);

	if(p) gate = 2;

	Drift(1,42,-70,30,0);

	CPsd(12,1,5,3,12,0,40,50);
	CPsd(10,1,5,5,12,0,40,0);
	CPsd(1,1,1000,3,12,20,40,0);
	

	if(!p){
		for(int i = 0 ; i < 5 && !p ; i++)
			p = T(101,3);

		if(p) gate = 0;

	}

	if(!p){
		Ad(0,21,-70,40,-50,90);	wa= -70;

		for(int i = 0 ; i < 5 && !p ; i++)
			p = T(101,3);

		if(p) gate = 3;
		else gate = 1;
	}

	if(gate == 0){
		Avoid(0,11,15,0,45,40,0);
		CPsd(12,1,5,3,12,0,40,0);
		CPsd(10,1,5,5,12,0,40,0);
		CPsd(1,1,1000,3,12,20,40,0);

		Drift(2,42,-90,30,0);

		CPsd(12,1,5,3,12,0,40,50);
		CPsd(10,1,75-5-35,5,12,0,40,0);
		Drift(2,32,-90,30,0);

		CPsd(12,1,5,3,12,0,40,50);
		CPsd(10,1,140-5-35,5,12,0,40,0);
		Drift(2,32,-90,30,-40);

		ar ab345(30);
	}
	else if(gate == 1){
		At(140,100);	wa =70;

		Drift(-2,18,20,30,40);	cw
		CPsd(12,8,5,3,16,0,40,0);
		
		CPsd(10,8,5,5,16,0,40,0);
		CPsd(8,8,1000,3,16,25,40,0);

		Drift(2,41,-90,35,0);

		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,70-5-35,5,12,0,50,0);
		Drift(2,32,-90,35,0);

		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,150-5-35,5,12,0,50,0);
		Drift(2,34,-90,45,0);

		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,140-5-35,5,12,0,50,0);
		Drift(2,32,-90,45,-55);


		ar ab345(30);
	}
	

	else{
		if(gate == 2)	Drift(2,39,-90,30,0);
		else Drift(0,18,-20,25,30);	cw

		CPsd(12,1,5,3,12,0,40,0);
		CPsd(10,1,5,5,12,0,40,0);
		CPsd(1,1,1000,3,12,20,40,0);

		Drift(2,37,-90,30,0);

		CPsd(12,1,5,3,12,0,40,50);
		CPsd(10,1,70-5-35,5,12,0,40,0);
		Drift(2,32,-90,30,-40);

		ar ab345(30);		
	}
}

int GetDir(int np, int wp){
	for(int i = 0 ; i < 4 ; i++){
		if(link[np][i] == wp) return i;
	}
}
int GetAng(int d, int wd){
	int rd = wd-  d;

	if(abs(rd) == 3){
		rd/=-3;
	}

	return rd * 90;
}

void MovePoint(int p, int wp){
	int wd = GetDir(p,wp);
	if(nd != wd) Axis(0,0,GetDir(nd,wd),60,130);

	MLC();
	nd = wd;
	MoveLine(2,0,1000,40,50);
	Od2(0,24,-180,40,-50,60,130);

	MLC();
	set
	MoveLine(2,0,1000,30,40);
	Axis(0,1,0,30,-40);
	
	Axis(0,-pos[1],0,30,140);


	nd = (nd + 2) % 4;
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

void MoveA() {
	ar ab45(30);
	ar ab345(30);

	if(bgate==0) {
		Drift(-3,47,90,40,150);
		Avoid(0,11,15,0,15,40,50);
		Drift(0,20,40,40,0); wa=40;
		Ot(20,90,40,35,0); wa=-10;
		Drift(3,27,-80,35,-45); cw
	}
	else {
		CPsd(12,1,5,3,12,0,45,55);
		CPsd(10,1,5,5,12,0,45,0);
		CPsd(1,1,1000,3,12,22,45,0);
		Drift(-3,36,90,40,-50);
		Avoid(0,11,15,0,15,40,50);
		Drift(0,20,40,40,0); wa=40;
		Ot(20,90,50,35,0); cw
		CPsd(12,1,10,3,12,0,45,0);
		CPsd(10,1,(bgate==1)? 10:40,5,12,0,45,0);
		Drift(5,37,-90,40,-50);
	}

	ar ab45(30);
	ar ab345(30);
}

void MoveC() {
	ar ab45(30);
	ar ab345(30);

	if(bgate==0) {
		Drift(25,-1,70,20,130); wa=-27;
		Ot(-20,-90,-40,25,35); wa=30;
		Drift(0,-16,-30,25,0); cw
		Ot(-20,-90,-30,25,0); wa=-30;
		Drift(-28,0,-62,30,-40); cw
	}
	else {
		Drift(25,-1,70,20,130); wa=-20;
		CPsd(12,4,-3,3,16,0,35,45);
		CPsd(10,4,-3,5,16,0,35,0);
		CPsd(4,4,-1000,3,16,26,35,0);
		Axis(0,-18,0,35,0); wa=-27;
		Ot(-22,-90,-40,25,0); wa=30;
		Drift(0,-13,-30,25,0); cw
		Ot(-20,-90,-30,25,0); wa=-30;
		Drift(-25,0,-40,30,0); wa=20;
		CPsd(12,3,-5,3,16,0,40,0);
		CPsd(10,3,-5,5,16,0,40,0);
		CPsd(-4,3,-1000,3,16,25,40,0);
		Drift(0,-12,-20,40,-50); cw
	}
	ar ab45(30);
	ar ab345(30);
}

void GetIn(int n) {
	ar ab45(30);
	ar ab345(30);

	if(n==0) {
		CPsd(12,1,50,3,12,0,50,60);
		CPsd(10,1,55+cm,5,12,0,50,0);
		Ot(25,-90,0,45,-55);
		nd=0;
	}
	else if(n==3) {
		CPsd(12,1,20,3,12,0,50,60);
		CPsd(10,1,30+cm,5,12,0,50,0);
		Ot(25,-90,0,45,-55);
		nd=0;
	}
	else if(n==6) {
		Axis(0,4+cm,0,40,50);
		Ot(25,-90,0,35,-45);
		nd=0;
	}
	else if(n==1) {
		CPsd(12,1,50,3,12,0,50,60);
		CPsd(10,1,55+cm,5,12,0,50,0);
		Drift(5,45,-90,45,-55);

		ar ab345(30);
	}
}
