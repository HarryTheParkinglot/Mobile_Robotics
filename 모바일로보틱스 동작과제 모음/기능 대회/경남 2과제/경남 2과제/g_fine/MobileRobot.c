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



#define ab45    wa = 90; AB(0,0,4,194,5,163,50,0); cw
//#define ab845(y) wa = 90; AB(8,150,4,184,5,167,50,y); cw
//#define ab145(y) wa = 90; AB(1,190,4,184,5,167,50,y); cw
#define ab245(y) wa = 90; AB(2,214,4,194,5,163,50,y); cw
#define ab745(y) wa = 90; AB(7,167,4,194,5,163,50,y); cw

#define ab345(y) wa = 90; AB(2,100,4,194,5,163,50,y); cw
#define ab745(y) wa = 90; AB(7,160,4,194,5,163,50,y); cw

#define ab86 AB(0,0,6,158,8,118,30,0);
#define ab13(x) AB(0,0,1,168,3,132,x,0);

#define ab865(y) AB(5,195,6,173,8,125,30,y); cw
#define ab134(y) AB(4,188,1,165,3,158,30,y); cw

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;



int distance[2] = {75,53};
int map[13] = {0};
int node[13];
int bMap[6];

int link[13][8] = {
   {-1,-1,-1,-1,-1,3,1,-1},
   {-1,-1,0,3,-1,4,2,-1},
   {-1,-1,1,4,-1,-1,-1,-1},
   
   {-1,0,-1,5,-1,-1,-1,1},
   {-1,1,-1,-1,-1,7,-1,2},
   
   {-1,-1,-1,-1,-1,8,-1,3},
   {-1,-1,-1,-1,-1,-1,-1,-1},
   {-1,4,-1,9,-1,-1,-1,-1},
   
   {-1,5,-1,10,-1,11,-1,-1},
   {-1,-1,-1,11,-1,12,-1,7},
   
   {-1,-1,-1,-1,-1,-1,11,8},
   {-1,8,10,-1,-1,-1,12,9},
   {-1,9,11,-1,-1,-1,-1,-1}
};

int route[50],g_route[50]; //[0] : 회전 타입 / [1] : 회전 방향  [2] : 회전 횟수 
int sort_min,sort_route[20][2],g_sort_route[20][2],check[13];

int flg = 0,fix[13] = {0};
int hist[50][13];
int min = 0,stage = 0,nowPos;
int seq[8] = {1,4,7,9,11,8,5,3};

int InHist(int *node, int dp){
   for(int i = 0  ;i < dp ; i++){
      for(int j = 0 ;  j < 13 ; j++){
         if(node[j] != hist[i][j]) break;
         if(j == 12) return 1;
      }
   }
   return 0;
}

int GetVisitable(int mode, int p, int lp){
	int cnt = 0;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp || fix[np]) continue;		
		if(mode) return np;
		cnt++;
	}
	return cnt;
}

int EdgeCheck(int *node){
   	if(node[0] > 0 && node[0] == node[2] && node[0] != node[6]){ 
      	fix[0] = fix[2] = 1;
      	return 1;
   	}
   	if(node[10] > 0 && node[10] == node[12] && node[10] != node[6]){ 
      	fix[10] = fix[12] = 1;
      	return 1;
   	}
   	return 0;
}

int Check(int *node){ //1차 정렬 확인 함수 
   	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
	if(node[0]+node[2]+node[10]+node[12] != (6-map[6])*2) return 0;
   	if(node[0] != node[2]) return 0;
   
   	int pCnt = 0,pList[8];
   	for(int i = 0 ; i < 8 ; i++){
      	int col = node[seq[i]];
      	if((pCnt && col == pList[pCnt - 1]) || pCnt > 4) return 0;
      	if(col > 0) pList[pCnt++] = col;
   	}
   
   if(pList[pCnt-1] == pList[0]) return 0;
   
   return 1;
}


void Sort(int lp, int p,int dp, int *node){
	if(flg || dp >= min) return;
	if(InHist(node,dp)) return;
	
	route[dp] = p;
	
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if(stage == 0 && EdgeCheck(node) || (stage == 1 && Check(node))){
		flg = 1;
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		int nnp = -1;
		
		if(np < 0 || np == lp || fix[np]) continue;	
		
		swap(node[p],node[np]);
	
		if(GetVisitable(0,np,p) == 1)
			nnp = GetVisitable(1,np,p);
			
		if(nnp >= 0){
			swap(node[np],node[nnp]);
			route[dp+1] = np;
			Sort(np,nnp,dp + 2,node);
		}
		else	Sort(p,np,dp + 1,node);
		
		if(nnp >= 0){swap(node[np],node[nnp]);}	
		
		swap(node[p],node[np]);
	}
}


void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(check[np] || np < 0 || (np != wp && node[np] > 0)) continue;
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
	if(node[0] != node[1]) return 0;
	if(node[10] != node[11]) return 0;
	if(node[5] != node[6] || node[6] != node[7]) return 0;
	return 1;
}

void Final(int lp, int p, int dp, int *node){
	if(dp >= sort_min || flg) return;
	
	//dis(node);
	
	if(Sorted(node)){
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 13 ; i++){
		if(lp == i || node[i] <= 0|| fix[i] || !CheckDFS(p,i,node)) continue;
		
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			swap(node[i],node[j]);
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			Final(j,g_route[min-1],dp+1,node);
			swap(node[i],node[j]);
		}	
	}
}

//////////////////

int nowPos,nowDir,gate,total = 18;

int GetDir(int p, int np){
	for(int i = 0 ; i < 8 ; i++)
		if(link[p][i] == np) return i;
}

int GetAng(int p, int wd){
	int rd = wd - nowDir;

	if(abs(rd) > 4)  rd = (rd > 0)? rd - 8 : rd + 8;

	if(abs(rd) == 4){
		if(rd > 0 && link[p][(nowDir+1)%8] < 0 && link[p][(nowDir+2)%8] < 0 &&link[p][(nowDir+3)%8] < 0) rd = -4;
	 	if(rd < 0 && link[p][(nowDir+7)%8] < 0 && link[p][(nowDir+6)%8] < 0 &&link[p][(nowDir+5)%8] < 0) rd = 4;
	}
	return rd * 45;
}


int GetNextDir(int p){
	int prior[8] = {0,1,7,2,6,3,5,4};

	for(int i = 0 ; i < 8 ; i++){
		int wd = (nowDir + prior[i])%8;
		if(link[p][wd] >= 0) return wd;
	}
}

void MovePos(int mode, int p, int np){
	CheckDFS(p,np,map);

	for(int i = 0 ; i < min ; i++)	MovePtoP(mode && i == min-1,mode,g_route[i],g_route[i+1]);

	if(mode == 1) nowPos = g_route[min-1];
	else nowPos = np;
}

void MovePtoP(int mode,int have, int p, int np){
	lcs lcd(1,1,"%d -> %d",p,np);	wait
	int wd = GetDir(p,np);
	
	if(wd != nowDir){
		if(have == 0) Axis(0,0,GetAng(p,wd),150,130);
		else	Axis(0,0,GetAng(p,wd),60,130); 
		
		//FixAngle(wd*45,10);
		nowDir = wd;
	
		MLC();
	}
	
	if(mode == 0){
		int wwd = GetNextDir(np);
		int ang = GetAng(np,wwd);

		if(wd != wwd){
			
			if(abs(ang) > 90 && (np == 0 || np == 2) && wd % 2){
				ang = (ang > 0)? ang - 45 : ang + 45;
				MoveLine(10,0,distance[wd%2]-25,30,140);	wa = (ang > 0)? -90 : 90;
				//FixAngle(wd*45,10);

				Ot(-25,(ang > 0)? 45 : -45,(ang > 0)? 45 : -45,30,140);		cw
				
				Od2(0,25,ang,30,140,100,120);	
			}			
			else{
				MoveLine(10,0,distance[wd%2]-30,30,40);
				Od2(0,30,ang,30,-40,100,120);	cw
			}
		}
		else	MoveLine(10,0,distance[wd%2],40,150); 		
	
		//FixAngle(wwd*45,10);
		MLC();
		nowDir = wwd;
		nowPos = np;
	}

	else if(mode == 1){
		pos[1] = 0;
		MoveLine(10,0,distance[wd%2]-24,30,140);
		//FixAngle(wd*45,10);
		Axis(0,-distance[wd%2] + 24,0,30,140);
		
		MLC();	
	}
	else{
		MovePtoP(0,0,p,np);
		MovePtoP(1,1,np,p);
	}
}


void dis(){
	lcs
	lcd(0,0,"%d %d %d / %d %d",map[0],map[1],map[2],map[3],map[4]);
	lcd(1,0,"%d %d %d / %d %d",map[5],map[6],map[7],map[8],map[9]);
	lcd(2,0,"%d %d %d",map[10],map[11],map[12]);
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
	TM(10);
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

//
ar ab245(30);
ar wa = 90; AB(1,125,4,194,5,163,50,50); cw
set

At(-80,100);	wa = -80;


case 2:
for(int i = 0 ; i < 3 ; i++){
	bMap[4 - 2*i] = Scan(92,126);
	bMap[5 - 2*i] = Scan(129,69);

	CPsd(12,3,5,3,17,0,50,60);
	CPsd(10,3,40 + (i == 2) * 10,5,17,0,50,-60);
}
lcs
lcd(0,0,"%d %d",bMap[0],bMap[1]);
lcd(1,0,"%d %d",bMap[2],bMap[3]);
lcd(2,0,"%d %d",bMap[4],bMap[5]);
wait

At(10,15);
map[12] = Scan(134,45);


At(20,30);
map[11] = Scan(168,87);


At(35,40);		cw
map[8] = Scan(183,84);
map[10] = Scan(93,160);
At(15,20);		cw

SS();


dis(map);

gate = -1;

for(int i = 10 ; i < 12 ; i++)	if(!map[i]) gate = i;

min = 1;

if(gate == 10 || gate == 11){
	CPsd(12,1,5,3,17,0,50,60);
	CPsd(10,1,17,5,17,0,50,0);
	Od2(0,30,-90,40,-50,60,130);	nowDir = 6;
	MLC();
	
	if(gate == 11)	{
		MovePtoP(1,1,10,11);
		swap(map[10],map[11]);
	}
	MovePtoP((map[8])? 2 : 0,1,10,8);   	
	swap(map[10],map[8]);
	
}
else{
	Drift(10,0,-10,50,60);	wa = -10;
	Axis(120,0,0,60,0);
	MovePsd(0,7,1000,0,0,20,60,0);	
	Drift(10,0,10,50,-60);	cw

	CPsd(12,8,5,3,24,0,50,60);
	CPsd(10,8,20,5,24,0,50,0);
	Od2(0,30,90,40,-50,60,130);	nowDir = 2;
	MLC();

	
	MovePtoP((map[11])? 2 : 0,1,12,11); 
	swap(map[12],map[11]);
	MovePtoP((map[8])? 2 : 0,1,11,8);   	
	swap(map[8],map[11]);
}

nowPos = 8;


Axis(0,0,GetAng(nowPos,1),60,130); MLC();
//FixAngle(1 * 45,10);
nowDir = 1;
map[5] = Scan(141,156);
dis(map);
Axis(0,0,GetAng(nowPos,0),60,130);
//FixAngle(0 * 45,10);
nowDir = 0;
map[0] = Scan(69,71);
map[1] = Scan(233,71);
map[3] = Scan(152,112);
dis(map);
Axis(0,0,GetAng(nowPos,7),60,130); MLC();
//FixAngle(7 * 45,10);
nowDir = 7;
map[2] = Scan(139,46);
map[4] = Scan(143,73);
map[6] = Scan(142,147);
dis(map);

Axis(0,0,GetAng(nowPos,6),60,130);
//FixAngle(6 * 45,10);
nowDir = 6;

map[7] = Scan(49,71);
map[9] = Scan(132,105);


dis(map);
wait

stage = 0;

for(int i = 0,val = 0; i < 13 ; i++)
	if(!map[i]) map[i] = val--;

	
for(int k = 0 ; k < 2 ; k++){
	stage = k;
	memset(hist,0,sizeof(hist));
	
	for(int i = 0 ; i < 50 ; i+=3){
		min = i;
		flg = 0;
		Sort(-1,nowPos,0,map);
		if(flg) break;
	}
	dis(map);
	
	for(int i = 0 ; i < min ; i++){
		lcs lcd(1,1,"%d %d",g_route[i],g_route[i+1]);	wait

		dis(map);	wait
		MovePtoP((map[g_route[i+1]] > 0)? 2 : 0,0,g_route[i],g_route[i+1]);
		swap(map[g_route[i]],map[g_route[i+1]]);
	}

	nowPos = g_route[min];
	SS();
}

LED_ON(3);
_delay_ms(100);
LED_OFF(3);


fix[0] = fix[2] = fix[10] = fix[12] = 1;


for(int i = 0; i < 10 ; i++){
	sort_min = i;
	flg = 0;
	Final(-1,nowPos,0,map);
	if(flg) break;
}


LED_ON(3);
_delay_ms(100);
LED_OFF(3);

_delay_ms(100);

LED_ON(3);
_delay_ms(100);
LED_OFF(3);


for(int i = 0 ; i < sort_min ; i++){
	lcs lcd(1,1,"%d -> %d - > %d",nowPos,g_sort_route[i][0],g_sort_route[i][1]);	wait
	MovePos(0,nowPos,g_sort_route[i][0]);
	MovePos(1,g_sort_route[i][0],g_sort_route[i][1]);
	swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);
	//nowPos = g_route[min-1];
	
}
dis(map);





///

SS();

int getOrder[4],getPrior[2][6] = {{0,2,4,1,3,5},{1,3,5,0,2,4}};

for(int i = 0 ; i < 4 ; i++){
	for(int j = 0 ; j < 6 ; j++){
		if(bMap[getPrior[i%2][j]] == map[10*(i/2)]){
			bMap[getPrior[i%2][j]] = 0;
			getOrder[i] = getPrior[i%2][j]; // 먹는 퍽의 인덱스 
			break;
		}
	}
}

lcs lcd(1,1,"%d %d %d %d",getOrder[0],getOrder[1],getOrder[2],getOrder[3]);	wait
GetOut(nowPos,getOrder[0]%2); //첫번째 먹는 곳으로 나가기 


for(int i = 0 ; i < 4;  i++){
	GetPuck(getOrder[i]);
	PutPuck(i,getOrder[i],(i == 3)? 0 : getOrder[i+1]);
}

SS();

LED_ON(3);
_delay_ms(1000);
LED_OFF(3);

case 3:

GetOut(3,0);	wait
GetOut(3,1);	wait

GetOut(4,0);	wait
GetOut(4,1);	wait


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


int Scan(int x, int y){ //limit size 	
	SS();
	if(!T(101,3)) return 0; //색 감지 안되면 그냥 스킵 	

	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;

			if(abs(pro[i][1] - x) < 30 && abs(pro[i][2] - y) < 20) return pro[i][0];
		}
	}

	return 0;
}

void MLC(){
	if(MoveLineCheck(0,2,0,0,10,120))	{Axis(1,0,0,10,-20); return;}
	if(MoveLineCheck(0,-4,0,0,10,120))	{Axis(-1,0,0,10,-20); return;}
	if(MoveLineCheck(0,6,0,0,10,120))	{Axis(1,0,0,10,-20); return;}
	if(MoveLineCheck(0,-8,0,0,10,120))	{Axis(-1,0,0,10,-20); return;}
	if(MoveLineCheck(0,10,0,0,10,120))	{Axis(1,0,0,10,-20); return;}
	if(MoveLineCheck(0,-12,0,0,10,120))	{Axis(-1,0,0,10,-20); return;}
	if(MoveLineCheck(0,14,0,0,10,120))	{Axis(1,0,0,10,-20); return;}
	if(MoveLineCheck(0,-16,0,0,10,120))	{Axis(-1,0,0,10,-20); return;}
}


void GetOut(int index, int toDir){
	if(index == 3 || index == 8){
		if(toDir == 0){
			Axis(0,0,GetAng(nowPos,0) -20,130,120);	wa = -20;

			Avoid(5,14,18,0,-135 + 75 * (index / 5),60,70);

			Ot(-20,-90,-10,50,0);	wa = 60;
			MovePsd(0,3,0,-1000,0,23,50,0);
			Ot(-10,90,10,45,0);	wa = -20;
			
			CPsd(12,4,-5,3,18,0,50,0);
			CPsd(10,4,-10,5,18,0,50,0);
			CPsd(-5,4,-1000,5,18,25,60,0);
			Ad(0,-10,-70,60,-70,60);	cw

			ar ab745(20);
		}
		else{
			Axis(0,0,GetAng(nowPos,0) + 20,100,120);	wa = 20;

			Avoid(4,14,18,0,-135 + 75 * (index / 5),60,70);
			
			Ot(-30,90,50,45,0);	wa = -20;
			
			Avoid(5,14,18,0,-50,50,0);
			Avoid(5,14,18,23,-1000,50,0);
			
			Ot(-10,-90,0,45,0);	
			Drift(-20,0,-50,40,0);	wa= 20;

			CPsd(12,5,-5,3,16,0,60,0);
			CPsd(10,5,-10,5,16,0,60,0);
			CPsd(-4,5,-1000,5,16,25,60,0);
			Ad(0,-10,70,50,-60,80);	cw

			ar ab245(20);
		}
	}

	if(index == 4 || index == 9){
		if(toDir == 1){
			Axis(0,0,GetAng(nowPos,0) + 20,130,120);	wa = 20;

			Avoid(4,14,18,0,-135 + 75 * (index / 5),60,70);

			Ot(-20,90,10,50,0);	wa = -60;
			MovePsd(0,6,0,1000,0,23,50,0);
			Ot(-10,-90,-10,45,0);	wa = 20;

			CPsd(12,5,-5,3,14,0,60,0);
			CPsd(10,5,-120,5,14,0,60,0);
			CPsd(-4,5,-1000,5,14,25,60,0);
			Ad(0,-10,70,60,-70,60);	cw

			ar ab245(20);
		}
		else{
			Axis(0,0,GetAng(nowPos,0) - 20,100,120);	wa = -20;

			Avoid(4,14,18,0,-130 + 75 * (index / 5),60,70);
			Ot(-30,-90,-50,50,0);	wa = 20;
			Avoid(4,14,18,0,-70,50,0);
			Avoid(4,14,18,33,-1000,50,0);
			
			Ot(-10,90,0,45,0);	
			Drift(20,0,50,40,0);	wa= -20;

			CPsd(12,4,-5,3,18,0,60,0);
			CPsd(10,4,-120,5,18,0,60,0);
			CPsd(-5,4,-1000,5,18,25,60,0);
			Ad(0,-10,20,50,-60,40);	cw

			ar ab745(20);
		}
	}
	
	ar ab45(20);
	SS();
}

void GetPuck(int index){
	Setting(11);


	if(index % 2){
		
		ar ab245(20);
		
		At(-10,10);	wa = 80;
		CPsd(12,6,5,3,18,0,40,50);
		CPsd(10,6,5 + 45 * (2 - index/2),5,18,0,40,0);
		Drift(0,10,10,40,-50);	cw	

		ar TB(T(103,3),0,129,157);

		Axis(0,10,0,30,40);
		Od2(0,10,-100,30,-40,60,130);	wa = -10;

		MovePsd(0,7,1000,0,0,20,30,40);	wa = 80;
		Ot(10,90,10,35,0);	cw

		CPsd(12,8,5,3,24,0,50,0);
		CPsd(10,8,160 - 45 - 45*(2 - index/2),5,24,0,50,-60);
	}

	else{
		ar ab745(20);
		
		At(10,10);	wa = -80;
		CPsd(12,3,5,3,14,0,40,50);
		CPsd(10,3,5 + 45 * (2 - index/2),5,14,0,40,0);
		Drift(0,10,-10,40,-50);	cw	

		ar TB(T(103,3),0,129,157);

		Axis(0,10,0,30,40);
		Od2(0,10,100,30,-40,60,130);	wa = 10;

		MovePsd(0,2,-1000,0,0,20,30,40);	wa = -80;
		Ot(10,-90,-10,35,0);	cw

		CPsd(12,1,5,3,17,0,50,0);
		CPsd(10,1,160 - 45 - 45*(2 - index/2),5,17,0,50,-60);
	}
}

void PutPuck(int index, int getIndex, int outIndex){
	if(getIndex % 2 == 0)	{ar AB(0,0,1,112,3,69,30,0); }
	else			{ar AB(0,0,6,83,8,81,30,0); }
	
	if(index % 2 != getIndex %2){
		if(index % 2 == 0){
			Drift(-10,0,10,40,50);	wa =  10;
			Axis(-130,0,0,40,0);
			MovePsd(0,2,-1000,0,0,20,40,0);
			Drift(-10,0,-10,40,-50);	cw
		}
		else{
			Drift(10,0,-10,40,50);	wa =  -10;
			Axis(130,0,0,40,0);
			MovePsd(0,7,1000,0,0,20,40,0);
			Drift(10,0,10,40,-50);	cw	
		}

		SS();
	}
	
	if(index % 2 == 0){
		ar AB(0,0,1,112,3,69,30,0); 
		Axis(35,0,0,30,140);	
		MoveLineCheck(0,0,1000,0,50,60);
		Axis(0,106 - 75 * (index / 2),0,50,-60);	
	}
	else if(index % 2){
		ar AB(0,0,6,83,8,81,30,0);
		Axis(-37,0,0,30,140);	
		MoveLineCheck(0,0,1000,0,50,60);
		Axis(0,106 - 77 * (index / 2),0,50,-60);	
	}	
	
	////////////////
	Axis(0,-5 -70 * (index / 2 == 0),0,50,60);
	MoveLineCheck(0,0,-1000,0,50,0);


	if(outIndex % 2 == 0){
		if(index % 2 == 0){		
			Ot(-20,-90,-40,50,0);	wa = 50;
			Ot(-18,90,-20,50,0);	wa = -60;		
		}
		else{
			Ot(-30,-90,-70,50,0);	wa = 20;
			Avoid(4,14,18,0,-70,50,0);
			Avoid(4,14,18,20,-1000,50,0);
			Ot(-10,90,10,40,0);	wa = -60;		
		}

		CPsd(12,5,-5,3,18,0,50,0);
		CPsd(10,5,-120,5,18,0,50,0);
		CPsd(-6,5,-1000,5,18,25,50,0);
		Drift(0,-10,-30,50,-60);	cw

		ar ab745(20);
	}
	else{
		if(index % 2 == 0){
			Ot(-30,90,70,50,0);	wa = -20;
			Avoid(5,14,18,0,-70,50,0);
			Avoid(5,14,18,20,-1000,50,0);
			Ot(-10,-90,-10,40,0);	wa = 60;
		}
		else{
			Ot(-20,90,40,50,0);	wa = -50;
			Ot(-18,-90,20,50,0);	wa = 60;
		}
		CPsd(12,4,-5,3,14,0,50,0);
		CPsd(10,4,-120,5,14,0,50,0);
		CPsd(-3,4,-1000,5,14,25,50,0);
		Drift(0,-10,30,50,-60);	cw

		ar ab245(20);
	}
}

