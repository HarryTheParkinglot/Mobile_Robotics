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


#define ab45    wa = 90; AB(0,0,4,184,5,167,50,0); cw
#define ab345(y) wa = 90; AB(3,126,4,184,5,167,50,y); cw
#define ab645(y) wa = 90; AB(6,144,4,184,5,167,50,y); cw
#define ab86 AB(0,0,6,136,8,112,50,0);

#define ab865(y) AB(5,195,6,173,8,125,30,y); cw

#define ab134(y) AB(4,188,1,165,3,158,30,y); cw

#define tx 142
#define ty 75

#define tx1 123
#define ty1 156

#define tx2 124
#define ty2 167
int A[3] = {0};


int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int link[18][4];
int map[18] = {0};
int mapGoal[18] = {0};
int node[18];

int block[18] = {0},check[18] = {0}; 

int route[10],g_route[10],min,flg = 0;
int sort_min;
int sort_route[20][3],type[20];
int g_sort_route[20][3],g_type[20]; //0 : (default) --> GetIn / 1 : getPuck(특정 포인트로 이동) / 2 : putPuck  / 3 : Strike Puck  
int nowPos = -1,nowDir = 0;

int InList(int val, int *list, int len){
	for(int i = 0 ; i < len ; i++)
		if(val == list[i]) return 1;
	return 0;
}

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 3 > 0) str[0] = p - 3; 
	if(p % 3 > 0) str[1] = p - 1; 
	if(p / 3 < 5) str[2] = p + 3; 
	if(p % 3 < 2) str[3] = p + 1; 
	if(p % 3 == 1) str[0] = str[2] = -1;
}

void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	route[dp] = p;
	
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || (node[np] && np != wp) || check[np] || block[np]) continue;
		DFS(np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	memset(check,0,sizeof(check));
	min = 10;
	DFS(p,wp,0,node);
	return min != 10;
}

int Sorted(int *node){
	for(int i = 0 ; i < 18 ; i++){
		if(!node[i]) continue;
		if(node[i] != mapGoal[i]) return 0;
	}
	return 1;
}
void display(int *node){
	lcs 
	for(int i = 2 ; i >= 0 ; i--){
		lcd(2-i,0,"%d %d %d %d %d %d",node[i],node[i+3],node[i+6],node[i+9],node[i+12],node[i+15]);
	}
}

void SortDFS(int dp, int change, int *node){
	
	if(dp >= sort_min) return;

	if(Sorted(node)){
		sort_min = dp;
		memcpy(g_type,type,sizeof(type));
		
		for(int j = 0 ; j < sort_min ; j++)
			for(int i = 0 ;i < 3 ; i++)
				g_sort_route[j][i] = sort_route[j][i];
		
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 6 ; i++){
		if(!node[i*3 + 1]) continue;
		if(node[i*3] || node[i*3+2]) return; //퍽이 있는 상태에서 가로막히면 해당 경우 버림  
	}
	
	for(int i = 0 ; i < 18 && !flg ; i++){
		if(!node[i] || node[i] == mapGoal[i]) continue;
		
		for(int j = 0 ; j < 36 && !flg; j++){
			if(node[j%18] || !mapGoal[j%18] || !CheckDFS(i,j%18,node)|| (i/3 != (j%18)/3 && node[((j%18)/3*3)+1]))continue;
			if(j / 18 == 0 && mapGoal[j%18] != node[i]) continue;
			if(j / 18 && mapGoal[j%18] == node[i]) continue;
			if(node[i] != mapGoal[j%18] && change >= 4) continue;
			
			int start = -1,plus = (mapGoal[j%18] != node[i]);
			
			if(i % 3 == 1)
				start = (InList(i-1,g_route,min))? i + 1 : i - 1;				
			
			else{
				if(i >= 3 && !node[i-3]) start = i-3;
				if(i <= 12 && !node[i+3]) start = i+3;	
			}
			
			if(start < 0) continue;
			
			sort_route[dp][0] = start; //Robot In
			sort_route[dp][1] = i; //Real Puck Pos 
			sort_route[dp][2] = j%18; //Put Pos
			type[dp] = (block[i]); 
				
			swap(node[i],node[j%18]);
			
			SortDFS(dp+1,change + plus,node);
			swap(node[i],node[j%18]);
			
		}
	}
	
	return;
} 


/////////////////////////////////////////////////////////


int GetDir(int p, int np){
	for(int i = 0 ; i < 4;  i++)
		if(link[p][i] == np) return i;
}

int GetAng(int d, int wd){
	int rd = wd - d;
	if(abs(rd) == 3) rd/=-3;
	if(abs(rd) == 2) rd = (g_psd[2] > 25)? 2 : -2;
	return rd * 90;
}

void MLC(){
	MoveLineCheck(0,2,0,0,10,120);
	MoveLineCheck(0,-4,0,0,10,120);
	MoveLineCheck(0,6,0,0,10,120);
	MoveLineCheck(0,-8,0,0,10,120);
	MoveLineCheck(0,10,0,0,10,120);
	MoveLineCheck(0,-12,0,0,10,120);
	MoveLineCheck(0,14,0,0,10,120);
	MoveLineCheck(0,-16,0,0,10,120);
	MoveLineCheck(0,18,0,0,10,120);
}

void ChangeDir(int p, int wd){
	if(nowDir == wd) return;

	wa = nowDir * 90;  //dir 0이면 wa = 0 ; wa = 1이면 각도 90 wa = 90;

	set
	Axis((p % 3 == 0)? -5 : 5,0,0,20,130);	
	Axis(0,0,GetAng(nowDir,wd),60,130); 
	nowDir = wd;

	wa = nowDir * 90;

	Axis((p % 3 == 0)? 5 : -5,0,0,10,120);	cw
	MLC();
	
	
}

int GetNextDir(int p){
	int prior[4] = {0,1,3,2};

	for(int i = 0 ; i < 4 ; i++){
		int wd = (nowDir + prior[i])%4;
		if(link[p][wd] >= 0) return wd;
	}
	return -1;
}


void GoPos(int mode, int p, int np){
	CheckDFS(p,np,map);
	
	for(int i = 0 ; i < min ; i++){
		GoPtoP((i == min -1 && mode),g_route[i],g_route[i+1]);	
	}
	if(map[p]){
		swap(map[p],map[np]);
	}
	
}

void GoPtoP(int mode, int p, int np){
	//mode 0 : Get / mode 1 : Put
	lcs lcd(1,1,"%d | %d - > %d",mode,p,np);
		wait
	ChangeDir(p,GetDir(p,np));
	
	if(mode){
		MLC();
		MoveLine(10,0,26,30,140);
		Axis(0,-26,0,30,140);
		MLC();
		SS();
	} 

	else{
		MoveLine(10,0,20,30,40);
		
		int wwd = GetNextDir(np);
		
		if(nowDir != wwd)
			Od2(0,30,GetAng(nowDir,wwd),30,-40,60,130);
		
		else{
			MoveLine(1,0,1000,30,0);	
			MoveLine(10,0,13,30,-40);
		}

		MLC();
		nowPos = np;
		nowDir = wwd;
	}

	
}

int ConvertPos(int p){
	if(p % 3) return p - 2;
	return p+2;
}


//////////////////////////////////////////////////////////////////////////

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

for(int i = 0 ; i < 18 ; i++)	Link(i,link[i]);


Axis(-3,3,0,10,120);

ar ab645(20);

bar_2 = -1;

for(int i = 0 ; i < 5 ; i++){
	CPsd(12,8,5,3,20,0,50,60 * (i == 0));
	CPsd(10,8,(i)? 45 : 40,5,20,0,50,0);

	if(bar_2 && bar_2 < 3) bar_2 = 3 - bar_2;
	mapGoal[(4 - i)*3 + 2] = bar_2;
	
	
	bar_2 = 0;
}
Drift(-3,35,90,40,-50);
lcs
lcd(1,1,"%d %d %d %d %d",mapGoal[2],mapGoal[5],mapGoal[8],mapGoal[11],mapGoal[14]);

ar ab645(20);	
Axis(0,5,0,10,120);
At(10,20);	wa = -80;


wait


for(int i = 0 ; i < 6 ; i++){
	map[i*3 + 1] = T(103,3);
	
	if(Cmd(T(103,3),103) < 20)
		block[i*3+1] = 1;

	if(i == 5) break;

	CPsd(12,3,5,3,24,0,40,50 * (i == 0));
	CPsd(10,3,45,5,24,0,40,0);
}

Drift(0,20,-10,40,-50);		cw

ar ab345(30);

scan();

Avoid(0,14,18,0,110,50,60);
Drift(5,35,-90,40,-50);	

ar ab345(30);
lcd(2,1,"%d %d %d %d %d %d",map[1],map[4],map[7],map[10],map[13],map[16]);
lcd(3,1,"%d %d %d %d %d %d",block[1],block[4],block[7],block[10],block[13],block[16]);

//wait

bar_2 = 0;

for(int i = 0 ; i < 5 ; i++){
	CPsd(12,1,5,3,14,0,50,60 * (i == 0));
	CPsd(10,1,(i)? 45 : 40,5,14,0,50,0);

	if(bar_2 && bar_2 < 3) bar_2 = 3 - bar_2;
	mapGoal[(5 - i)*3] = bar_2;
	
	
	bar_2 = 0;
}

Drift(0,10,-20,40,-50);	wa = -20;

CPsd(12,4,-5,3,18,0,50,60);
CPsd(10,4,-200,5,18,0,50,0);
CPsd(-5,4,-1000,5,18,20,50,0);
Drift(0,-10,20,40,-50);	cw
ar ab345(20);
SS();

lcs
display(map);
lcd(3,0,"MAP");
wait

lcs
display(mapGoal);
lcd(3,0,"MAPGOAL");
wait

memcpy(node,map,sizeof(node));

sort_min = 11;
SortDFS(0,0,node);

nowPos = -3;


for(int i = 0 ; i < sort_min ; i++){

	lcs lcd(1,1,"NP%d P%d GP%d PP%d TP%d",nowPos,g_sort_route[i][0],g_sort_route[i][1],g_sort_route[i][2],type[i]);		wait
	lcs display(map);	wait

	if(nowPos < 0 || (type[i] && !CheckDFS(nowPos,g_sort_route[i][0],map)) || (!type[i] && !CheckDFS(nowPos,g_sort_route[i][1],map))){
		if(nowPos >= 0) GetOut(nowPos);
		GetIn(g_sort_route[i][0]);	wait
	}

	if(type[i]){
		GoPos(0,nowPos,g_sort_route[i][0]); 
		ChangeDir(nowPos,GetDir(nowPos,g_sort_route[i][1]));
		SS();

		LED_ON(3);
		_delay_ms(100);
		LED_OFF(3);
		//lcs lcd(1,1,"STRIKE!");
		wait

		//LenCull(nowPos);
		
		nowPos = ConvertPos(nowPos);

		swap(map[g_sort_route[i][1]],map[nowPos]);

		if(nowPos != g_sort_route[i][2])
			GoPos(1,nowPos,g_sort_route[i][2]);
		
		else{
			LED_ON(3);
			_delay_ms(100);
			LED_OFF(3);
			lcs lcd(1,1,"NOWPOS %d TP %d",nowPos,g_sort_route[i][2]);	wait	
			
			if(nowPos/3 && !map[nowPos-3]){
				GoPos(0,nowPos,nowPos - 3);
				GoPos(1,nowPos,nowPos + 3);	
			}
			else if(nowPos/3 < 5 && !map[nowPos+3]){
				GoPos(0,nowPos,nowPos + 3);
				GoPos(1,nowPos,nowPos - 3);	
			}
		}
	}
	
	else{
		GoPos(0,nowPos,g_sort_route[i][1]);
		
		CheckDFS(g_sort_route[i][1],g_sort_route[i][2],map);
		
		if(g_sort_route[i][1] % 3 == 1 && link[nowPos][nowDir] != g_sort_route[i][2])
			GoPos(0,nowPos,link[nowPos][nowDir]);
		
		GoPos(1,nowPos,g_sort_route[i][2]);
	}
	
	if(nowPos % 3 == 1){
		MLC();
		Axis(0,-50,0,40,150);
		MLC();
		nowPos = link[nowPos][(nowDir+2) % 4];
	}
	//swap(map[g_sort_route[i][1]],map[g_sort_route[i][2]]);
}
GetOut(nowPos);


for(int i = 0 ; i < 18 ; i++){
	if(!mapGoal[i] || map[i]) continue;

	for(int j = 0 ; j < 3 ; j++)
		if(A[j] == mapGoal[i])	{	
			GetPuck(abs(nowPos),j,i%3);
			PutPuck(i);
			A[j] = 0;
			break;
		}
}

if(nowPos == -3) {
	ar ab345(30);
	Drift(30,-1,70,40,50); wa=-20;
	Avoid(5,11,15,0,-110,50,0);
	Avoid(5,11,15,25,-1000,50,0);
	Drift(0,-13,20,50,-60); cw
}

ar ab345(30);

SS();

LED_ON(3);
_delay_ms(2000);
LED_OFF(3);

case -1:
nowPos = -2;
ar ab645(20);
for(int i = 0 ; i < 5 ; i++){
	PutPuck(i*3+2);
	wait
}
wait



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



void GetIn(int n) {
	if((abs(nowPos)) % 3 != n % 3){		
		if(n%3==0) {
			Axis(0,0,90,100,120);
			ar ab345(20);
		
			Avoid(0,14,18,0,120,50,60);
			Drift(3,36,-90,40,-50);	ar ab345(30);
		}
		else{
			Drift(30,-1,70,40,50); wa=-20;
			Avoid(5,11,15,0,-110,50,0);
			Avoid(5,11,15,25,-1000,50,0);
			Ad(0,-13,-70,50,-60,80); cw
			ar ab645(30);
   		}
	} 

	if(n % 3 == 0){
		nowDir = 3;
		
		if(n==15) {
			Ot(24,-90,-90,35,45);   
			Axis(0,5,0,30,-40);
		}
		else if(n==12) {
			CPsd(12,1,3,3,17,0,60,70);
			CPsd(10,1,26,5,17,0,60,0);
			Drift(0,26,-20,50,0); wa=-20;
			Ot(14,-90,-50,45,0); wa=20;
			Drift(0,12,-20,50,-60); cw
		}
		else if(n==9) {
			CPsd(12,1,3,3,17,0,60,70);
			CPsd(10,1,78,5,17,0,60,0);
			Drift(0,26,-20,50,0); wa=-20;
			Ot(14,-90,-50,45,0); wa=20;
			Drift(0,12,-20,50,-60); cw
		}
		else if(n==6) {
			CPsd(12,1,3,3,17,0,60,70);
			CPsd(10,1,127,5,17,0,60,0);
			Drift(0,26,-20,50,0); wa=-20;
			Ot(14,-90,-50,45,0); wa=20;
			Drift(0,12,-20,50,-60); cw
		}
		else if(n==3) {
			CPsd(12,1,3,3,17,0,60,70);
			CPsd(10,1,178,5,17,0,60,0);
			Drift(0,26,-20,50,0); wa=-20;
			Ot(14,-90,-50,45,0); wa=20;
			Drift(0,12,-20,50,-60); cw
		}
		else {
			CPsd(12,1,3,3,17,0,60,70);
			CPsd(10,1,228,5,17,0,60,0);
			Drift(0,26,-20,50,0); wa=-20;
			Ot(14,-90,-50,45,0); wa=20;
			Drift(0,12,-20,50,-60); cw
		}
	}

	else{
		nowDir = 1;
		if(n==17) {
			Ot(24,90,70,35,45); wa=-20;
			Drift(0,5,20,30,-40); cw
		}
		else if(n==14) {
			CPsd(12,8,3,3,22,0,60,70);
			CPsd(10,8,26,5,22,0,60,0);
			Drift(0,26,20,50,0); wa=20;
			Ot(14,90,50,45,0); wa=-20;
			Drift(0,12,20,50,-60); cw
		}
		else if(n==11) {
			CPsd(12,8,3,3,22,0,60,70);
			CPsd(10,8,78,5,22,0,60,0);
			Drift(0,26,20,50,0); wa=20;
			Ot(14,90,50,45,0); wa=-20;
			Drift(0,12,20,50,-60); cw
		}
		else if(n==8) {
			CPsd(12,8,3,3,22,0,60,70);
			CPsd(10,8,127,5,22,0,60,0);
			Drift(0,26,20,50,0); wa=20;
			Ot(14,90,50,45,0); wa=-20;
			Drift(0,12,20,50,-60); cw
		}
		else if(n==5) {
			CPsd(12,8,3,3,22,0,60,70);
			CPsd(10,8,178,5,22,0,60,0);
			Drift(0,26,20,50,0); wa=20;
			Ot(14,90,50,45,0); wa=-20;
			Drift(0,12,20,50,-60); cw
		}
		else {
			CPsd(12,8,3,3,22,0,60,70);
			CPsd(10,8,230,5,22,0,60,0);
			Drift(0,26,20,50,0); wa=20;
			Ot(14,90,50,45,0); wa=-20;
			Drift(0,12,20,50,-60); cw
		}
	}
	MLC();
	nowPos = n;
}

void GetOut(int n) {
   if(n%3==0) {	
   		nowPos = -3;
      if(n==15) {
	  	ChangeDir(n,0);
         
         Axis(-25,-25,0,40,150);
      }
      else if(n==12) {
         ChangeDir(n,0);
         wa=90;
         Ot(-29,90,-20,50,60); wa=-20;
         CPsd(12,4,-10,3,15,0,60,0);
         CPsd(10,4,-10,5,15,0,60,0);
         CPsd(-5,4,-1000,3,15,25,60,0);
         Ad(0,-13,20,50,-60,60); cw
      }
      else if(n==9) {
         ChangeDir(n,0);
         wa=90;
         Ot(-29,90,-20,50,60); wa=-20;
         CPsd(12,4,-10,3,15,0,60,0);
         CPsd(10,4,-60,5,15,0,60,0);
         CPsd(-5,4,-1000,3,15,25,60,0);
         Ad(0,-13,20,50,-60,60); cw
      }
      else if(n==6) {
         ChangeDir(n,0);
         wa=90;
         Ot(-29,90,-20,50,60); wa=-20;
         CPsd(12,4,-10,3,15,0,60,0);
         CPsd(10,4,-110,5,15,0,60,0);
         CPsd(-5,4,-1000,3,15,25,60,0);
         Ad(0,-13,20,50,-60,60); cw
      }
      else if(n==3) {
         ChangeDir(n,0);
         wa=90;
         Ot(-29,90,-20,50,60); wa=-20;
         CPsd(12,4,-10,3,15,0,60,0);
         CPsd(10,4,-160,5,15,0,60,0);
         CPsd(-5,4,-1000,3,15,25,60,0);
         Ad(0,-13,20,50,-60,60); cw
      }
	else {
	  	ChangeDir(n,3);
		Drift(27,0,90,35,145);
		wa=90;
		Ot(-29,90,-20,50,60); wa=-20;
		CPsd(12,4,-10,3,15,0,60,0);
		CPsd(10,4,-180,5,15,0,60,0);
		CPsd(-5,4,-1000,3,15,25,60,0);
		Ad(0,-13,20,50,-60,60); cw
      }
	  ar ab345(20);
   }
   else {
   		nowPos = -2;
      if(n==17) {
         ChangeDir(n,0);
         Axis(27,-22,0,40,150);
      }
      else if(n==14) {
         ChangeDir(n,0);
         wa=-90;
         Ot(-28,-90,20,50,60); wa=20;
         CPsd(12,5,-10,3,11,0,60,0);
         CPsd(10,5,-10,5,11,0,60,0);
         CPsd(-4,5,-1000,3,11,25,60,0);
         Ad(0,-13,-20,50,-60,60); cw
      }
      else if(n==11) {
         ChangeDir(n,0);
         wa=-90;
         Ot(-28,-90,20,50,60); wa=20;
         CPsd(12,5,-10,3,11,0,60,0);
         CPsd(10,5,-60,5,11,0,60,0);
         CPsd(-4,5,-1000,3,11,25,60,0);
         Ad(0,-13,-20,50,-60,60); cw
      }
      else if(n==8) {
         ChangeDir(n,0);
         wa=-90;
         Ot(-28,-90,20,50,60); wa=20;
         CPsd(12,5,-10,3,11,0,60,0);
         CPsd(10,5,-110,5,11,0,60,0);
         CPsd(-4,5,-1000,3,11,25,60,0);
         Ad(0,-13,-20,50,-60,60); cw
      }
      else if(n==5) {
         ChangeDir(n,0);
         wa=-90;
         Ot(-28,-90,20,50,60); wa=20;
         CPsd(12,5,-10,3,11,0,60,0);
         CPsd(10,5,-160,5,11,0,60,0);
         CPsd(-4,5,-1000,3,11,25,60,0);
         Ad(0,-13,-20,50,-60,60); cw
      }
      else {
	  	ChangeDir(n,1);
         Drift(-27,0,-90,35,145);
         wa=-90;
         Ot(-28,-90,20,50,60); wa=20;
         CPsd(12,5,-10,3,11,0,60,0);
         CPsd(10,5,-180,5,11,0,60,0);
         CPsd(-4,5,-1000,3,11,25,60,0);
         Ad(0,-13,-20,50,-60,60); cw
      }
	  ar ab645(20);
   }
}


void LenCull(int n) {
   set
   if(n % 3 == 2)	ChangeDir(n,1);
   else 			ChangeDir(n,3);

   ar TB(T(103,3),0,tx,ty);
   MLC();
   Axis(0,-18,0,20,130);
   MoveLine(10,0,37,90,170);
   SS();
   Axis(0,-pos[1],0,40,150);

   GetOut(n);

   Avoid(0,11,15,0,90,50,60);
   Drift(3,50,-90,50,-60);
   Axis(2,2,0,10,120);
   ar ab345(30);

   if(n/3==0) {
      Axis(0,0,-50,70,130);
      ar TB(T(103,3),0,tx1,ty1);
      Axis(0,25,0,40,50);
      Ad(0,10,-40,40,-50,70);
      MLC();
   }

   else{
      CPsd(12,1,10,3,11,0,50,60);
      CPsd(10,1,35*(n/3),5,11,0,50,0);
      Ad(0,5,-50,50,-60,70);
      ar TB(T(103,3),0,tx1,ty1);
      Axis(0,25,0,40,50);
      Ad(0,10,-40,40,-50,70);
      MLC();
   }
}


void scan() {
   Drift(0,85,40,50,160);
   A[0] = T(103,3);
   Axis(0,0,50,70,130);
   A[1] = T(103,3);
   Axis(0,0,50,70,130); wa=50;
   A[2] = T(103,3);

   lcs lcd(1,0, "%d %d %d", A[0],A[1],A[2]);

   Drift(-85,0,-140,50,160); cw
   ar ab345(30);
}

void GetPuck(int np, int n, int wp) {
   if(np%3==0) {
   		ar ab345(30);
      Drift(30,-1,70,40,50); wa=-20;
      Avoid(5,11,15,0,-110,50,0);
      Avoid(5,11,15,25,-1000,50,0);
      Drift(0,-13,20,50,-60); cw
      ar ab345(30);
   }
   else {
      Axis(0,0,90,130,120);
      ar ab345(30);
   }

   if(n==0) { //가장 오른쪽 퍽
      ar ab345(30);
      Drift(0,60,20,50,60); wa=20;
      Ot(25,45,25,45,0); cw
      Avoid(0,11,15,0,80,50,-60); wa=-45;
      Drift(20,-5,45,40,150); cw
      ar ab645(30);
      Axis(0,0,90,70,130);
      ar ab345(30);
      Axis(-3,0,0,10,120);

      if(wp%3==0) { //왼쪽으로 가는 경우
         Drift(0,30,20,50,60); wa=20;
         Ot(25,90,90,45,0);
         Ot(25,90,60,45,0); wa=-10;
         Drift(8,27,-80,30,-40); cw
         ar ab345(30);
      }
      else { //오른쪽으로 가는 경우
         Drift(0,60,20,50,60); wa=20;
         Ot(24,90,0,45,0); wa=-90;
         Ot(23,-90,0,45,0); wa=20;
         Drift(-5,58,70,50,-60); cw
         ar ab645(30);
      }
   }
   else if(n==1) { //가운데 퍽
      Drift(0,51,20,50,60); wa=20;
      Ot(25,90,70,45,-55); cw
      ar TB(T(103,3),0,tx2,ty2);
      Axis(0,10,0,40,50);

      if(wp%3==0) {
         Ot(10,90,0,40,0);
         Ad(-5,0,-90,45,-55,70);
         wa=-90;
         Ot(-50,90,-20,40,50);
         wa=-20;
         Axis(0,15,0,45,0);
         Drift(5,27,-70,35,-45); cw
         ar ab345(30);
      }
      else {
         Ot(10,-90,0,40,0);
         Ad(5,0,90,45,-55,70);
         wa=90;
         Ot(-50,-90,20,40,50);
         wa=20;
         Axis(0,15,0,45,0);
         Drift(-5,30,70,45,-55); cw
         ar ab645(30);
      }
   }
   else { //가장 왼쪽 퍽
      Drift(0,30,20,50,60); wa=20;
      Ot(27,90,90,45,0);
      Drift(0,10,30,40,-50); cw
      Axis(0,40,0,40,150); wa=50;
      Drift(-25,0,-50,40,150); cw
      ar ab345(30);
      Axis(0,0,-90,70,130);
      ar ab645(30);
      Axis(3,0,0,10,120);

      if(wp%3==0) { //왼쪽으로 가는 경우
         Drift(0,60,-20,50,60); wa=-20;
         Ot(24,-90,0,45,0); wa=70;
         Ot(23,90,0,45,0); wa=-20;
         Drift(5,50,-70,40,-50); cw
         ar ab345(30);
      }
      else { //오른쪽으로 가는 경우
         Drift(0,30,-20,50,60); wa=-20;
         Ot(25,-90,-90,45,0);
         Ot(25,-90,-60,45,0); wa=10;
         Drift(-8,30,80,40,-50); cw
         ar ab645(30);
      }
   }
   
}

void PutPuck(int wp) {
   if(wp%3==0) { //왼쪽에서 시작
   		nowPos = -3;
      if(wp==12) {
         ar ab345(30);
         CPsd(12,1,8,3,11,0,60,70);
         CPsd(10,1,25,5,11,0,60,0);
         Ot(20,-80,-43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,45,13,45,0); wa=-30;
         Drift(0,-10,10,50,0); wa=-20;
         CPsd(12,4,-7,3,17,0,60,0);
         CPsd(10,4,-7,5,17,0,60,0);
         CPsd(-5,4,-1000,3,17,25,60,0);
         Drift(0,-9,20,60,-70); cw
      }
      else if(wp==9) {
         ar ab345(30);
         CPsd(12,1,8,3,11,0,60,70);
         CPsd(10,1,75,5,11,0,60,0);
         Ot(20,-80,-43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,45,13,45,0); wa=-30;
         Drift(0,-10,10,50,0); wa=-20;
         CPsd(12,4,-7,3,17,0,60,0);
         CPsd(10,4,-57,5,17,0,60,0);
         CPsd(-5,4,-1000,3,17,25,60,0);
         Drift(0,-9,20,60,-70); cw
      }
      else if(wp==6) {
         ar ab345(30);
         CPsd(12,1,8,3,11,0,60,70);
         CPsd(10,1,127,5,11,0,60,0);
         Ot(20,-80,-43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,45,13,45,0); wa=-30;
         Drift(0,-10,10,50,0); wa=-20;
         CPsd(12,4,-7,3,17,0,60,0);
         CPsd(10,4,-107,5,17,0,60,0);
         CPsd(-5,4,-1000,3,17,25,60,0);
         Drift(0,-9,20,60,-70); cw
      }
      else if(wp==3) {
         ar ab345(30);
         CPsd(12,1,8,3,11,0,60,70);
         CPsd(10,1,175,5,11,0,60,0);
         Ot(20,-80,-43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,45,13,45,0); wa=-30;
         Drift(0,-10,10,50,0); wa=-20;
         CPsd(12,4,-7,3,17,0,60,0);
         CPsd(10,4,-157,5,17,0,60,0);
         CPsd(-5,4,-1000,3,17,25,60,0);
         Drift(0,-9,20,60,-70); cw
      }
      else {
         ar ab345(30);
         CPsd(12,1,8,3,11,0,60,70);
         CPsd(10,1,225,5,11,0,60,0);
         Ot(20,-80,-43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,45,13,45,0); wa=-30;
         Drift(0,-10,10,50,0); wa=-20;
         CPsd(12,4,-7,3,17,0,60,0);
         CPsd(10,4,-207,5,17,0,60,0);
         CPsd(-5,4,-1000,3,17,25,60,0);
         Drift(0,-9,20,60,-70); cw
      }

	  ar ab345(30);
   }   
   else { //오른쪽에서 시작
   	nowPos -2;
      if(wp==14) {
         ar ab645(30);
         CPsd(12,8,8,3,13,0,60,70);
         CPsd(10,8,25,5,13,0,60,0);
         Ot(20,90,43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,-45,-13,45,0); wa=30;
         Drift(0,-10,-10,50,0); wa=20;
         CPsd(12,5,-7,3,13,0,60,0);
         CPsd(10,5,-7,5,13,0,60,0);
         CPsd(-4,5,-1000,3,13,25,60,0);
         Drift(0,-9,-20,60,-70); cw
      }
      else if(wp==11) {
         ar ab645(30);
         CPsd(12,8,8,3,13,0,60,70);
         CPsd(10,8,75,5,13,0,60,0);
         Ot(20,90,43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,-45,-13,45,0); wa=30;
         Drift(0,-10,-10,50,0); wa=20;
         CPsd(12,5,-7,3,13,0,60,0);
         CPsd(10,5,-57,5,13,0,60,0);
         CPsd(-4,5,-1000,3,13,25,60,0);
         Drift(0,-9,-20,60,-70); cw
      }
      else if(wp==8) {
         ar ab645(30);;
         CPsd(12,8,8,3,13,0,60,70);
         CPsd(10,8,125,5,13,0,60,0);
         Ot(20,90,43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,-45,-13,45,0); wa=30;
         Drift(0,-10,-10,50,0); wa=20;
         CPsd(12,5,-7,3,13,0,60,0);
         CPsd(10,5,-107,5,13,0,60,0);
         CPsd(-4,5,-1000,3,13,25,60,0);
         Drift(0,-9,-20,60,-70); cw
      }
      else if(wp==5) {
         ar ab645(30);
         CPsd(12,8,8,3,13,0,60,70);
         CPsd(10,8,175,5,13,0,60,0);
         Ot(20,90,43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,-45,-13,45,0); wa=30;
         Drift(0,-10,-10,50,0); wa=20;
         CPsd(12,5,-7,3,13,0,60,0);
         CPsd(10,5,-157,5,13,0,60,0);
         CPsd(-4,5,-1000,3,13,25,60,0);
         Drift(0,-9,-20,60,-70); cw
      }
      else {
         ar ab645(30);
         CPsd(12,8,8,3,13,0,60,70);
         CPsd(10,8,225,5,13,0,60,0);
         Ot(20,90,43,50,-60);
         Axis(0,-18,0,50,60);
         Ot(-5,-45,-13,45,0); wa=30;
         Drift(0,-10,-10,50,0); wa=20;
         CPsd(12,5,-7,3,13,0,60,0);
         CPsd(10,5,-207,5,13,0,60,0);
         CPsd(-4,5,-1000,3,13,25,60,0);
         Drift(0,-9,-20,60,-70); cw
      }
	  ar ab645(30);
   }
}

int Scan(int x, int y){
	SS();
	
	if(!T(101,3)) return;

	for(int i = 0 ; i < 5 ; i++){
		SV1
		V1();
		V3

		for(int j = 0 ; j < 9 ; j++){
			if(!pro[j][0]) 	continue;

			if(abs(pro[j][1] - x) < 30 && abs(pro[j][2] - y) < 30)	return pro[j][0];
		}
	}
	return 0;

}
