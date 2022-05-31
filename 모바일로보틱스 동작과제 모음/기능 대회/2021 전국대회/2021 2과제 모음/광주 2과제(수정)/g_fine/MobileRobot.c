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

int map[16] = {0};

int mapGoal[16] = {0};
int test[16];

int main_route[4],main_g_route[4],main_min;
int cWay[4][4],cPos[4][4]; //cWay[dp / color]

int link[16][8],sub_link[8][4];
int sub_map[8] = {0};
int cGate[5][4],order[5][3];
int pList[5][4][12];  //dp / color / np
int sub_mapGoal[8];


signed short min,route[13],g_route[13],check[16];
signed short sort_min,flg,sort_route[30],g_sort_route[30];
signed short indexToGate[4],gateToIndex[4],block = 8;  
signed short nowDir,nowPos,robotArea = 0,goalTotal;
//robotArea 0 --> Home 1 -> SUB 2 -> Map


void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 8);
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[2] = p - 1;
	if(p / 4 < 3) str[4] = p + 4;
	if(p % 4 < 3) str[6] = p + 1;
	
	
	if(str[0] >= 0 && str[2] >= 0) str[1] = p - 5;
	if(str[4] >= 0 && str[2] >= 0) str[3] = p + 3;
	if(str[4] >= 0 && str[6] >= 0) str[5] = p + 5;
	if(str[0] >= 0 && str[6] >= 0) str[7] = p - 3;
	
}

void SubLink(int p, int *str){
	memset(str,-1,sizeof(int) * 8);

	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 == 0) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}

int GetWay(int p, int np){
	if(abs(p - np) == 4) return 0; //Verical 
	if(abs(p - np) == 1) return 1; //Horizontal
	return 2;	//diagonal(대각선)
}

int Sorted(int *node, int *goal){
	for(int i = 0 ; i < 8 ; i++){
		if(goal[i] < 0) continue;
		if(goal[i] != node[i]) return 0;
	}
	return 1;
}
void Sort(int lp, int p, int dp, int *node){
	if(dp >= sort_min || flg) return;
	
	sort_route[dp] = p;
	
	if(Sorted(node,sub_mapGoal)){
		sort_min = dp;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 4 ; i++){
		int np = sub_link[p][i];
		if(np < 0 || lp == np) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);	
		swap(node[p],node[np]);
	}
}


int dfsPrior[8] = {0,2,6,4,1,3,7,5};

void DFS(int way, int rock, int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int d = dfsPrior[i];
		int np = link[p][d];

		if(way == 0 && (d != 0 && d != 4)) continue;
		if(way == 1 && (d != 2 && d != 6)) continue;
		if(way == 2 && d%2 == 0) continue;
		if(way == 3 && np != wp  && node[np]) continue;
		if(np < 0 || check[np] || node[np] == 4) continue; 
		if(rock && node[np]) continue;
		
		DFS(way,rock,np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int way, int rock, int p, int wp, int *node){
	min = 12;
	memset(check,0,sizeof(check));
	DFS(way,rock,p,wp,0,node);
	return min != 12;
}


int GetNextCount(int way, int cPos, int *node, int *list){
	int cnt = 0;

	for(int i = 0 ; i < 16 ; i++){
		if(way == 0 && (i % 4 != cPos % 4)) continue;
		if(way == 1 && (i / 4 != cPos / 4)) continue;
		if(way == 2 && ((abs(i/4 + i % 4) != abs(cPos / 4 + cPos % 4)) && (abs(i / 4 - i % 4) != abs(cPos / 4 - cPos % 4)))) continue;
		if(i == cPos || node[i] || !CheckDFS(way,0,cPos,i,node)) continue;
		
		list[cnt++] = i;
	}
	
	return cnt;
}





int IsUsedWay(int color, int way, int dp){
	for(int i = 1 ; i < color ; i++)
		if(cWay[dp][i] == way) return 1;
	
	return 0;
}
int IsUsedGate(int color, int gate, int dp){
	for(int i = 1 ; i < color ; i++)
		if(cGate[dp][i] == gate) return 1;
	
	return 0;
}

int MakeOrder(int dp, int *list, int *node){ //이동 가능하게 만드는지 검사 
	int o[3]; //order
	
	memcpy(test,node,sizeof(test));
	
	for(int k = 3; k >= 1 ; k--){
		swap(test[cPos[dp][k]],test[cPos[dp+1][k]]);
	}
	
	//display(test);
	for(o[0] = 1 ; o[0] <= 3 ; o[0] ++){
		for(o[1] = 1 ; o[1] <= 3 ; o[1] ++){
			if(o[0] == o[1]) continue;
			for(o[2] = 1 ; o[2] <= 3 ; o[2] ++){
				if(o[2] == o[1] || o[2] == o[0]) continue;
				
				//display(test);
				int moved[3] = {0};
				
				for(int k = 0; k < 3 ; k++){
					if(!CheckDFS(cWay[dp][o[k]],1,cPos[dp][o[k]],cPos[dp+1][o[k]],test)) break;
					if(cPos[dp+1][o[k]] < 0) continue;
					moved[k] = 1;
					swap(test[cPos[dp][o[k]]],test[cPos[dp+1][o[k]]]);
					
					if(k == 2) {
						memcpy(list,o,sizeof(o));
						return 1;
					}	
				}
				
				for(int k = 0; k < 3 ; k++)	{
					if(!moved[k]) continue;
					swap(test[cPos[dp][o[k]]],test[cPos[dp+1][o[k]]]);
				}
			}
		}
	}
	return 0;
}

void MainSortDFS(int color, int dp, int *node){ //깊이별 색 위치 /  
	if(dp > main_min || flg) return;		
	
	if(!color && !MakeOrder(dp,order[dp],node)) return;
	
	if(!color && Sorted(node,mapGoal)){
		main_min = dp + 1;
			
		flg = 1;
		return;
	}
	
	if(dp == main_min) return;
	if(color == 0)	MainSortDFS(1,dp + 1,node);
	
	else{
		int p = cPos[dp][color],cnt = 1;
		
		for(int i = 0; i < 3 && !flg; i++){
			
			
			if(IsUsedWay(color,i,dp)) continue;
			if(mapGoal[p] == color){ 
				if(i) continue;
				pList[dp][color][0] = -1;
				cWay[dp][color] = -1;
			}
			else cnt = GetNextCount(i,p,node,pList[dp][color]);
			
			if(dp && cWay[dp-1][color] == i) continue;  
			
			cWay[dp][color] = i; //색깔 방향 
		
			for(int j = 0 ; j < cnt && !flg; j++){	
				int np = cPos[dp+1][color] = pList[dp][color][j];
				if(node[np]) continue; //이미 채워져있으면 버림 
				
				cGate[dp][color] = -1;
				if(i != 1 && np >= 0 && p / 4 >= 2 && np / 4 <= 2){
					
					if(i == 0) cGate[dp][color] = indexToGate[p % 4]; //사용하는 게이트  
					
					else if(i == 2) {
						if(p % 4 + p / 4 == np % 4 + np / 4){
							for(int k = 0 ; k < 4 ; k++)
								if(2 + k == p / 4 + p % 4) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트  
						} 
						else{
							for(int k = 0 ; k < 4 ; k++)
								if(abs(2 - k) == abs(p / 4 - p % 4)) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트
						}
					}

					if(IsUsedGate(color,cGate[dp][color],dp))  continue;
				}
				
				if(pList[j] >= 0){
					swap(node[p],node[np]);
				}
				//display(map);
				MainSortDFS((color+1)%4,dp,node);
				
				if(pList[j] >= 0){
					swap(node[p],node[np]);
				}
			}
		}
	}
}



int GetDir(int p, int np){
	for(int i = 0 ; i < 8 ; i++)
		if(link[p][i] == np) return i;

	return -1;
}

int GetDirSub(int p, int np){
	for(int i = 0 ; i < 4 ; i++)
		if(sub_link[p][i] == np) return i;
}

// nowAng == 180
//rd == 0

int CalGyro(int nowAng){

	get_gyro();
	int nowGyro = (int)(-gyro) % 360;
	nowGyro = (nowGyro >= 0)? nowGyro : nowGyro + 360;		

	if(nowGyro > 180 && nowAng == 0) nowAng = 360;
	lcs lcd(1,0,"gyro %d ang %d",nowGyro,nowAng); 
	//lcd(2,0,"gyro %d",(int)gyro); wait

	return (abs(nowAng - nowGyro) < 30)? nowAng - nowGyro :  0;
}

int GetAng(int p, int wd){
	int rd = wd - nowDir;

	if(abs(rd) > 4) rd = (rd > 0)? rd - 8 : 8 + rd;
	if(abs(rd) > 3){
		if(rd < 0 && link[p][(nowDir + 7)%4] < 0 && link[p][(nowDir + 6)%4] < 0 && link[p][(nowDir + 5)%4] < 0)	rd = rd + 8;
		if(rd > 0 && link[p][(nowDir + 1)%4] < 0 && link[p][(nowDir + 2)%4] < 0 && link[p][(nowDir + 3)%4] < 0)	rd = rd - 8;
	}
	return rd * 45;
}


int GetAngSub(int p, int wd){
	int rd = wd - nowDir;
	
	if(abs(rd) == 3) rd /= -3;
	if(abs(rd) == 2){
		rd = (sub_link[p][(nowDir+1)%4] >= 0)?  2 : -2;
	}
	return rd * 90;
}

int GetNextDir(int p, int d){
	int prior[2][4] = {
		{0,2,6,4},
		{1,7,3,5}
	};

	for(int i = 0 ; i < 4; i++){
		int nd = (nowDir + prior[nowDir%2][i])%8;
		if(link[p][nd] >= 0) return nd;
	}
}
int GetNextDirSub(int p){
	int prior[4] = {0,1,3,2};

	for(int i = 0 ; i < 4; i++){
		int nd = (nowDir + prior[i])%4;

		if(sub_link[p][nd] >= 0) return nd;
	}
}

void Move(int mode, int way, int rock, int p, int wp){
	lcs lcd(1,1,"P : %d NP : %d",p,wp);	wait
	CheckDFS(way,rock,p,wp,map);
	
	for(int i = 0 ; i < min ; i++)
		MovePos(mode,mode && i == min - 1,g_route[i],g_route[i+1]);

	if(mode) swap(map[p],map[wp]);
	
}

void MovePos(int mode, int push, int p, int np){
	//mode 0 : getPuck
	//mode 1 : putPuck
	lcs lcd(1,1,"%d %d->%d / %d",mode,p,np);
	
	int wd = GetDir(p,np);	
	int cm = wd % 2;

	if(nowDir != wd){
		int rAng = GetAng(p,wd); SS();
		if(mode)
			Axis(0,0,rAng,60,130); 
		else
			Axis(0,0,rAng,150,130); 

		At(CalGyro(wd * 45),20);
		nowDir = wd;
		if(!cm) MLC();
	}
	
	if(push){
		if(!cm){
			pos[1] = 0;
			MoveLine(10,0,26,30,140);
			Axis(0,-25,0,30,140);
			At(CalGyro(wd * 45),20);
			MLC();
		}
		else{
			Axis(0,48,0,40,150);
			Axis(0,-48,0,40,150);
			At(CalGyro(wd * 45),20);	
		}
		return;
	}
	
	int wwd = GetNextDir(np,nowDir);
	

	if(wd != wwd){
		if(!cm){
			MoveLine(10,0,25,30,40);
			Ad(0,25,GetAng(p,wwd),30,-40,100);
		}
		else{
			
			int rAng =  GetAng(p,wwd);
			
			if(abs(rAng) > 90){
				rAng = (rAng > 0)? rAng - 45 : rAng + 45;
				
				Axis(0,42,0,30,140);	wa = (rAng > 0)? -90 : 90;
				Ot(-20,(rAng > 0)? 45 : -45,(rAng > 0)? 45 : -45,15,125);	cw MoveLineCheck(0,(rAng > 0)? 1000 : -1000,0,0,20,130);
				Drift(0,25,rAng,30,140);
				MLC();
			}
			else{
				Axis(0,35,0,30,40);
				Ad(0,35,rAng,30,-40,100);	cw
			}
		}
		At(CalGyro(wwd * 45),20);
		MLC();
		
		nowDir = wwd;
	}
	else{
		if(mapGoal[np]){ //시트지 지날 때 
			MoveLine(10,0,50,30,140);
		}
		else{
			MoveLine(1,0,1000,30,40);
			MoveLine(10,0,14,30,-40);
		}	
		At(CalGyro(wwd * 45),20);
		MLC();
	}
	nowPos = np;
	
}

void MovePosSub(int p, int np){
	
	int wd = GetDirSub(p,np);	

	if(nowDir != wd){
		Axis(0,0,GetAngSub(p,wd),150,130);
		MLC();
		nowDir = wd;
	}
	
	if(sub_map[np] > 0){
		//SWAP
		MLC();
		MoveLine(10,0,20,30,40);
		Od2(0,25,GetAngSub(np,(nowDir+2)%4),30,-40,80,130);
		At(CalGyro(((nowDir+2)%4) * 90),20);
		MLC();

		MoveLine(10,0,22,30,140);
		Axis(0,-22,0,30,140);

		MLC();
		nowDir = (nowDir + 2) % 4;
	}

	else{
		int wwd = GetNextDirSub(np);
	
		if(wd != wwd){
			MoveLine(10,0,15,30,40);
			Od2(0,30,GetAngSub(p,wwd),30,-40,150,130);
			MLC();

			nowDir = wwd;
		}
		else{
			MoveLine(1,0,1000,40,50);
			MoveLine(10,0,14,40,-50);
		}

	}
	swap(sub_map[p],sub_map[np]);
	nowPos = np;
}

int SetSubSort(int dp, int *node){
	int null[2] = {0};
	memset(sub_mapGoal,-1,sizeof(sub_mapGoal));
	
	for(int i = 0,cnt = 0 ; i < 8 ; i++){
		if(node[i] <= 0) null[cnt++] = i;
		
		for(int j = 1; j <= 3 ; j++){
			if(cGate[dp][j] >= 0)
				sub_mapGoal[cGate[dp][j]] = j;
			if(cWay[dp][j] >= 0)
				sub_mapGoal[cWay[dp][j] + 4] = j;
		}
	}
		
	for(int i = 0 ; i < 25 ; i++){
		for(int j = 0 ; j < 2 ; j++){
			flg = 0;
			sort_min = i;
			Sort(-1,null[j],0,node);
			if(flg) return min;
		}
	}
	return -1;
}

//////////////////


int main(void)
{

    Interface_init();

	write_gyro(0x1E, 1); // 지자기 OFF
	write_gyro(0x1E, 5); // 자이로 500dps	
	write_gyro(0x1E, 20); // 능동적 진동성분제거 OFF

	write_gyro(0x1E, 16); // 자이로 보정

	Camera_init();
	Setting(11);
	TM(20);
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

ar ab45(20); SS();
set

map[13] = Scan(40,77);
map[14] = Scan(206,77);



At(-40,60);	 SS();
map[15] = Scan(139,39);  


At(80,60);	wa = 40; SS();
map[12] = Scan(101,50); 


Drift(-75,10,-40,50,160); 	cw 
ar ab13(20);	SS();

wa = 90;
Ot(30,90,0,45,55);	cw
Avoid(0,14,18,0,60,50,0);
Avoid(0,14,18,0,50,50,0);

block = -1;
if(psdCheck == 1) block = 8;
else if(psdCheck == 2 && block < 0) block = 9;

MoveLineCheck(0,0,1000,0,50,0);
Ot(14,90,0,40,0);
Axis(-5,0,0,40,-50);
ar ab13(30); SS();

MoveLineCheck(0,100,0,0,10,120); SS();

mapGoal[0] = Scan(124,136);
Axis(0,0,-90,130,120);
ar ab45(30); SS();

MLC();SS();

mapGoal[5] = Scan(124,136);
mapGoal[6] = Scan(128,53);
mapGoal[7] = Scan(131,25);

case 2:

Axis(-50,0,0,40,150);
ar ab245(30); 
MoveLineCheck(0,100,0,0,10,120); SS();

mapGoal[1] = Scan(124,136);
mapGoal[2] = Scan(128,53);
mapGoal[3] = Scan(131,25);

wa = 90;
Ot(16,90,0,40,50);	cw

CPsd(12,1,5,3,30,0,50,0);
CPsd(10,1,40,5,30,0,50,0);
CPsd(13,1,1000,5,30,0,50,0);

Ot(14 + 25,-90,70,45,0);	wa = -20;
Axis(0,-20,0,50,0);
Avoid(5,14,18,0,-60,50,0);

if(psdCheck == 1 && block < 0) block = 10;
else if(psdCheck == 2 && block < 0) block = 11;


Avoid(5,14,18,0,-62,50,0);
Ot(-21,-90,-10,45,0);	wa = 60;

CPsd(12,4,-5,3,14,0,40,0);
CPsd(13,4,-1000,3,14,20,40,0);
CPsd(10,4,-9,5,14,0,40,0);
Ad(0,-10,30,40,-50,40);	cw

ar ab45(30);
SS();

map[block] = 4;
mapGoal[4] = 6;
for(int i = 0 ; i < 8 ; i++)	{
	if(i == 4) continue;
	mapGoal[4] -= mapGoal[i];
}

lcs
lcd(0,0,"%d %d %d %d",mapGoal[0],mapGoal[1],mapGoal[2],mapGoal[3]);
lcd(1,0,"%d %d %d %d",mapGoal[4],mapGoal[5],mapGoal[6],mapGoal[7]);
wait
lcs
lcd(0,0,"%d %d %d %d",map[8],map[9],map[10],map[11]);
lcd(1,0,"%d %d %d %d",map[12],map[13],map[14],map[15]);
wait
//Mirror

GoSub(1);                          //Scan
wait


///////////

for(int i = 0 ; i < 16 ; i++) {
	Link(i,link[i]);
	cPos[0][map[i]] = i;
}
for(int i = 0 ; i < 8 ; i++) SubLink(i,sub_link[i]);

for(int i = 8,cnt = 0 ; i < 12 ; i++){
	if(map[i] == 4) {
		block = i;
		indexToGate[i%4] = -1;
		continue;
	}	
	indexToGate[i%4] = cnt;
	gateToIndex[cnt++] = i;
}


SS();




for(int i = 1; i <= 4 ; i++){
	main_min = i;
	MainSortDFS(1,0,map);
	if(flg) break;
}

SS();
LED_ON(3);
_delay_ms(1000);
LED_OFF(3);


for(int i = 0 ; i < main_min ; i++){
	memset(sub_mapGoal,-1,sizeof(sub_mapGoal));
	
	for(int j = 1 ; j <= 3 ; j++){
		if(cGate[i][j] >= 0)
			sub_mapGoal[gateToIndex[cGate[i][j]]] = j;
		if(cWay[i][j] >= 0)
			sub_mapGoal[cWay[i][j] + 4] = j;
	}

	SetSubSort(i,sub_map);
	
	if(sort_min){
		if(robotArea == -1) GoSub(0);
		lcs lcd(1,1,"GETINSUB : %d",g_sort_route[0]);
		wait

		GetInSub(g_sort_route[0]);

		for(int j = 0 ; j < sort_min ; j++)
			MovePosSub(g_sort_route[j],g_sort_route[j+1]);
	}

	if(robotArea == 1)
		GetOutSub(nowPos);
	
	for(int j = 0 ; j < 3 ; j++){
		int color = order[i][j]; 
		int p = cPos[i][color];
		int wp = cPos[i+1][color];
		int way = cWay[i][color];
		
		if(!j){
			if(!i) GetIn(p);
			else{
				for(int k = 12 ; k < 16 ; k++)
					if(!map[k])	{GetIn(k); break;}
			}
		}
		lcs
		Move(0,3,0,nowPos,p);
		Move(1,cWay[i][color],1,p,wp);
	}
	GetOut();
}

SS();

LED_ON(3);
_delay_ms(1000);
LED_OFF(3);



///////////



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

			if(abs(pro[i][1] - x) < 25 && abs(pro[i][2] - y) < 15 ) return pro[i][0];
		}
	}

	return 0;
}

void MLC(){
	if(MoveLineCheck(0,2,0,0,10,120)) return;
	if(MoveLineCheck(0,-3,0,0,10,120)) return;
	if(MoveLineCheck(0,4,0,0,10,120)) return;
	if(MoveLineCheck(0,-5,0,0,10,120)) return;
	if(MoveLineCheck(0,6,0,0,10,120)) return;
	if(MoveLineCheck(0,-7,0,0,10,120)) return;
	if(MoveLineCheck(0,8,0,0,10,120)) return;
	if(MoveLineCheck(0,-9,0,0,10,120)) return;
	if(MoveLineCheck(0,10,0,0,10,120)) return;
}


void GetIn(int p){
	robotArea = 0;
	nowPos = p;
	nowDir = 0;
	
	ar ab45(20);
	set

	if(p % 4 == 0){
		Axis(-75,20,0,50,160);
		ar ab13(20);
		CPsd(12,1,5,3,16,0,40,50);
		CPsd(10,1,47,5,16,0,40,-50);
		MLC();
	}
	else if(p % 4 == 1){
		Axis(-15,0,0,30,40);
		MoveLineCheck(0,-12,0,0,30,-40);
		Axis(0,73,0,40,150);
		MLC();
	}
	else if(p % 4 == 2){
		Axis(15,0,0,30,40);
		MoveLineCheck(0,12,0,0,30,-40);
		Axis(0,73,0,40,150);
		MLC();
	}
	else if(p % 4 == 3){
		Axis(75,20,0,50,160);
		ar ab86(20);
		CPsd(12,8,5,3,20,0,40,50);
		CPsd(10,8,47,5,20,0,40,-50);
		MLC();
	}	
}



void GetOut(){
	lcs
	lcd(1,1,"GETOUTA");
	wait
	//displayA();
	wait
	lcs
	lcd(1,1,"np%d nd%d",nowPos,nowDir);

	int minDist = 10;
	int target = -1;
	
	for(int i = 12 ; i < 16 ;  i++){
		if(!map[i] && CheckDFS(3,0,nowPos,i,map) && (target < 0 || min <= minDist)){
			target = i;
			minDist = min;
		}
	}
	
	Move(0,3,0,nowPos,target); SS();
	

	if(target == 12){
		Axis(0,0,GetAng(nowPos,0) -20,130,120);	cw SS();
		Axis(0,0,CalGyro(0) - 20,10,120); wa = -20;

		CPsd(12,4,-5,3,18,0,50,60);
		CPsd(10,4,-35,5,18,0,50,0);	
		Ot(-30,90,10,45,0);	wa = 80;
	
		CPsd(12,6,5,3,18,0,40,0);

		CPsd(13,6,1000,5,18,0,40,0);
		CPsd(10,6,15,5,18,0,40,0);
		Ad(0,10,10,40,-50,30);	cw
		ar ab45(30);
	}

	else if(target == 13){
		Axis(0,0,GetAng(nowPos,0),130,120);	cw SS();
		Axis(0,0,CalGyro(0),10,120); wa = -20;
		Axis(22,-72,0,50,160);
		ar ab45(30);
	}

	else if(target == 14){
		Axis(0,0,GetAng(nowPos,0),130,120);	cw SS();
		Axis(0,0,CalGyro(0),10,120); wa = -20;
		Axis(-22,-72,0,50,160);
		ar ab45(30);
	}

	else{
		Axis(0,0,GetAng(nowPos,0) + 20,130,120);	cw SS();
		Axis(0,0,CalGyro(0) + 20,10,120); wa = 20;

		CPsd(12,5,-5,3,14,0,50,60);
		CPsd(10,5,-35,5,14,0,50,0);	
		Ot(-30,-90,-10,45,0);	wa = -80;
	
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0);
		CPsd(10,3,17,3,14,0,40,0);
		Ad(0,10,-10,40,-50,30);	cw
		ar ab45(30);
	}
	robotArea = -1;
}



void GoSub(int scan){
	robotArea = 1;
	nowPos = -1;

	ar ab45(20);
	MoveLineCheck(0,1000,0,0,50,60);	wa = 90;
	Ot(22,-90,20,45,0);	wa= 20;
	Avoid(4,11,15,0,-70,50,0);
	Avoid(4,11,15,20,-1000,50,0);
	Ad(0,-12,-20,50,-60,20);	cw
	ar ab45(20); SS();

	if(scan)
		sub_map[3] = Scan(210,92);
	
	Drift(-1,42,70,30,140);	wa=  -20;
	Avoid(5,9,13,20,-1000,40,50);
	Drift(0,-10,20,40,-50);	cw

	ar ab45(20);
	
	if(scan){
		At(-10,20);	
		
		sub_map[0] = Scan(123,15);
		sub_map[1] = Scan(140,41);
		sub_map[2] = Scan(192,101);
		
		At(20,20);	
		
		sub_map[4] = Scan(130,13);
		sub_map[5] = Scan(127,41);
		sub_map[6] = Scan(91,106);

		At(-10,20);	
		SS();

		Axis(0,2,0,10,120);
		ar ab45(20);
		

		sub_map[7] = 12;
		for(int i = 0 ; i < 7 ; i++) 
			sub_map[7] -= sub_map[i];

	}
	
	lcs
	lcd(0,0,"%d %d %d %d",sub_map[0],sub_map[1],sub_map[2],sub_map[3]);
	lcd(1,0,"%d %d %d %d",sub_map[4],sub_map[5],sub_map[6],sub_map[7]);

	wait

	SS();
}


void GetInSub(int p){
	robotArea = 1;
	ar ab45(20);
	ar ab45(20);
	

	nowPos = p;

	int dir = (p / 4)? -1 : 1;

	if(p % 4 == 0){
		LineAvoid(0,9,13,0,80,40,50);
		Avoid(0,9,13,0,10,40,0);
		LineAvoid(0,9,13,0,80,40,0);
	
		if(p / 4 == 0){
			Avoid(0,9,13,0,20,40,0);
			Drift(1 * dir,40,-90 * dir,30,-40);
			MoveLineCheck(0,100,0,0,10,120);
			MoveLine(1,0,1000,30,40);
			MoveLine(10,0,14,30,-40);
		}

		else{
			Avoid(0,9,13,0,24,40,0);
			Drift(0,10,-20,35,0);	wa= -20;
			Ot(25,90,-50,30,0);	wa = 20;
			Ad(0,-5,-20,30,-40,80);	cw 
			ar ab45(20);
	
			MoveLineCheck(0,-100,0,0,20,130);
			MoveLine(1,0,1000,30,40);
			MoveLine(10,0,14,30,-40);
			
			SS();
		}

		nowDir = 0;
	}

	else if(p % 4 == 1){
		LineAvoid(0,9,13,0,80,40,50);
		Avoid(0,10,14,0,10,40,0);
		LineAvoid(0,9,13,0,80,40,0);

		Ot(13,-90 * dir,0,40,0);
		MoveLineCheck(0,8 * dir,0,0,20,-30);
		nowDir = 1;
		SS();
	}

	else if(p % 4 == 2){
		LineAvoid(0,9,13,0,100,40,50);
		Ot(13,-90 * dir,0,30,0);
		MoveLineCheck(0,8 * dir,0,0,20,-30);
		nowDir = 1;
	}

	else if(p % 4 == 3){
		Axis(20 * dir,10,0,30,40);
		MoveLineCheck(0,8 * dir,0,0,20,-30);
		nowDir = 1;
	}
	

	
}

void GetOutSub(int p){
	robotArea = -1;
	if(p / 4 == 0 && p >= 0){
		wa = nowDir * 90;
		Ad(0,-5,GetAngSub(p,0),20,130,100);
		cw
	}
	else if(p >= 0)
		Axis(0,0,GetAngSub(p,0),130,120);
	
	if(p < 0){
		ar ab45(20);
		Drift(0,32,-90,30,140);
		LineAvoid(0,9,13,0,1000,40,50);
		Avoid(0,9,13,0,14,40,0);
		Avoid(0,9,13,0,35 + 10 + 22,40,0);
		Ot(5,90,10,30,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	else if(p == 0){
		CPsd(12,1,5,3,23,0,50,60);
		CPsd(10,1,48,5,23,0,50,0); 
		Ot(25,-90,-10,45,0);	wa = 80;

		CPsd(12,6,5,3,18,0,40,0);
		CPsd(10,6,5,5,18,0,40,0); 
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}

	else if(p == 1){	
		wa = -90;
		Ot(22,-90,0,30,40);	cw
		Avoid(0,12,16,0,40 + 10,40,0);
		Ot(5,-90,-10,30,0);	wa = 80;
		
		CPsd(12,6,5,3,18,0,40,0);
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}
	
	else if(p == 4 || p == 5){
		wa = (p == 4)? 90 : -90;
		Ot(22,(p == 4)? 90 : -90,0,30,40);	cw
		Avoid(0,9,13,0,35 + 45 + 10,40,0);
		Ot(5,-90,-10,30,0);	wa = 80;
		
		CPsd(12,6,5,3,18,0,40,0);
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}
	

	else if(p == 3){
		CPsd(12,8,5,3,32,0,50,60);
		CPsd(10,8,48,5,32,0,50,0); 
		Ot(25,90,10,45,0);	wa = -80;

		CPsd(12,3,5,3,14,0,40,0);
		CPsd(10,3,5,5,14,0,40,0); 
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}

	else if(p == 2){
		wa = 90;
		Ot(22,90,0,30,40);	cw
		Avoid(0,9,13,0,40 + 10,40,0);
		Ot(5,90,10,30,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	
	else if(p >= 6){
		wa = (p == 6)? 90 : -90;
		Ot(22,(p == 6)? 90 : -90,0,30,40);	cw
	
		Avoid(0,9,13,0,35 + 10 + 45,40,0);
		Ot(5,90,10,30,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	ar ab45(20);
}





