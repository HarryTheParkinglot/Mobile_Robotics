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
#define swap(a,b) tmp = a,a = b,b = tmp;
int aMap[16] = {0};
int block;
int bMap[8] = {0,};

int aMapGoal[16] = {0};
int bMapGoal[8] = {0};

int dfs_route[4][10],g_dfs_route[4][10],dfs_min[4],convert_min = 10,check[16] = {0};
int g_route[15],route[15],min,flg = 0;
int aLink[16][8],bLink[8][4];
int nowDir,nowPos,val = 0;
int rlc = -1;
int puckTotal = 0,goalTotal = 0,total = 0;
int wayList[4];
int minWayList[4];
int minOrder[3];
int colorPos[4] = {0};
int colorGPos[4] = {0};


void ALink(int p) {
   memset(aLink[p],-1,sizeof(int) * 8);

   if(p / 4 > 0) aLink[p][0] = p - 4;
   if(p % 4 > 0) aLink[p][2] = p - 1;
   if(p / 4 < 3) aLink[p][4] = p + 4;
   if(p % 4 < 3) aLink[p][6] = p + 1;

   if(aLink[p][0] >= 0 && aLink[p][2] >= 0) aLink[p][1] = p - 5;
   if(aLink[p][4] >= 0 && aLink[p][2] >= 0) aLink[p][3] = p + 3;
   if(aLink[p][4] >= 0 && aLink[p][6] >= 0) aLink[p][5] = p + 5;
   if(aLink[p][0] >= 0 && aLink[p][6] >= 0) aLink[p][7] = p - 3;
}

void BLink(int p) {
   memset(bLink[p],-1,sizeof(int) * 4);

   if(p / 4 > 0) bLink[p][0] = p - 4;
   if(p % 4 > 0) bLink[p][1] = p - 1;
   if(p / 4 < 1) bLink[p][2] = p + 4;
   if(p % 4 < 3) bLink[p][3] = p + 1;
}

int Sorted(int *node, int *goal, int len) {
   for(int i = 0 ; i < len ; i++) {
      if(bMapGoal[i] < 0) continue;
      if(node[i] != goal[i]) return 0;
   }
   return 1;
}

void displayA() {
	lcs

	for(int i = 0 ; i < 4 ; i++){
		lcd(i,0,"%d%d%d%d",aMap[i*4],aMap[i*4+1],aMap[i*4+2],aMap[i*4+3]);	
		lcd(i,9,"%d%d%d%d",aMapGoal[i*4],aMapGoal[i*4+1],aMapGoal[i*4+2],aMapGoal[i*4+3]);	
	}
}
void displayB() {
	lcs

	for(int i = 0 ; i < 2 ; i++){
		lcd(i,0,"%d%d%d%d",bMap[i*4],bMap[i*4+1],bMap[i*4+2],bMap[i*4+3]);	
		lcd(i,9,"%d%d%d%d",bMapGoal[i*4],bMapGoal[i*4+1],bMapGoal[i*4+2],bMapGoal[i*4+3]);	
	}
	
}

int GetWay(int p, int np) {
   if(abs(p - np) == 4) return 0; //Vertical
   if(abs(p - np) == 1) return 1; //Horizon
   return 2; //diagonal
}

int WayAlreadyUsed(int color, int type){
	if(!color) return 0;
	for(int i = 1 ; i < 4 ; i++){
		if(i == color || wayList[i] < 0) continue;
		if(wayList[i] == type) return 1;
	}
	return 0;
}

void DFS(int color, int pastWay, int p, int wp, int dp, int wayConvert, int *node) { //색깔 보유시
   	int minus = (color && p == wp) && (GetWay(dfs_route[color][dp-2],dfs_route[color][dp-1]) != GetWay(dfs_route[color][dp-1],dfs_route[color][dp]));
	
	if(wayConvert - minus > convert_min || (wayConvert - minus == convert_min && dp > dfs_min[color])) return;

   	dfs_route[color][dp] = p;
   	
   	if(p == wp && (!color || (pastWay == wayList[color]))){
		wayList[color] = pastWay;
    	dfs_min[color] = dp;
    	convert_min = wayConvert - minus;
    	memcpy(g_dfs_route[color],dfs_route[color],sizeof(dfs_route[color]));
      	return;
   }

   check[p] = 1;

   for(int i = 0 ; i < 8 ; i++) {
      int np = aLink[p][i];

      if(np < 0 || check[np] || (np != wp && node[np])) continue;
      int nowWay = GetWay(p,np);
      if(p / 4 == 2 && nowWay == 1) continue; //두번째 라인에서는 좌우이동 불가능

      int plus = color && ((nowWay != pastWay) || (np / 4 == 2 && bMap[np%4 - (np%4 > block%4)] != color)); //색깔이 다른 경우 convert필요
      DFS(color,nowWay,np,wp,dp+1,wayConvert + plus,node);
   }
   check[p] = 0;
}

int CheckDFS(int color, int pastWay, int p, int wp, int *node) {
   dfs_min[color] = 8;
   convert_min = 5;
   memset(check,0,sizeof(check));
   DFS(color,pastWay,p,wp,0,0,node);

   return dfs_min[color] != 8;
}

void Sort(int lp, int p, int dp, int *node) {
   if(dp >= min) return;

   route[dp] = p;

   if(Sorted(node,bMapGoal,8)) {
      flg = 1;
      min = dp;

      memcpy(g_route,route,sizeof(route));
      return;
   }

   for(int i = 0; i < 4 ; i++) {
      int np = bLink[p][i];

      if(np < 0 || np == lp) continue;
      swap(node[p],node[np]);
      Sort(p,np,dp+1,node);
      swap(node[p],node[np]);
   }
}



int GetColorGate(int color, int *node){
	for(int i =  0 ; i < 4;  i++)
		if(color == node[i])  return i%4 + (i%4 >= block % 4);
	return -1;
}

int GetColorWay(int color, int *node){
	for(int i = 4 ; i < 8 ; i++)
		if(color == node[i]) 
			return i%4;
		
	return -1;
}

void SetGateWay(int color, int gate, int way) {
   int null;
   memset(bMapGoal,-1,sizeof(bMapGoal));

   if(gate >= 0)	bMapGoal[gate] = color;
   bMapGoal[way + 4] = color;

   for(int i = 0 ; i < 8 ; i++)
      if(!bMap[i])   null = i;

   for(int i = 0 ; i < 20 ; i++) {
      min = i;
      flg = 0;
      
      Sort(-1,null,0,bMap);
      
      if(flg) {
         return;
      }
   }
}

void SetFinalGateWay() {
   memset(bMapGoal,-1,sizeof(bMapGoal));
	
   	for(int i = 1 ; i <= 3 ; i++)
		bMapGoal[wayList[i] + 4] = i;
   	
   	for(int i = 1 ; i <= 3 ; i++){
   		int p = g_dfs_route[i][dfs_min[i] - 1] ;
	   	
		if(p / 4 == 2){
			int gate = (p%4 > block)? p%4 - 1 : p%4;
	   		bMapGoal[gate] = i;
	   	}
	}

	int cnt = 0,val = 0,null[2] = {0};
   	
	for(int i = 0 ; i < 8 ; i++){
    	if(!bMap[i])   {
			null[cnt++]  = i;
			bMap[i] = val--;
		}
    }

   	for(int i = 0 ; i < 20 ; i++) {
		min = i;
      	flg = 0;
      	
      	for(int j = 0 ; j < 2 ; j++){  
      		Sort(-1,null[j],0,bMap);
    		if(flg) return;  		
		}
   	}
}
void MakeMinWayList(){
	int min = 10,min2 = 20;
	
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			if(i == j) continue;
			for(int k = 0 ; k < 3 ; k++){
				if(k == j || k == i) continue;
				wayList[1] = i;
				wayList[2] = j;
				wayList[3] = k;
				
				int order[3];
				for(order[0] = 1 ; order[0] <= 3 ; order[0]++){
					
					for(order[1] = 1 ; order[1] <= 3 ; order[1]++){
						if(order[1] == order[0]) break;
						
						for(order[2] = 1 ; order[2] <= 3 ; order[2]++){
							if(order[1] == order[2] || order[0] == order[2]) break;
							
							int node[16],bNode[8];
							memcpy(node,aMap,sizeof(node));
							memcpy(bNode,bMap,sizeof(bNode));
							
							int cnt = 0,cnt2 = 0;
							
							for(int t = 0 ; t < 3 ; t++){
								int color = order[t];
								if(!CheckDFS(color,(t == 0)? GetColorGate(color,bNode): -1,colorPos[color],colorGPos[color],node)) break; 
								
								cnt += convert_min;
								cnt2 += dfs_min[t];

								swap(node[colorPos[color]],node[g_dfs_route[color][dfs_min[color]-1]]);
								
								if(t == 2){
									if(cnt < min || (cnt == min && cnt2 < min2)) {
										memcpy(minWayList,wayList,sizeof(wayList));
										memcpy(minOrder,order,sizeof(order));
										min = cnt;
										min2 = cnt2;	
									}
								}
							}			
						}
					}
				}
			}	
		}
	}
}

int GetDirA(int p, int np){
	for(int i = 0 ; i < 8 ; i++)
		if(aLink[p][i] == np) return i;

	return -1;
}
int GetDirB(int p, int np){
	for(int i = 0 ; i < 4 ; i++)
		if(bLink[p][i] == np) return i;
	return -1;
}

int GetAngA(int wd){
	int rd = wd - nowDir;

	if(abs(rd) > 4) rd = (rd > 0)? rd - 8 : 8 + rd;
	if(abs(rd) == 4) rd = (g_psd[2] < 20)? -4 : 4; 
	
	return rd * 45;
}

int GetAngB(int wd){
	int rd = wd - nowDir;

	if(abs(rd) == 3) rd /= -3;
	if(abs(rd) == 2) rd = (g_psd[2] < 20)? -2 : 2; 
	return rd * 90;
}

int GetNextDirA(int p, int d){
	int prior[2][4] = {
		{0,2,6,4},
		{1,7,3,5}
	};

	for(int i = 0 ; i < 4; i++){
		int nd = (nowDir + prior[d%2][i])%8;

		if(aLink[p][nd] >= 0) return nd;
	}
}
int GetNextDirB(int p){
	int prior[4] = {0,1,3,2};

	for(int i = 0 ; i < 4; i++){
		int nd = (nowDir + prior[i])%4;

		if(bLink[p][nd] >= 0) return nd;
	}
}

void MoveA(int p, int wp){
	CheckDFS(0,0,p,wp,aMap);
	lcs lcd(1,1,"A / %d->%d / %d",p,wp);
	wait
	for(int i = 0 ; i < dfs_min[0] ; i++)
		MovePosA(0,g_dfs_route[0][i],g_dfs_route[0][i+1],(i < dfs_min[0] - 1)? g_dfs_route[0][i+2] : -1);
	
}

void MovePosA(int mode, int p, int np, int nnp){
	//mode 0 : getPuck
	//mode 1 : putPuck
	lcs lcd(1,1,"%d %d->%d / %d",mode,p,np,nnp);
	int wd = GetDirA(p,np);	
	int cm = wd % 2;

	if(nowDir != wd){
		Axis(0,0,GetAngA(wd),100,120);
		nowDir = wd;
		if(!cm) MLC();
	}
	
	if(mode){
		if(!cm){
			set
			MoveLine(10,0,26,30,140);
			Axis(0,-pos[1],0,30,140);
			MLC();
		}
		else{
			set
			Axis(0,70-22,0,40,150);
			Axis(0,-pos[1],0,40,150);
		}
		return;
	}
	
	int wwd = (nnp >= 0 && !cm)? GetDirA(np,nnp) : GetNextDirA(np,nowDir);

	if(wd != wwd){
		if(!cm){
			MoveLine(10,0,25,30,40);
			Ad(0,25,GetAngA(wwd),30,-40,80);
		}
		else{
			Axis(0,36,0,30,40);
			Ad(0,35,GetAngA(wwd),30,-40,80);
		}

		if(wwd%2 == 0)	MLC();
		
		nowDir = wwd;
	}
	else{
		if(aMapGoal[np]){
			MoveLine(10,0,50,30,140);
		}
		else{
			MoveLine(1,0,1000,30,40);
			MoveLine(10,0,14,30,-40);
		}
		MLC();
	}
	nowPos = np;
	
}

void MovePosB(int p, int np){
	int wd = GetDirB(p,np);	

	if(nowDir != wd){
		Axis(0,0,GetAngB(wd),100,120);
		nowDir = wd;
		MLC();
	}
	
	if(bMap[np] > 0){
		//SWAP
		MLC();
		
		MoveLine(10,0,20,30,40);

		int sideDir = (GetDirB(p,np) + 1) % 4;
		int chk = (bLink[np][sideDir] >= 0);
		Od2(0,25,(chk)? 180 : -180,30,-40,60,130);

		MLC();
		set

		MoveLine(10,0,23,30,140);
		Axis(0,-pos[1],0,30,140);

		MLC();
		nowDir = (nowDir + 2) % 4;
	}

	else{
		int wwd = GetNextDirB(np);
	
		if(wd != wwd){
			MoveLine(10,0,15,30,40);
			Od2(0,30,GetAngB(wwd),30,-40,100,120);
			MLC();

			nowDir = wwd;
		}
		else{
			MoveLine(1,0,1000,40,50);
			MoveLine(10,0,14,40,-50);
		}

	}
	swap(bMap[p],bMap[np]);
	nowPos = np;
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

case 0:
case 1:


ar ab45(20);
aMap[13] = Scan(49,89);
aMap[14] = Scan(215,87);

SS();
At(-40,60);	
aMap[15] = Scan(139,49); 


At(80,60);	wa = 40;
aMap[12] = Scan(101,50); 


Ad(-75,10,-40,50,160,40); 	cw
ar ab13(20);

wa = 90;
Ot(30,90,0,45,55);	cw
Avoid(0,12,16,0,60,50,0);
Avoid(0,12,16,0,50,50,0);

block = -1;

if(psdCheck == 1) block = 8;
else if(psdCheck == 2 && block < 0) block = 9;

MoveLineCheck(0,0,1000,0,50,0);
Ot(14,90,0,45,0);
Axis(-14,0,0,40,-50);
ar ab13(30);

MoveLineCheck(0,100,0,0,10,120);

aMapGoal[0] = Scan(124,136);
At(-90,100); SS();
ar ab45(30);

MLC();

aMapGoal[5] = Scan(124,136);
aMapGoal[6] = Scan(128,53);
aMapGoal[7] = Scan(131,25);

case 2:

Axis(-50,0,0,40,150);
ar ab245(30);
MoveLineCheck(0,100,0,0,10,120); SS();

aMapGoal[1] = Scan(124,136);
aMapGoal[2] = Scan(128,53);
aMapGoal[3] = Scan(131,25);

wa = 90;
Ot(16,90,0,40,50);	cw

CPsd(12,1,5,3,30,0,50,0);
CPsd(10,1,40,5,30,0,50,0);
CPsd(13,1,1000,5,30,0,50,0);

Ot(14 + 25,-90,70,45,0);	wa = -20;
Axis(0,-20,0,50,0);
Avoid(5,12,16,0,-60,50,0);

if(psdCheck == 1 && block < 0) block = 10;
else if(psdCheck == 2 && block < 0) block = 11;

Axis(0,-60,0,50,0);
Ot(-20,-90,-10,45,0);	wa = 60;

CPsd(12,4,-5,3,14,0,40,0);
CPsd(13,4,-1000,3,14,20,40,0);
CPsd(10,4,-8,5,14,0,40,0);
Ad(0,-10,30,40,-50,40);	cw

ar ab45(30);


SS();

for(int i = 0 ; i < 16 ; i++)
	goalTotal += aMapGoal[i];

aMapGoal[4] = 6 - goalTotal;
aMap[block] = 4;
displayA();
SS();
wait
   

//Mirror

GoB(1);                         
displayB();		
wait

for(int i = 0 ; i < 16 ; i++)  {
   	ALink(i);
   	if(aMapGoal[i])	colorGPos[aMapGoal[i]] = i;
   	if(aMap[i])	colorPos[aMap[i]] = i;
}

for(int i = 0 ; i < 8 ; i++){   
  	BLink(i);
  	if(!bMap[i]) bMap[i] = val--;
}


MakeMinWayList();

for(int t = 0 ; t < 3 ; t++){
	int color = minOrder[t];

	CheckDFS(color,(color == 1)? GetColorGate(1,bMap): -1,colorPos[color],colorGPos[color],aMap);
	
	for(int w = 0 ; w <= dfs_min[color] - 2; w++) {
		
		int lp = (w)? g_dfs_route[color][w-1] : -1;
		int p = g_dfs_route[color][w];
		int np = g_dfs_route[color][w+1];
		int nnp = (w != dfs_min[color] - 2)? g_dfs_route[color][w+2] : -1;
		
		int goalWay = GetWay(p,np);
		int goalGate = -1;
		if(lp / 4 == 2) goalGate = lp % 4 - (lp % 4 >= 	block%4);
		if(p / 4 == 2) goalGate = p % 4 - (p % 4 >= 	block%4);
		if(np / 4 == 2) goalGate = np % 4 - (np % 4 >= 	block%4);

        if((goalGate >= 0 && GetColorGate(color,bMap) != goalGate) || (goalWay != GetColorWay(color,bMap))){
           	SetGateWay(color,goalGate,goalWay);
           	
			if(min){
	           	if(!rlc) GetOutA(); //현 위치에서 나가기 
		   		if(rlc != 1)	GoB(0);
		   		GetInB(g_route[0]);
				
				for(int t = 0 ; t < min ; t++){
					MovePosB(g_route[t],g_route[t+1]);
					displayB();
				}
			}
        }
		
		if(rlc){
			if(rlc == 1)	GetOutB(nowPos);
			GetInA(g_dfs_route[color][0]);
		}
		
		if(nowPos != p)	MoveA(nowPos,p);

		MovePosA(nnp < 0 || (GetWay(np,nnp) != GetWay(p,np)),nowPos,np,nnp);
		swap(aMap[g_dfs_route[color][w]],aMap[g_dfs_route[color][w+1]]);	
		displayA();

		wait
	}
}

wait

SetFinalGateWay();
displayB();

if(min){
	if(rlc == 0) GetOutA();
	if(rlc == -1) GoB(0);
	GetInB(g_route[0]);
		
	for(int w = 0 ; w < min ; w++) {
	  MovePosB(g_route[w],g_route[w+1]);
	  displayB();
	}

	GetOutB(g_route[min]);   
}

	

for(int j = 0  ; j < 3 ; j++){
   	for(int i = 1 ; i < 4 ; i++){
		int m = dfs_min[i];
		int p = g_dfs_route[i][m-1];
		int np = g_dfs_route[i][m];
		if(aMap[np]) continue;
		
		if(rlc) GetInA(g_dfs_route[i][0]);
		
		if(nowPos != p)
			MoveA(nowPos,p);
		displayA();
        swap(aMap[p],aMap[np]);
		displayA();
        MovePosA(1,p,np,-1);
   	}
}

GetOutA();

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


int Scan(int x, int y){ //limit size 	
	SS();

	if(!T(101,3)) return 0; //색 감지 안되면 그냥 스킵 	

	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;

			if(abs(pro[i][1] - x) < 25 && abs(pro[i][2] - y) < 15) return pro[i][0];
		}
	}

	return 0;
}


void GetInA(int p){
	rlc = 0;
	nowPos = p;
	nowDir = 0;
	
	ar ab45(20);
	if(p % 4 == 0){
		Axis(-75,20,0,50,160);
		ar ab13(20);
		CPsd(12,1,5,3,16,0,40,50);
		CPsd(10,1,50,5,16,0,40,-50);
		MLC();
	}
	else if(p % 4 == 1){
		Axis(-15,0,0,30,40);
		MoveLineCheck(0,-12,0,0,30,-40);
		Axis(0,75,0,40,150);
		MLC();
	}
	else if(p % 4 == 2){
		Axis(15,0,0,30,40);
		MoveLineCheck(0,12,0,0,30,-40);
		Axis(0,75,0,40,150);
		MLC();
	}
	else if(p % 4 == 3){
		Axis(75,20,0,50,160);
		ar ab86(20);
		CPsd(12,8,5,3,20,0,40,50);
		CPsd(10,8,50,5,20,0,40,-50);
		MLC();
	}	
}



void GetOutA(){
	lcs
	lcd(1,1,"GETOUTA");
	wait
	displayA();
	wait
	lcs
	lcd(1,1,"np%d nd%d",nowPos,nowDir);

	int minDist = 10;
	int target = -1;
	
	for(int i = 12 ; i < 16 ;  i++){
		if(!aMap[i] && CheckDFS(0,0,nowPos,i,aMap) && (target < 0 || dfs_min[0] <= minDist)){
			target = i;
			minDist = dfs_min[0];
		}
	}
	
	MoveA(nowPos,target); SS();
	At(GetAngA(0),100);	cw SS();
	MLC();
	if(target == 12){
		At(-20,40);	wa = -20;
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
		Axis(20,-72,0,50,160);
		ar ab45(30);
	}

	else if(target == 14){

		Axis(-22,-72,0,50,160);
		ar ab45(30);
	}

	else{
		At(20,40);	wa = 20;
		CPsd(12,5,-5,3,14,0,50,60);
		CPsd(10,5,-35,5,14,0,50,0);	
		Ot(-30,-90,-10,45,0);	wa = -80;
	
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0);
		CPsd(10,3,17,3,14,0,40,0);
		Ad(0,10,-10,40,-50,30);	cw
		ar ab45(30);
	}
	rlc = -1;
}

void GoB(int scan){
	rlc = 1;
	nowPos = -1;
	ar ab45(20);
	MoveLineCheck(0,1000,0,0,50,60);	wa = 90;
	Ot(22,-90,15,45,0);	wa= 15;
	Avoid(4,12,14,0,-70,50,0);
	Avoid(4,12,14,20,-1000,50,0);
	Ad(0,-12,-15,50,-60,20);	cw
	ar ab45(20); 
	

	if(scan){
		bMap[3] = Scan(219,98);
	}


	Drift(0,26,50,25,35);	wa=  50;
	Ot(15,-90,40,25,0);	cw;	
	Axis(0,-15,0,30,-40);

	ar ab45(20);
	
	if(scan){
		At(-10,20);	
		
		bMap[0] = Scan(114,27);
		bMap[1] = Scan(128,50);
		bMap[2] = Scan(175,113);
		
		At(20,20);	

		bMap[4] = Scan(130,27);
		bMap[5] = Scan(113,51);
		bMap[6] = Scan(71,114);

		At(-10,20);	
		SS();

		Axis(0,2,0,10,120);
		ar ab45(20);

		for(int i = 0 ; i < 7 ; i++) 
			puckTotal += bMap[i];

		bMap[7] = 12 - puckTotal;

	}

	SS();
}


void GetInB(int p){
	rlc = 1;
	ar ab45(20);
	nowPos = p;

	int dir = (p / 4)? -1 : 1;

	if(p % 4 == 0){
		LineAvoid(0,10,14,0,80,40,50);
		Avoid(0,10,14,0,10,40,0);
		LineAvoid(0,10,14,0,80,40,0);
	
		if(p / 4 == 0){
			Avoid(0,10,14,0,20,40,0);
			Drift(1 * dir,40,-90 * dir,30,-40);
			MoveLineCheck(0,100,0,0,10,120);
			MoveLine(1,0,1000,30,40);
			MoveLine(10,0,14,30,-40);
		}

		else{
			Avoid(0,10,14,0,24,40,0);
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
		LineAvoid(0,10,14,0,80,40,50);
		Avoid(0,10,14,0,10,40,0);
		LineAvoid(0,10,14,0,80,40,0);

		Ot(13,-90 * dir,0,40,0);
		MoveLineCheck(0,8 * dir,0,0,20,-30);
		nowDir = 1;
		SS();
	}

	else if(p % 4 == 2){
		LineAvoid(0,10,14,0,100,40,50);
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

void GetOutB(int p){
	rlc = -1;
	

	if(p < 0){
		ar ab45(20);
		Drift(0,32,-90,30,140);
		LineAvoid(0,11,15,0,1000,40,50);
		Avoid(0,11,15,0,14,40,0);
		Avoid(0,12,16,0,35 + 10 + 22,40,0);
		Ot(5,90,10,35,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	else if(p == 0){
		At(GetAngB(0),100);	
		CPsd(12,1,5,3,23,0,50,60);
		CPsd(10,1,43,5,23,0,50,0); 
		Ot(25,-90,-10,45,0);	wa = 80;

		CPsd(12,6,5,3,18,0,40,0);
		CPsd(10,6,5,5,18,0,40,0); 
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}

	else if(p == 1){
		At(GetAngB(0),100);	wa = -90;
		Ot(22,-90,0,30,40);	cw
		Avoid(0,12,16,0,35 + 10,40,0);
		Ot(5,-90,-10,40,0);	wa = 80;
		
		CPsd(12,6,5,3,18,0,40,0);
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}
	
	else if(p == 4 || p == 5){
		
		At(GetAngB(0),100);	wa = (p == 4)? 90 : -90;
		Ot(22,(p == 4)? 90 : -90,0,30,40);	cw
		Avoid(0,12,16,0,35 + 45 + 10,40,0);
		Ot(5,-90,-10,40,0);	wa = 80;
		
		CPsd(12,6,5,3,18,0,40,0);
		CPsd(13,6,1000,5,18,0,40,0); 
		CPsd(10,6,10,5,18,0,40,0); 
		Ad(0,13,10,40,-50,20);	cw
	}
	

	else if(p == 3){
		At(GetAngB(0),100);	
		CPsd(12,8,5,3,32,0,50,60);
		CPsd(10,8,43,5,32,0,50,0); 
		Ot(25,90,10,45,0);	wa = -80;

		CPsd(12,3,5,3,14,0,40,0);
		CPsd(10,3,5,5,14,0,40,0); 
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}

	else if(p == 2){
		At(GetAngB(0),100);	wa = 90;
		Ot(22,90,0,30,40);	cw
		Avoid(0,12,16,0,35 + 10,40,0);
		Ot(5,90,10,40,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	
	else if(p >= 6){
		At(GetAngB(0),100);	wa = (p == 6)? 90 : -90;
		Ot(22,(p == 6)? 90 : -90,0,30,40);	cw
	
		Avoid(0,12,16,0,35 + 10 + 45,40,0);
		Ot(5,90,10,40,0);	wa = -80;
		
		CPsd(12,3,5,3,14,0,40,0);
		CPsd(13,3,1000,5,14,0,40,0); 
		CPsd(10,3,15,5,14,0,40,0); 
		Ad(0,13,-10,40,-50,20);	cw
	}
	ar ab45(20);
}


void MLC(){
	MoveLineCheck(0,2,0,0,10,120);
	MoveLineCheck(0,-4,0,0,10,120);
	MoveLineCheck(0,6,0,0,10,120);
	MoveLineCheck(0,-8,0,0,10,120);
	MoveLineCheck(0,10,0,0,10,120);
	MoveLineCheck(0,-12,0,0,10,120);
}


