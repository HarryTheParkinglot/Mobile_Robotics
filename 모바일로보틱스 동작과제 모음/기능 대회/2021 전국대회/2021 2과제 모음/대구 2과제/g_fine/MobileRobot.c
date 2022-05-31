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
#define ab13 AB(0,0,1,162,3,134,30,0);

#define ab865(y) AB(5,195,6,173,8,125,30,y); cw
#define ab134(y) AB(4,188,1,165,3,158,30,y); cw
int tmp;
#define swap(a,b) tmp = a,a= b,b = tmp;

#define M 40

int nowPos,nowDir,fc= 0;

int g_route[20],route[20],min,check[20] = {0},link[20][4],subLink[8][4];
int map[20] = {0},mapGoal[20] = {0},visit[20] = {0};

int detect_puck = 0,detect_seet = 0,detect_block = 0;
int seet_map[12][2],seet_check[12] = {0};
int points[6],seet_points[4],first_color;

///////////////
int sub_map[8] = {0},sub_mapGoal[8] = {0},node[8],colors[3];
int sort_route[20],g_sort_route[20],sort_min = 20,flg = 0,gate,subGate;

int Sorted(int *arr, int *goalArr, int size){
	for(int i = 0 ; i < size ; i++){
		if(arr[i] <= 0) continue;
		if(arr[i] != goalArr[i]) return 0;
	}
	return 1;
}

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 < 4) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}


void SubLink(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4 > 0) str[0] = p-4;
	if(p % 4 > 0) str[1] = p-1;
	if(p / 4 < 1) str[2] = p+4;
	if(p % 4 < 3) str[3] = p+1;
}

void DFS(int p, int wp, int dp, int *node, int link[][4]){ //길찾기 DFS 하나  
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(int) * (min + 1));
		return;
	} 
	
	visit[p] = 1;
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || visit[np] || (np != wp && node[np])) continue;
		
		DFS(np,wp,dp+1,node,link);
	}
	
	visit[p] = 0;
} 

int CheckDFS(int p, int np, int *node, int link[][4]){
	memset(visit,0,sizeof(visit));
	min = 12;
	DFS(p,np,0,node,link);
	return min != 12;
}

int GetPuckCnt(int p, int d){
	int count = 0;
	
	memset(points,-1,sizeof(points));
	points[0] = p;
	
	
	for(int i = 1 ; i <= 3 ; i++){
		points[i] = link[points[i-1]][d];
		
		if(points[i] < 0) break; //퍽이 있으면 어차피 다음꺼 스캔 힘듦  
		if(!map[points[i]] || check[points[i]]) continue;
		count ++;
	}
	return count;
} 

int GetSeetCnt(int p, int d){
	int plus[4] = {-3,-1,3,1},count = 0;
	
	int std = (p/4-1)*3 + p%4;
	
	int points[4] = {std,std-1,std+2,std+3};
	int view_points[2] = {points[(d+1)%4] + plus[d],points[d] + plus[d]};
	if(link[p][(d+1)%4] < 0) view_points[0] = -1;
	if(link[p][(d+3)%4] < 0) view_points[1] = -1;
	if(link[p][d] < 0 || link[link[p][d]][d] < 0) memset(view_points,-1,sizeof(view_points));
	
	for(int i = 0 ; i < 2 ; i ++){
		seet_points[i] = view_points[i];
		if(seet_points[i] < 0 || seet_check[seet_points[i]]) seet_points[i] = -1;		
		else count ++;
	}
	//printf("%d %d\n",view_points[0],view_points[1]);
	return count;
}

int GetSeetTotal(int p){
	int tot = 0;
	for(int i = 0 ; i < 4 ; i++)
		tot += GetSeetCnt(p,i);
	return tot;
}

int GetPuckTotal(int p){
	int tot = 0;
	for(int i = 0 ; i < 4 ; i++)
		tot += GetPuckCnt(p,i);
	
	return tot;
}

int NextPos(int p, int len){
	if(len < 0) return -1;
	
	
	for(int i = 0 ; i < 4; i++){
		int np = link[p][i];
		if(np < 0 || map[np]) continue; //퍽이 있거나 방문한경우 
		
		if(!check[np] && ((detect_seet < 3 && GetSeetTotal(np)) || (detect_puck < 3 && GetPuckTotal(np))))	return np;
		int val = NextPos(np,len-1); 
		if(val >= 0) return val;
	}
	return -1;
}


void ApplySeetScan(int p) {
	check[p]  = 1;

	for(int i = 0 ; i < 2 ; i++){
		if(seet_points[i] < 0 || seet_check[seet_points[i]]) continue;
		
		Axis(0,0,(i)? -20 : 20,40,150);
		ScanSeet(seet_points[i]);
		Axis(0,0,(i)? 20 : -20,40,150);
		MLC();

		seet_check[seet_points[i]] = 1;
		if(seet_map[seet_points[i]][0]) detect_seet ++;
		
	}
}

void ApplyPuckScan(int p){
	check[p]  = 1;
	
	for(int i = 1 ; i <= 3 ; i++){
		if(!check[points[i]]){ //바로 앞이거나 퍽인 경우 
			int value = ScanPuck(i - 1);
			if(i != 1 && !detect_block && !value) continue;
			map[points[i]] = value;

			if(i == 1 && !map[points[i]] && !detect_block) {
				if(g_psd[0] < 40)  	map[points[i]] = 4;
				else {
					Axis(-2,0,0,10,120);	if(g_psd[0] < 40)  	map[points[i]] = 4;
					Axis(4,0,0,10,120);		if(g_psd[0] < 40)  	map[points[i]] = 4;
					Axis(-2,0,0,10,120);
				}
			}

			if(map[points[i]] == 4) detect_block = 1;
			if(map[points[i]]){
				check[points[i]] = 1;
				detect_puck ++;	
							
				if(detect_puck == 3)
					for(int j = 0 ; j < 20 ; j++)
						if(map[j] < 0)  map[j] = 0;
				
			}
		}
	}
}


void SetMapGoal(int color, int index, int type){
	//type 1(10cm) : 가로 (horizon) 
	//type 2(15cm) : 세로 (vertical) 
	//type 3(20cm) : 대각선 (diagonal) 
	
	if(type == 1){
		int value = (mapGoal[index] || mapGoal[index+1]) * 2;	
		mapGoal[index + value] = mapGoal[index + value + 1] = color;
	}
	else if(type == 2){
		int value = (mapGoal[index] || mapGoal[index+4] || index+4 == gate);	
		mapGoal[index + value] = mapGoal[index + value + 4] = color;
	}
	else{
		if(index + 5 != gate && !mapGoal[index] && !mapGoal[index + 5]) mapGoal[index] = mapGoal[index + 5] = color;
		else mapGoal[index + 1] = mapGoal[index + 4] = color;
	}
}

int SubSorted(int fc, int gate, int *node){
	if(node[gate] > 0 || node[gate+4] > 0)  return 0;
	

	for(int i = 0 ; i < 4; i++){
		if(node[i] <= 0) continue;
		if(node[i] != node[i+4]) return 0;
 	}
 	if(node[0] == fc || node[3] == fc) return 0;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= sort_min) return;
	//printf("dp : %d\n",dp);
	//dis();
	
	sort_route[dp] = p;
	
	if(SubSorted(fc,subGate,node)){
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		flg = 1;
	}
	
	for(int i = 0 ; i < 4 && !flg ; i++){
		int np = subLink[p][i];
		if(np < 0 || np == lp) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}


int GetDir(int p, int np, int link[][4]){
	for(int i = 0  ; i < 6;  i++)
		if(link[p][i] == np) return i;
}

int GetAng(int d, int wd){
	int rd = wd -d;
	
	if(abs(rd) == 3) rd/=-3;
	if(abs(rd) == 2) rd = (g_psd[2] > 20)? 2 : -2;

	return rd * 90;
}
////////////////////////

void display(int *node){
	lcs
	for(int i = 0 ; i < 4;  i++){
		lcd(i,0,"%d %d %d %d %d",node[3],node[3 - i + 4],node[3 - i + 8],node[3 - i + 12],node[3 - i + 16]);
	}
	wait
}
void displaySeet(int node[][2]){
	lcs
	for(int i = 0 ; i < 3;  i++){
		lcd(i,0,"%d[%d] %d[%d] %d[%d]",node[i*3][0],node[i*3][1],node[i*3+1][0],node[i*3+1][1],node[i*3+2][0],node[i*3+2][1]);
	}
	wait
}

void MLC(){
	if(MoveLineCheck(0,2,0,0,10,120)) return;
	if(MoveLineCheck(0,-4,0,0,10,120)) return;
	if(MoveLineCheck(0,6,0,0,10,120)) return;
	if(MoveLineCheck(0,-8,0,0,10,120)) return;
	if(MoveLineCheck(0,10,0,0,10,120)) return;
	if(MoveLineCheck(0,-12,0,0,10,120)) return;
	if(MoveLineCheck(0,14,0,0,10,120)) return;
}


int GetNextDir(int p, int link[][4]){
	int prior[4] = {0,1,3,2};
	
	
	for(int i = 0 ; i < 4 ; i++){
		int wd = (nowDir + prior[i]) % 4;
		if(link[p][wd] >= 0) return wd;
	}
}

void MovePos(int mode, int p, int wp, int *node, int link[][4]){
	CheckDFS(p,wp,node,link);

	for(int i = 0 ; i < min ; i++)
		MovePtoP(mode && i == min - 1,g_route[i],g_route[i+1],map,link);
	
	if(mode)
		swap(node[p],node[wp]);
	
}

void MovePtoP(int mode, int p, int np, int *node, int link[][4]){
	lcs lcd(1,1,"%d | %d -> %d",mode,p,np);
	//wait
	
	int wd = GetDir(p,np,link);	
	if(nowDir != wd)
		Axis(0,0,GetAng(nowDir,wd),60,130);
	nowDir = wd;

	MLC();
	
	
	if(mode % 2 == 0){			
		int wwd = GetNextDir(np,link);
	
		MoveLine(10,0,(wd % 2 || mode == 2)? 20 : 15,40,50);
	
		if(nowDir != wwd){
			Od2(0,30,GetAng(nowDir,wwd),30,-40,100,120);
		}
		else	{
			MoveLine(1,0,1000,40,0);	
			MoveLine(10,0,14,40,-50);	
		}
		MLC();

		nowDir = wwd;
		nowPos = np;
	}
	else{
		set
		MoveLine(10,0,(wd % 2 || mode == 3)? 50 - 23 : 45-23,40,150);
		Axis(0,-pos[1],0,40,150);
		MLC();
	}
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



ar ab745(30); SS();

for(int i = 0 ; i < 20 ; i++)	Link(i,link[i]);
for(int i = 0 ; i < 12 ; i++)	SubLink(i,subLink[i]);

memset(map,-1,sizeof(map));

map[16] = map[17] = map[18] = map[19] = 0;

GetInMap();

gate = 19 + ((int)pos[0]/50);

MoveLineCheck(0,10,0,0,10,120);
MoveLine(1,0,1000,40,50);
MoveLine(10,0,14,40,-50);

nowPos  = gate;

lcs lcd(1,1,"nowPos %d",nowPos);	

flg =  0;

while(!flg){
	int sd = nowDir;

	for(int j = 0 ; j < 4 ; j++){
		int wd = (sd+j)%4;
	
		int cnt = (detect_puck < 3)? GetPuckCnt(nowPos,wd) : 0;
		int cnt2 = (detect_seet < 3)? GetSeetCnt(nowPos,wd) : 0;

		if(cnt + cnt2 == 0)	continue;
			
		if(nowDir != wd){
			Axis(0,0,GetAng(nowDir,wd),130,120);	
			MLC();	
			SS();
		}
		nowDir = wd;
		
		if(cnt) ApplyPuckScan(nowPos);
		if(cnt2) {
			lcs
			lcd(0,1,"SEETSCAN");
			ApplySeetScan(nowPos);
			
		}
	}
	display(map);
	displaySeet(seet_map);

	if(detect_seet == 3){
		if(detect_puck == 3) flg = 1;
		
		for(int j = 0 ; j < 20 ; j++){
			if(map[j] < 0) break;
			if(j == 19) flg = 1;
		}		
	}

	for(int j = 0 ; j < 10 && !flg; j++){
		if(NextPos(nowPos,j) >= 0){
			MovePos(0,nowPos,NextPos(nowPos,j),map,link);	
			break;}
		
		flg = j == 9;
	}
}


for(int i = 8 ; i >= 0 ; i--){ //mapGoal 세팅 
	//printf("%d\n",i + i/3);
	if(!seet_map[i][0]) continue;
	
	SetMapGoal(seet_map[i][0],(i/3)*4 + i%3,seet_map[i][1]);
}

SS();
lcs
lcd(1,1,"NOWPOS %d",nowPos);	


for(int t = 0 ; t < 2 ; t ++){
	for(int i =  0 ; i < 20 ; i++){	//퍽 넣을 위치 
		if(map[i] || !mapGoal[i] || mapGoal[i] == map[i]) continue;

		for(int j = 0 ; j < 20 ; j++){
			if(!map[j] || map[j] == mapGoal[j] || mapGoal[i] != map[j] || !CheckDFS(nowPos,j,map,link) || !CheckDFS(i,j,map,link)) continue;
		
			MovePos(0,nowPos,j,map,link);
			MovePos(1,j,i,map,link);		
			break;
		}
	}
}


 



for(int i = 0 ; i < 20 ; i++){ //first Color(가장 먼저 사용하는 색) 세팅 
	if(map[i] || !mapGoal[i]) continue;
	fc = mapGoal[i];
	break;
}




GetOut();
GoHome(gate % 4);	

case -2:
GoSubScan();



if(key == -2) {
	fc = 2;
	for(int i = 0 ; i < 12 ; i++)	SubLink(i,subLink[i]);
}
lcs lcd(1,1,"SUBGATE : %d",subGate);	wait

SS();

for(int i = 0, val = 0; i < 8 ; i++)
	if(!sub_map[i]) sub_map[i] = val--;

for(int i = 0 ; i < 30 ; i++){ //정렬 알고리즘 
	flg = 0;
	sort_min = i;
	memcpy(node,sub_map,sizeof(node));
	Sort(-1,subGate,0,node);
	if(flg) {LED_ON(3); _delay_ms(1000); LED_OFF(3); break;}
}

nowPos = subGate;

for(int i = 0 ; sort_min &&  i < sort_min; i++){
	MovePos(0,g_sort_route[i],g_sort_route[i+1],sub_map,subLink);
	
	if(sub_map[g_sort_route[i+1]])
		MovePos(1,g_sort_route[i+1],g_sort_route[i],sub_map,subLink);
}


for(int t = 0 ,cnt = 0; t < 2 ; t ++){
	for(int i =  0 ; i < 20 ; i++){	//퍽 넣을 위치 
		if(map[i] || !mapGoal[i] || mapGoal[i] == map[i]) continue;

		for(int j = 0 ; j < 8 ; j++){
			if(!map[j] || mapGoal[i] != sub_map[j] || !CheckDFS(nowPos,j,map,subLink) || !CheckDFS(gate,i,map,link)) continue;
		
			if(cnt) GoSubMap();
			
			MovePos(0,subGate,j,sub_map,subLink);
			MovePos(0,j,subGate,sub_map,subLink);
			GoMap();

			nowPos = gate;
			MovePos(1,gate,i,map,link);		
			swap(map[i],sub_map[j]);
			break;
		}
	}
}

GetOut();
GoHome();

LED_ON(3);
_delay_ms(1000);
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
		lcd(2,0,"C%d X%d Y%d S%d S%d",key_puck,Cmd(key_puck,102),Cmd(key_puck,103),Cmd(key_puck,104),Cmd(key_puck,105));		
		lcd(3,0,"%d%d%d%d%d|%d%d G%04d",IR_4,IR_1,IR_2,IR_3,IR_5,IND_1,IND_2, (int)gyro);
		lcd(3,16,"k:%02d",key);
	}
}


int ScanPuck(int dist){

	if(!T(101,3)) return;
	
	int posData[3][2] = {
		{130,145},
		{127,56},
		{126,25}
	};

	for(int t = 0 ; t < 5 ; t++){
		SV1
		V1();
		V3
		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;
		
			if(abs(pro[i][1] - posData[dist][0]) > 20 || abs(pro[i][2] - posData[dist][1]) > 20) continue;
			return pro[i][0];
		}
	}
	return 0;
}

void ScanSeet(int index){
	if(!T(101,3)) return;
	int x = 132;
	int y = 85;
	
	int sizeData[3] = {40,64,80};

	for(int t = 0 ; t < 5 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;
		
			if(abs(pro[i][1] - x) > 30 || abs(pro[i][2] - y) > 30) continue;
			lcs 
			lcd(1,1,"C%d X%d Y%d S%d",pro[i][0],pro[i][1],pro[i][2],pro[i][3]);
			lcd(2,1,"IDX %d",index);	//wait

			for(int j = 0 ; j < 3 ; j++){
				if(abs(pro[i][3] - sizeData[j]) > 10) continue;
				
				seet_map[index][0] = pro[i][0]; 
				seet_map[index][1] = j + 1;
				return;
			}
		}
	}
}

void GetOut(){
	MovePos(0,nowPos,gate,map,link);
	
	if(nowDir != 2)
		Axis(0,0,GetAng(nowDir,2),100,120); 
	nowDir = 2;
	MLC();
	
}


void GoSubScan(){
	ar ab345(30);
	nowDir = 2;
	Drift(-3,40,90,40,150);
	code=0;
	co[0]=0;
	Ot(7,-90,0,40,50);
	Axis(10,0,0,50,0);

	if(co[0]==1) {
		subGate = 2;
	   Axis(5,0,0,50,-60);
	   MoveLineCheck(0,-1000,2,0,20,130);
   
	   MoveLine(1,0,1000,50,60);
	   Ad(0,15,90,50,-60,100);
	   MoveLineCheck(0,-1000,0,0,20,130);

	   sub_map[3] = T(103,3);
	   Axis(0,0,-45,130,120);
	   sub_map[7] = T(103,3);
	   Axis(0,0,-45,130,120);
	   sub_map[6] = T(103,3);
	   Axis(0,0,-60,130,120);

	   SS();
	   SV1
	   V1();
	   V3

	   for(int i=0; i<9; i++) {
	      if(!pro[i][0]) continue;
	      if(abs(pro[i][1]-67)<20 && abs(pro[i][2]-86)<20) sub_map[5] = pro[i][0];
	      if(abs(pro[i][1]-146)<20 && abs(pro[i][2]-37)<20) sub_map[4] = pro[i][0];
	   }

	   Axis(0,0,-30,80,130);

	   SS();
	   SV1
	   V1();
	   V3

	   for(int i=0; i<9; i++) {
	      if(!pro[i][0]) continue;
	      if(abs(pro[i][1]-130)<20 && abs(pro[i][2]-130)<20) sub_map[1] = pro[i][0];
	      if(abs(pro[i][1]-125)<20 && abs(pro[i][2]-45)<20) sub_map[0] = pro[i][0];
	   }

	   lcs lcd(0,0, "%d %d %d %d", sub_map[0],sub_map[1],sub_map[2],sub_map[3]);
	   lcd(1,0, "%d %d %d %d", sub_map[4],sub_map[5],sub_map[6],sub_map[7]);
		wait
	   nowDir = 1;
	}
	else {
		subGate = 1;
	   MoveLineCheck(0,1000,100,0,50,0);
	   Axis(3,0,0,50,-60);
	   MoveLineCheck(0,-1000,2,0,20,130);

	   MoveLine(1,0,1000,50,60);
	   Ad(0,15,-90,50,-60,100);
	   MoveLineCheck(0,1000,0,0,20,130);

	   sub_map[0] = T(103,3);
	   Axis(0,0,45,130,120);
	   sub_map[4] = T(103,3);
	   Axis(0,0,45,130,120);
	   sub_map[5] = T(103,3);
	   Axis(0,0,60,130,120);

	   SS();
	   SV1
	   V1();
	   V3

	   for(int i=0; i<9; i++) {
	      if(!pro[i][0]) continue;
	      if(abs(pro[i][1]-200)<20 && abs(pro[i][2]-86)<20) sub_map[6] = pro[i][0];
	      if(abs(pro[i][1]-118)<20 && abs(pro[i][2]-36)<20) sub_map[7] = pro[i][0];
	   }

	   Axis(0,0,30,80,130);

	   SS();
	   SV1
	   V1();
	   V3

	   for(int i=0; i<9; i++) {
	      if(!pro[i][0]) continue;
	      if(abs(pro[i][1]-130)<20 && abs(pro[i][2]-130)<20) sub_map[2] = pro[i][0];
	      if(abs(pro[i][1]-125)<20 && abs(pro[i][2]-45)<20) sub_map[3] = pro[i][0];
	   }

	   lcs lcd(0,0, "%d %d %d %d", sub_map[0],sub_map[1],sub_map[2],sub_map[3]);
	   lcd(1,0, "%d %d %d %d", sub_map[4],sub_map[5],sub_map[6],sub_map[7]);	wait
	   nowDir = 3;
	}
	
}

void GoHome() {
	if(nowDir != 2)
		Axis(0,0,GetAng(nowDir,2),100,120);
	MLC();

	nowDir = 2;
	MLC();

   if(gate%4==0) {
      MoveLine(0,0,1000,50,60);
      Ot(35,90,-70,45,0); wa=20;
      Avoid(4,11,15,0,-90,50,0);
      Avoid(4,11,15,25,-1000,50,0);
      Drift(0,-13,-20,50,-60); cw
   }
   else if(gate%4==1) {
      MoveLine(0,0,1000,50,60);
      Ot(35,90,-70,45,0); wa=20;
      Avoid(4,11,15,0,-40,50,0);
      Avoid(4,11,15,25,-1000,50,0);
      Drift(0,-13,-20,50,-60); cw
   }
   else if(gate%4==2) {
      MoveLine(0,0,1000,50,60);
      Ot(35,90,-70,45,0); wa=20;
      Avoid(4,11,15,25,-1000,50,0);
      Drift(0,-13,-20,50,-60); cw
   }
   else {
      MoveLine(0,0,1000,50,60);
      Drift(3,40,-90,50,-60);
   }

   ar ab345(30)
}

void GetInMap(){	
	CPsd(12,8,5,3,18,0,40,50);
	CPsd(10,8,25,5,18,0,40,-50);

	set
	MoveLineCheck(0,-1000,0,0,50,60);
	Axis(-5,0,0,50,-60);
}


void GoMap() {
	if(nowDir != 0)
		Axis(0,0,GetAng(nowDir,0),100,120);
	nowDir = 0;
	MLC();
	
	
	nowPos = gate;

   if(subGate == 1) { //왼쪽에서 나오는 경우
      if(gate%4 <= 1) { //왼쪽 두군데로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,90,0,45,0);
         MoveLineCheck(0,-1000,0,0,50,0);
         Axis(-3,0,0,50,-60);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==2) { //오른쪽에서 두번째로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(-3,50,0,50,-60);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else { //가장 오른쪽으로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,-90,0,45,0);
         MoveLineCheck(0,1000,0,0,50,0);
         Axis(3,0,0,50,-60);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
   }
   else { //오른쪽에서 나오는 경우
      if(gate%4 >= 2) { //오른쪽 두군데로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,-90,0,45,0);
         MoveLineCheck(0,1000,0,0,50,0);
         Axis(3,0,0,50,-60);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==1) { //왼쪽에서 두번째로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(3,50,0,50,-60);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else { //가장 왼쪽으로 들어가는 경우
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,90,0,45,0);
         MoveLineCheck(0,-1000,0,0,50,0);
         Axis(-3,0,0,50,-60);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
   }
}

void GoSubMap() {
	nowPos = subGate;
   if(subGate == 1) { //왼쪽으로 들어가는 경우
      if(gate%4==0) {
         MoveLine(0,0,1000,50,60);
         Ot(15,90,0,45,0);
         Axis(-40,0,0,50,0); wa=-90;
         Ot(20,-90,0,45,0); cw
         Ot(17,90,0,45,0);
         MoveLineCheck(0,-1000,0,0,50,0);
         Axis(-3,0,0,50,-60);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==1) {
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,90,0,45,0);
         MoveLineCheck(0,-1000,0,0,50,0);
         Axis(-3,0,0,50,-60);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==2) {
         MoveLine(0,0,1000,50,60);
         Axis(0,50,0,50,-60);
         MLC();
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else {
         MoveLine(0,0,1000,50,60);
         Ot(15,-90,0,45,0);
         Axis(15,0,0,50,0); wa=90;
         Ot(20,90,0,45,0); cw
         Ot(15,-90,0,45,-55);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
   }
   else { //오른쪽으로 들어가는 경우
      if(gate%4==0) {
         MoveLine(0,0,1000,50,60);
         Ot(15,90,0,45,0);
         Axis(-10,0,0,50,0); wa=-90;
         Ot(20,-90,0,45,0); cw
         Ot(15,90,0,45,-55);
         MoveLineCheck(0,1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==1) {
         MoveLine(0,0,1000,50,60);
         Axis(0,50,0,50,-60);
         MLC();
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else if(gate%4==2) {
         MoveLine(0,0,1000,50,60);
         Axis(0,30,0,50,0);
         Ot(20,-90,0,45,0);
         MoveLineCheck(0,1000,0,0,50,0);
         Axis(3,0,0,50,-60);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
      else {
         MoveLine(0,0,1000,50,60);
         Ot(15,-90,0,45,0);
         Axis(40,0,0,50,0); wa=90;
         Ot(20,90,0,45,0); cw
         Ot(17,-90,0,45,0);
         MoveLineCheck(0,1000,0,0,50,0);
         Axis(3,0,0,50,-60);
         MoveLineCheck(0,-1000,0,0,20,130);
         MoveLine(1,0,1000,50,60);
         MoveLine(10,0,14,50,-60);
      }
   }
}

