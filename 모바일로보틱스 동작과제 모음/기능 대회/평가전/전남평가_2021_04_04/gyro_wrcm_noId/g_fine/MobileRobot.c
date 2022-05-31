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

if(T(102,3)){
   At(15,30);   wa = 15;
   CPsd(12,5,-5,3,13,0,40,50);
   CPsd(10,5,-5,5,13,0,40,0);
   CPsd(5,5,-1000,3,13,25,40,0);
   Axis(0,-30,0,40,0);
   Ot(-10,90,0,30,0);
   Drift(20,0,60,30,0);   wa = -15;

   Avoid(5,11,16,40,-1000,40,0);
   CPsd(12,4,-5,3,17,0,40,0);
   CPsd(10,4,-5,5,17,0,40,0);
   CPsd(-5,4,-1000,3,17,20,40,0);
   Ad(0,-10,20,40,-50,20);   cw

   ar ab345(30);

   Avoid(0,11,15,0,79,40,50);
   movePoint = 1;
}

else{
   CPsd(12,1,5,3,12,0,40,50);
   CPsd(10,1,5,5,12,0,40,0);
   CPsd(1,1,1000,3,12,20,40,0);

   Drift(2,38,-90,30,-40);
   
   wait
}

for(int i = 0 ; i < 20 ; i++){
   bgate = -1;
   if(g_psd[1] > 18) bgate = 2;
   else if(g_psd[2] > 10) bgate = 1;
   else if(g_psd[3] > 15) bgate = 0;

   if(bgate >= 0){
      if(movePoint) Axis(0,10,0,40,-50);
      break;
   }

   CPsd(12,1,5,3,12,0,40,50 * !movePoint);
   
   CPsd(10,1,25,5,12,0,40,0);
   Drift(3,35,-90,40,-50);
   
   ar ab345(30);

   Avoid(0,11,15,0,79,40,50);

   movePoint = 1;
}


Drift(0,-10,15,30,40);   wa = 15;
Axis(0,-30,0,40,0);
CPsd(12,3,-5,3,17,0,40,0);
CPsd(10,3,-5,5,17,0,40,0);
CPsd(-4,3,-1000,3,17,20,40,0);

Ad(0,-10,-15,40,-50,20);   cw

ar ab345(30);



MoveA(bgate);


case 3:
if(key == 3) bgate=  2;

ar ab345(30);
Ad(0,25,-10,30,140,20);   wa = -10;

//wait

map[0] = Scan(139,14);
map[3] = Scan(210,108);


case 4:
Ad(2,32,-80,30,140,80);   cw

ar ab45(30);

Ad(0,40,50,40,150,60);   

map[1] = Scan(27,56);

case 6:
At(-10,20);

map[2] = Scan(116,10);
map[4] = Scan(164,167);

case 7:
At(-20,40);


map[5] = Scan(152,37);

case 12:
At(-40,60);   wa = -20;


case 8:
map[7] = Scan(219,168);
map[8] = Scan(101,39);




case 9:
//150
Axis(0,110-40,0,40,50);
Drift(1,42,-70,30,-40);   cw
ar ab13(30);

CPsd(12,1,5,3,12,0,30,40);
CPsd(10,1,10,3,12,0,30,-40);

wait
mapGoal[6] = Scan2(230,0);
mapGoal[7] = Scan2(163,0);
mapGoal[8] = Scan2(65,0);
//15 

case 11:
Drift(5,40,-90,30,140);   


case 13: if(key == 13)bgate = 0;
case 14: if(key == 14)bgate = 1;
case 15: if(key == 15)bgate = 2;


ar ab345(30);


At(15,20);   wa = 15;

if(bgate != 2)
   mapGoal[5]   = Scan2(44,77);


if(bgate == 2){
   Axis(0,40,0,40,150);
   
   mapGoal[4]   = Scan2(44,77);
   

   Axis(0,30,0,30,140);

   mapGoal[3]   = Scan2(44,77);

   Axis(0,30,0,30,140);

}
if(bgate == 1){
   Axis(0,70,0,40,150);
   
   mapGoal[3]   = Scan2(44,77);

   Axis(0,30,0,30,140);

}
if(bgate == 0){
   
   Axis(0,30,0,30,140);

   mapGoal[4]   = Scan2(44,77);   

   Axis(0,70,0,40,150);

}

lcs
lcd(1,1,"%d %d %d",mapGoal[3],mapGoal[4],mapGoal[5]);


mapGoal[0]   = Scan2(105,18);   
mapGoal[1]   = Scan2(70,41);   
mapGoal[2]   = Scan2(19,97);   


case -5:

wa = 15;
Ad(0,-10,-15-20,30,140,40);   wa = -20;// 90 60
map[6]   = Scan(213,148);   

case -6:

wa = -20;
Axis(0,30,0,40,50); 
Drift(5,35,-70,30,-40);   cw


ar ab345(30);

SS();

lcs 

lcd(1,1,"%d %d %d / %d %d %d",map[0],map[1],map[2],mapGoal[0],mapGoal[1],mapGoal[2]);
lcd(2,1,"%d %d %d / %d %d %d",map[3],map[4],map[5],mapGoal[3],mapGoal[4],mapGoal[5]);
lcd(3,1,"%d %d %d / %d %d %d",map[6],map[7],map[8],mapGoal[6],mapGoal[7],mapGoal[8]);



/////////////////////////ALGO
   int bp = 0;
   for(int i = 0 ; i < 9 ; i++){
      if(!map[i]) bp = i;
      Link(i,link[i]);
   }
   
   for(int i = 0 ; i <= M ; i++){
      min = i;
      memcpy(node,map,sizeof(int) * 9);
      Sort(-1,bp,0,node);
      if(flg)   {
         GetIn(bp);
         
         for(int i = 0 ; i < min ; i++)
            MovePoint(g_route[i],g_route[i+1]);
         
         
         GetOut(g_route[min]);
         break;
      }
   }


MoveC();
GetPuck();
MoveA();
PutPuck(g_route[min]);

MoveC();

Fin();


///////////


///





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

void GetPuck(){
   int puck = -1;
   ar ab345(30);

   Drift(0,35,-20,30,40);   wa = -20;
   
   if(T(103,3)) puck = 2;
   
   Drift(1,41,-70,30,0);   cw


   CPsd(12,1,5,3,12,0,40,50);
   CPsd(10,1,5,5,12,0,40,0);
   CPsd(1,1,1000,3,12,20,40,0);

   if(puck < 0 && T(101,3)) puck = 0;

   if(puck < 0){
      Ad(0,21,-70,40,-50,70);   wa = -70;

      if(T(101,3)) puck = 3;
      else puck = 1;
   }

   if(puck == 0){
      Avoid(0,12,16,0,35,50,0);
      
      CPsd(12,1,5,3,12,0,40,0);
      CPsd(10,1,10,5,12,0,40,0);
      CPsd(1,1,1000,3,12,20,40,0);
      Drift(2,42,-90,30,0);

      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,75 - 5 - 35,5,12,0,50,0);
      Drift(3,33,-90,40,0);
      
      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,140 - 5 - 35,5,12,0,50,0);
      Drift(3,32,-90,40,-50);

      ar ab345(30);
   }
   else if(puck == 1){
      At(140,100);   wa = 70;
      Drift(0,18,20,25,35);   cw
      
      CPsd(12,8,5,3,16,0,40,50);
      CPsd(10,8,10,5,16,0,40,0);
      CPsd(8,8,1000,3,16,25,40,0);
      Drift(2,42,-90,30,0);

      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,70 - 5 - 35,5,12,0,50,0);
      Drift(3,33,-90,40,0);
      
      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,150 - 5 - 35,5,12,0,50,0);
      Drift(3,36,-90,40,0);

      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,140 - 5 - 35,5,12,0,50,0);
      Drift(3,32,-90,40,-50);

      ar ab345(30);
   }
   else{
      if(puck == 2)
         Drift(1,38,-90,30,0);

      else   Drift(0,18,-18,25,35);   cw
   
      CPsd(12,1,5,3,12,0,40,50);
      CPsd(10,1,10,5,12,0,40,0);
      CPsd(1,1,1000,3,12,20,40,0);
      Drift(2,36,-90,30,0);

      CPsd(12,1,5,3,12,0,50,60);
      CPsd(10,1,70 - 5 - 35,5,12,0,50,0);
      Drift(3,33,-90,40,-50);
   }

   ar ab345(30);
}

int Scan(int x, int y){	
	for(int t = 0 ; t < 5 ; t++) {
		SV1
		V1();
		V3

		for(int i = 0 ; i <  9 ; i++){
			if(!pro[i][0]) continue;
			if(abs(pro[i][1] - x) < 25){
				if(abs(pro[i][2] - y) < 25) return pro[i][0];
			}
		}
   }
   return 0;
}

int Scan2(int x, int y){
	
	for(int t = 0 ; t < 5 ; t++) {
		SV1
		V1();
		V3

		for(int i = 0 ; i <  9 ; i++){
			if(!pro[i][0]) continue;
			if(abs(pro[i][1] - x) < 20){
				if(abs(pro[i][2] - y) < 20) return pro[i][0];
			}
		}
	}
	return 0;
}



void Fin(){
   
   ar ab345(30);

   Axis(0,35,0,40,50);
   
   
   Drift(1,41,-90,30,0);   cw

   CPsd(12,1,5,3,12,0,40,50);
   CPsd(10,1,5,5,12,0,40,0);
   CPsd(1,1,1000,3,12,20,40,0);

   Axis(0,39,0,40,-50);

   LED_ON(3);
   _delay_ms(2000);
   LED_OFF(3);
   
}



/*










*/

void MoveA() {
   ar ab45(30);
   ar ab345(30);
   cw


   if(bgate==0) {
      Drift(-3,47,90,40,150);
	  Avoid(0,11,15,0,15,40,50);
	  Drift(0,20,40,35,0); wa=40;
	  Ot(20,90,40,30,0); wa=-10;
	  Drift(2,27,-80,30,-40); cw
   }
   else {
      CPsd(12,1,5,3,12,0,40,50);
      CPsd(10,1,5,5,12,0,40,0);
      CPsd(1,1,1000,3,12,22,40,0);
	  Drift(-3,36,91,40,-50);
	  Avoid(0,11,15,0,15,40,50);
	  Drift(0,20,40,35,0); wa=40;
	  Ot(17,90,50,30,0); cw
      CPsd(12,1,10,3,12,0,40,0);
      CPsd(10,1,(bgate==1)? 10:40,5,12,0,40,0);
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
      Drift(25,-1,70,25,135); wa=-20;
      CPsd(12,4,-3,3,16,0,35,45);
      CPsd(10,4,-3,5,16,0,35,0);
      CPsd(4,4,-1000,3,16,26,35,0);
      Axis(0,-18,0,35,0); wa=-27;
      Ot(-22,-90,-40,25,0); wa=30;
      Drift(0,-13,-30,25,0); cw
      Ot(-20,-90,-30,25,0); wa=-30;
      Drift(-25,0,-40,40,0); wa=20;
      CPsd(12,3,-5,3,15,0,40,0);
      CPsd(10,3,-5,5,15,0,40,0);
      CPsd(-4,3,-1000,3,15,25,40,0);
      Drift(0,-12,-20,40,-35); cw
   }

   ar ab45(30);
   ar ab345(30);
}


int GetDir(int p, int wp){
   for(int i = 0 ; i < 4;  i++)
      if(link[p][i] == wp)   return i;    
   
}

int GetAng(int d, int wd){
   int rd = (wd - d);

   if(abs(rd) == 3) rd/=-3;

   return rd * 90;
}

void MLC(){
   MoveLineCheck(0,-2,0,0,10,120);
   MoveLineCheck(0,4,0,0,10,120);
   MoveLineCheck(0,-6,0,0,10,120);
   MoveLineCheck(0,8,0,0,10,120);
   MoveLineCheck(0,-10,0,0,10,120);
}

void MovePoint(int from, int to){
   
   int wd = GetDir(from,to);
   int rd = GetAng(nd,wd);
   if(rd)   Axis(0,0,rd,60,130);

   nd = wd;
   MLC();
   MoveLine(2,0,1000,40,50);
   
   Od2(0,24,-180,40,-50,60,130);

   
   MLC();
   set
   MoveLine(2,0,1000,30,40);
   Axis(0,1,0,30,-40);

   Axis(0,-pos[1],0,30,140);

   nd = (nd + 2) % 4;
}

void GetIn(int n){
   ar ab45(30);
   ar ab345(30);

   int cm = 10;

   if(n==0) {
      CPsd(12,1,50,3,12,0,50,60);
      CPsd(10,1,55+cm,5,12,0,50,0);
      Ot(25,-90,0,45,-55);
      nd=0;
   }
   else if(n==3) {
      CPsd(12,1,18,3,12,0,50,60);
      CPsd(10,1,32+cm,5,12,0,50,0);
      Ot(25,-90,0,45,-55);
      nd=0;
   }
   else if(n==6) {
      Axis(0,4,0,40,50);
      Ot(22,-90,0,40,-50);
      nd=0;
   }
   else if(n==1) {
      CPsd(12,1,44,3,12,0,60,70);
      CPsd(10,1,62+cm,5,12,0,60,0);
      Drift(3,45,-90,50,-60);

      ar ab45(30);
      ar ab345(30);

      CPsd(12,1,16,3,12,0,40,50);
      CPsd(10,1,32+cm,5,12,0,50,0);
      Ot(26,-90,0,45,-55);
      nd=3;
   }
   else if(n==2) {
      CPsd(12,1,45,3,12,0,60,70);
      CPsd(10,1,62+cm,5,12,0,60,0);
      Drift(3,45,-90,50,-60);

      ar ab45(30);
      ar ab345(30);

      CPsd(12,1,43,3,12,0,50,60);
      CPsd(10,1,52+cm,5,12,0,50,0);
      Ot(26,-90,0,45,-55);
      nd=3;
   }
   else if(n==4) {
      CPsd(12,1,10,3,12,0,50,60);
      CPsd(10,1,7+cm,5,12,0,50,0);
      Drift(3,30,-90,40,-50);

      ar ab45(30);

      Axis(0,51,0,50,60);
      Ot(27,90,0,45,-55);
      nd=3;
   }
   else if(n==5) {
      CPsd(12,1,10,3,12,0,50,60);
      CPsd(10,1,7+cm,5,12,0,50,0);
      Drift(3,30,-90,40,-50);

      ar ab45(30);

      Axis(0,107,0,50,60);
      Ot(26,90,0,45,-55);
      nd=3;
   }
   else if(n==7) {
      CPsd(12,1,10,3,12,0,50,60);
      CPsd(10,1,7+cm,5,12,0,50,0);
      Drift(3,30,-90,40,-50);

      ar ab45(30);

      Axis(0,46+cm,0,50,60);
      Ot(27,-90,0,45,-55);
      nd=3;
   }
   else if(n==8) {
      CPsd(12,1,10,3,12,0,50,60);
      CPsd(10,1,7+cm,5,12,0,50,0);
      Drift(3,30,-90,40,-50);

      ar ab45(30);

      Axis(0,98+cm,0,50,60);
      Ot(30,-90,0,45,-55);
      nd=3;
   }
}

void GetOut(int n){
   if(n%3==0) {
      if(nd!=0) Axis(0,0,GetAng(nd,0),110,120);
      wa=90;
      Ot(-22,90,-20,40,50); wa=-20;
      CPsd(12,4,-5,3,16,0,40,0);
      CPsd(10,4,-5,5,16,0,40,0);
      CPsd(-5,4,-1000,3,16,26,40,0);
      Drift(0,-11,20,35,-45); cw
   }
   else {
      if(nd!=3) Axis(0,0,GetAng(nd,3),110,120);
      wa=90;
      Ot(31,-90,20,40,50); wa=20;
      Avoid(4,12,16,0,(n==4)? -20:-75,40,0);
      Avoid(4,12,16,26,-1000,40,0);
      Ot(-20,90,40,40,0); wa=-30;
      Drift(0,-5,10,40,0); wa=-20;
      CPsd(12,4,-5,3,16,0,40,0);
      CPsd(10,4,-5,5,16,0,40,0);
      CPsd(-5,4,-1000,3,16,26,40,0);
      Drift(0,-11,20,40,-50); cw
   }

   ar ab45(30);
   ar ab345(30);
}

void PutPuck(int index){
   ar ab45(30);
   ar ab345(30);

   if(index==3) {
      CPsd(12,1,10,3,12,0,40,50);
      CPsd(10,1,45,5,12,0,40,0);
      Ot(10,-90,-40,35,-45); wa=-40;
      Axis(-10,-16,0,40,50);
      Drift(0,-35,40,40,-50); cw
   }
   else {
      CPsd(12,1,5,3,12,0,40,50);
      CPsd(10,1,15,5,12,0,40,0); //25 32 57
      Drift(4,37,-90,35,-45);
      ar ab45(30);

      Avoid(0,12,16,0,(index==4)? 49:106,40,50);
      Ot(10,90,40,30,-40); wa=40;
      Axis(10,-16,0,40,50);
      Drift(0,-20,-20,40,0); wa=20;
      Avoid(4,12,16,0,(index==4)? -5:-60,40,0);
      Avoid(4,12,16,26,-1000,40,0);
      Ot(-20,90,40,40,0); wa=-30;
      Drift(0,-5,10,40,0); wa=-20;
      CPsd(12,4,-3,3,16,0,40,0);
      CPsd(10,4,-3,5,16,0,40,0);
      CPsd(-5,4,-1000,3,16,28,40,0);
      Drift(0,-11,20,40,-50); cw
   }

   ar ab45(30);
   ar ab345(30);
}
