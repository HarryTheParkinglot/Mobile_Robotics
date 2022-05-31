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

int dirList[6] = {25,90,155,205,270,335};

int node[10];
int map[10] = {
   0,0,3,
   3,2,
   0,1,0,
   1,2
}; 
int hole = -1,block = -1,line;

int mapGoal[10] = {
	2,3,1,
	2,3,
	3,1,2,
	1,2
};
int nowAng = 0,val = 0;
int dfs_route[10],g_dfs_route[10],dfs_min = 10,check[10];
int g_route[M],route[M],min,flg = 0,null[3],cnt = 0,storage[4];
int storage[4] = {0};


int link[10][6] = {
   {-1,-1,-1,3,1,-1},
   {-1,0,3,4,2,-1},
   {-1,1,4,-1,-1,-1},
   
   {0,-1,5,6,4,1},
   {1,3,6,7,-1,2},
   
   {-1,-1,-1,8,6,3},
   {3,5,8,9,7,4},
   {4,6,9,-1,-1,-1},
   
   {5,-1,-1,-1,9,6},
   {6,8,-1,-1,-1,7}
};

int history[M][10];

int Sorted(){
   for(int i = 0 ; i < 10 ; i++){  
      if(node[i] <= 0) continue;
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}

int InHistory(int *list,int dp){
   for(int i = 0 ; i < dp ; i++){
      for(int j = 0 ; j < 10 ; j++){
         if(node[j] != history[i][j]) break;
         if(j == 9) return 1;
      }
   }
   return 0;
}

void DFS(int p, int wp, int dp){
   if(dp >= dfs_min) return;
   
   dfs_route[dp] = p;
   
   if(p == wp){
      dfs_min = dp;
      memcpy(g_dfs_route,dfs_route,sizeof(dfs_route));
      return;
   }
   
   check[p] = 1;
   
   for(int i = 0 ; i < 6 ; i++){
      int np = link[p][i];
      if(np < 0 || check[np] || np == block) continue;
      DFS(np,wp,dp+1);
   }
   
   check[p] = 0;
}

int InList(int p, int *list, int len){
	for(int i = 0 ; i < len ; i++)
		if(p == list[i]) return 1;
	return 0;
}


int CheckDFS(int p, int wp){
   dfs_min = 10;
   memset(check,0,sizeof(check));
   DFS(p,wp,0);
   return dfs_min != 10;
}

void Sort(int lp, int p, int dp ,int *node){
   if(dp >= min || flg) return;
   
   if(InHistory(node,dp) && lp != -1) return;
   
   route[dp] = p;
   
   //display();
      
   if(Sorted()){
      min = dp;
      flg= 1;
      memcpy(g_route,route,sizeof(int) * (min + 1));
      return;
   }
   
   for(int i = 0 ; i < 6 && !flg; i++){
      int np = link[p][i];
      if(np < 0 || np == lp || np == hole || np == block) continue;
      
      swap(node[p],node[np]);
      Sort(p,np,dp+1,node);
      swap(node[p],node[np]);
   }
   /*
   for(int i = 0 ; i < cnt  && !flg; i++){
		int np = null[i];
		if(np == p || InList(np,link[p],6)) continue;
		route[dp+1] = -1;
		Sort(-1,np,dp+2,node);
   }
   */
}

int GetDir(int p, int np){
	for(int i = 0  ; i < 6;  i++)
		if(link[p][i] == np) return i;
}

int GetAng(int wantAng){
	int rd = wantAng - nowAng;

	if(abs(rd) > 180) {
		rd = (rd > 0)? rd - 360 : rd + 360;
	}

	nowAng = wantAng;

	return rd;
}
////////////////////////




void MLC(){
	if(MoveLineCheck(0,2,0,0,10,120)) return;
	if(MoveLineCheck(0,-4,0,0,10,120)) return;
	if(MoveLineCheck(0,6,0,0,10,120)) return;
	if(MoveLineCheck(0,-8,0,0,10,120)) return;
	if(MoveLineCheck(0,10,0,0,10,120)) return;
	if(MoveLineCheck(0,-12,0,0,10,120)) return;
	if(MoveLineCheck(0,14,0,0,10,120)) return;
}

void display(){
	lcs

	lcd(0,0,"%d %d %d",map[0],map[1],map[2]);
	lcd(1,0," %d %d",map[3],map[4]);
	lcd(2,0,"%d %d %d",map[5],map[6],map[7]);
	lcd(3,0," %d %d",map[8],map[9]);
	
}
void displayGoal(){
	lcs

	lcd(0,0,"%d %d %d",mapGoal[0],mapGoal[1],mapGoal[2]);
	lcd(1,0," %d %d",mapGoal[3],mapGoal[4]);
	lcd(2,0,"%d %d %d",mapGoal[5],mapGoal[6],mapGoal[7]);
	lcd(3,0," %d %d",mapGoal[8],mapGoal[9]);
	
}


void SwapMove(int p, int wp, int *node){
	MoveLine(10,0,10,30,40);
	MoveLine(4,0,1000,30,0);

	if(g_psd[2] > 30)
		Od2(0,25,180,30,-40,60,130);
	else
		Od2(0,25,180,30,-40,60,130);
	
	MLC();

	set
	MoveLine(10,0,10,30,40);
	MoveLine(4,0,1000,30,0);
	Axis(0,1,0,30,-40);
	Axis(0,-pos[1] - 5,0,30,140);	

	nowAng = (nowAng + 180) % 360;

}

void MovePtoP(int p, int np, int nnp, int *node){
	lcs lcd(1,1,"Move %d to %d",p,np);
	lcd(2,0,"nowAng %d",nowAng);
	
	int ndir = GetDir(p,np);	
	
	Axis(0,0,GetAng(dirList[ndir]),100,120);
	
	Axis(0,5,0,20,130);

	MLC(); SS();
	lcd(2,0,"nowAng %d",nowAng);
	
	if(node[np] > 0)
		SwapMove(p,np,node);

	else {
		int nndir = GetDir(np,nnp);		
		
		MoveLine(10,0,10,30,40);
		MoveLine(4,0,1000,30,0);
		
		if(nndir != ndir){
			Od2(0,27,GetAng(dirList[nndir]),30,-40,100,120);
		}
		else{
			Axis(0,27,0,30,-40);
		}
	}

	swap(node[p],node[np]);
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


Axis(0,-25,0,30,140); 
ar ab45(20);
Axis(0,25,0,30,140);

Ad(-25,0,40,30,140,80);	wa = 40;


//mapGoal[3] = ScanGoal(65,29); 
//mapGoal[4] = ScanGoal(160,47); 

Ad(15,0,-20,30,140,40);	wa = 20;


//ScanMap(8,145,54,22); 


Ad(10 + 10,0,-40,40,150,80);	wa = -20;


//ScanMap(9,102,58,22); 

Ad(15,0,-20,30,140,40);	wa = -40;

//mapGoal[8] = ScanGoal(119,55); 
//mapGoal[9] = ScanGoal(192,33); 

//wait

Ad(-25,0,40,30,140,60);	cw



case 2:


Axis(0,-25,0,30,140); 
ar ab45(20);

//wait 

Axis(0,55,0,50,60);
MoveLineCheck(0,0,1000,0,50,0);
Axis(0,18,0,50,-60);
//ScanMap(6,138,91,30); 

case 3:

Ad(0,30,80,40,150,80);	wa= -10;
//ScanMap(5,205,74,24); 

case -2:
wa= -10;
Axis(0,35,0,40,50);
Drift(-2,42,100,30,-40);	cw

ar ab86(20);
//mapGoal[0] = ScanGoal(57,23);
//mapGoal[1] = ScanGoal(112,22);
//mapGoal[2] = ScanGoal(163,30);

case 4:
At(90,100);

Axis(0,2,0,10,120);
ar wa = 90; AB(7,18,4,194,5,159,50,50); cw	

At(20,20);	wa = 20;
Axis(0,100,0,50,160);

//ScanMap(7,50,104,36);
case 5:
wa =20;
Drift(2,50,-110,40,150);	cw

ar ab13(30); SS();
//mapGoal[5] = ScanGoal(69,29);
//mapGoal[6] = ScanGoal(134,27);
//mapGoal[7] = ScanGoal(186,27);

case 6:
At(-90,100);

Axis(0,2,0,10,120);
ar ab45(30);
ar wa = 90; AB(2,20,4,194,5,159,50,50); cw	
//78

//

Ad(2,30,-90,30,140,100);	
//ScanMap(4,89,44,16);

Ad(-48,110,30,50,160,10);	wa = 30;
//ScanMap(0,53,52,16);


At(-30,40);	
//ScanMap(1,143,93,32);


At(-30,40);		wa= -30;
//ScanMap(2,206,53,20);


wa= -30;
Ad(-35,-70,30,40,150,10);	cw
//ScanMap(3,114,91,34);

Axis(-40,-80,0,50,160);
ar ab245(30);

display();
wait
displayGoal();
wait


SS();
wait


//break;

case -3:

for(int i = 0 ; i < 10 ; i++){   
  if(!map[i] && i != hole && i != block) {
     null[cnt++] = i;
     map[i] = val--;
  }
}

if(map[hole] && !Sorted()){
  memcpy(node,map,sizeof(node));
  
  int min_dist = 10;
  int min_pos = -1;
  int way[10];
  
  for(int i = 0 ; i < cnt ; i++){
     if(!CheckDFS(null[i],hole) || dfs_min >= min_dist) continue;
     min_pos = null[i];
     min_dist = dfs_min;
     memcpy(way,g_dfs_route,sizeof(way));
  }
  
  GetIn(way[0]);
  for(int i = 0 ; i < min_dist ; i++){
     display();
     MovePtoP(way[i],way[i+1],way[i+2],map);
  }
  memcpy(map,node,sizeof(node));
  
  cnt = 0;
  for(int i = 0 ; i < 10 ; i++)
     if(map[i] <= 0 && i != block && i != hole)
        null[cnt++] = i;   
}

for(int i = 0 ; i < M ; i++){
		for(int j = 0 ; j < cnt; j++){
		memcpy(node,map,sizeof(map));
		min = i;
		flg = 0;
		
		Sort(-1,null[j],0,node);
		if(flg) goto exit;
	}
}

exit:

if(!Sorted())
	GetIn(g_route[0]);


for(int i = 0 ; i < min ; i++){
  //display();
 
	MovePtoP(g_route[i],g_route[i+1],(i != min-1)? g_route[i+1] : -1,map);
	wait	
	
}

GetOut(g_route[min]);


case -4:

for(int i = 0 ; i < 10;  i++){
	if(map[i] > 0) continue;
	

	if(i == 0)	{
		GoA();
		ScanA();	wait}

	for(int j = 0 ; j < 4 ; j++){
		if(mapGoal[i] != storage[j]) continue;
		map[i] = storage[j];
		storage[j] = 0;

		GetPuck(j);
		PutPuck(i);
		break;
	}
}








case 8:
Fin();



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


int ScanGoal(int x, int y){ //limit size 	
	//해당 사이즈보다 큰 경우 시트지로 판정 
	//해당 사이즈보다 작은 경우 퍽으로 판정 



	if(!T(101,3)) return; //색 감지 안되면 그냥 스킵 	

	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;

			if(abs(pro[i][1] - x) < 25 && abs(pro[i][2] - y) < 25) return pro[i][0];
		}
	}
	return 0;
}

int ScanMap(int index, int x, int y, int size){ //limit size 	
	//해당 사이즈보다 큰 경우 시트지로 판정 
	//해당 사이즈보다 작은 경우 퍽으로 판정 
	wait
	
	if(!T(101,3)) return; //색 감지 안되면 그냥 스킵 	

	for(int t = 0 ; t < 3 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;

			if(pro[i][0] == 3){
				lcs lcd(1,1,"C%d X%d Y%d S%d",pro[i][0],pro[i][1],pro[i][2],pro[i][3]);			
				wait
				return;
			}

			if(abs(pro[i][1] - x) < 30 && abs(pro[i][2] - y) < 30){
				if(pro[i][3] < size + 8){
					map[index] = pro[i][0];
					
				}
				else if(pro[i][0] == 1) hole = index;
				else if(pro[i][0] == 2) block = index;
			}
		}
	}
	return 0;
}


void GetIn(int index){
	ar ab245(30);

	if(index == 5){
		CPsd(12,1,5,3,12,0,50,60);
		CPsd(10,1,65,5,12,0,50,0);
		Ot(10,-90,0,50,-60);
		nowAng = 0;
	}

	else if(index == 8){
		Axis(42,15,0,40,150);
		nowAng = 0;
	}


	else if(index == 3 || index == 4 || index == 6 || index == 7 || index == 9){
		//85
		Ad(2,45,-90,40,150,100);
		Axis(0,2,0,10,120);
		ar wa = 90; AB(7,18,4,194,5,159,50,50); cw		
		
		if(index == 6){
			Axis(0,80-38,0,50,60);
			Ot(38,90,0,40,-50);
			nowAng = 270;
		}
		else if(index == 7){
			Axis(0,80-38 + 70,0,50,60);
			Ot(38,90,90,40,-50);
			nowAng= 0;
		}
		else if(index == 9){
			Axis(0,80-38 + 35,0,50,60);
			Ot(35,-90,0,40,-50);
			nowAng = 270;
		}

		else if(index == 3){
			Ot(47,90,90,30,40);		wa = 0;
			Axis(0,70 + 35 - 42,0,50,-60);
			nowAng=  0;
		}
		else if(index == 4){
			Axis(0,71,0,40,50);
			Ot(42,90,70,30,0);		wa = -20;
			Axis(0,70 + 38 - 42,0,40,-50);
			nowAng =  340;
		}
	}

	else{
		At(-20,20);	wa = -20;
		Axis(115,215,0,60,70);
		Drift(20,40,110,50,-60);	cw
		Axis(0,-15,0,30,40);
		ab45(30);
		ar ab745(30);

		if(index == 2){
			Axis(-32,10,0,40,150);
			nowAng = 90;
		}
		else if(index == 1){
			CPsd(12,8,5,3,18,0,50,60);
			CPsd(10,8,10 - 5 - 35 + 70,3,18,0,50,0);
			Ot(32,90,0,45,-55);
			nowAng = 90;
		}
		else{
			CPsd(12,8,5,3,18,0,50,60);
			CPsd(10,8,10 - 5 - 38 + 140,3,18,0,50,0);
			Ot(35,90,90,45,-55);
			nowAng = 180;
		}

		//Drift();
		//Drift();
	}
	SS();

}

void GetOut(int index){
	if(index == 2){
		At(GetAng(-25),60);	wa = -25;
		Axis(-20,0,0,50,60);	wa = 65;
		Ot(-20,60,0,50,0);	cw

		Axis(0,-230,0,50,0);	wa = -25;
		Drift(-13,-8,25,40,-50);	cw

		ar ab245(30);
	}
	else if(index == 1){
		At(GetAng(-25),60);	wa = -25;
		Axis(0,-60,0,50,60);
		Ot(-20,-25,0,50,0);		cw

		Axis(0,-135,0,50,0);	wa = -25;
		Drift(-15,-25,25,40,-50);	cw
	}
	else if(index == 0){
		At(GetAng(20),60);	wa = 20;
		Axis(-5,0,0,30,40);
		CPsd(12,3,-5,3,18,0,50,60);
		CPsd(10,3,-80,5,18,0,50,0);
		Ot(-26,90,0,50,0);	wa = -70;
		Ot(-20,-90,-45,30,0);	wa = -25;
		Ot(-20,-25,0,40,0);	cw
		Axis(0,-60,0,50,0);	wa = -25;
		Ad(-20,-20,25,45,-55,60);	cw
	}
	else if(index == 3){
		At(GetAng(-25),60);	wa = -25;
		Axis(0,-60,0,50,60);
		Ot(-20,-25,0,50,0);		cw

		Axis(0,-60,0,50,0);	wa = -25;
		Drift(-14,-29,25,40,-50);	cw
	}
	else if(index == 4){
		At(GetAng(-25),60);	wa = -25;
		Axis(-20,0,0,50,60);	wa = 65;
		Ot(-20,60,0,50,0);	cw

		Axis(0,-140,0,50,0);	wa = -25;
		Drift(-13,-8,25,40,-50);	cw

	}
	else if(index == 5){
		At(GetAng(20),60);	wa = 20;
		Axis(-5,0,0,30,40);
		CPsd(12,3,-5,3,18,0,50,60);
		CPsd(10,3,-50,5,18,0,50,0);
		CPsd(-4,3,-1000,5,18,20,50,0);
		Ad(0,-12,-20,50,-60,40);	cw
	}

	else if(index == 6){
		At(GetAng(-25),60);	wa = -25;
		Axis(-20,0,0,50,60);	wa = 65;
		Ot(-20,60,0,50,0);	cw

		Axis(0,-65,0,50,0);	wa = -25;
		Drift(-13,-8,25,40,-50);	cw

	}
	else if(index == 7){
		At(GetAng(0),60);	
		Ot(-35,-90,-40,50,60);	wa = 50;
		Drift(0,-20,-30,50,0);		wa = 20;
		Avoid(4,12,16,0,-50,50,0);	
		Avoid(4,12,16,30,-1000,50,0);
		Ot(-18,90,50,40,0);	wa = -20;
		Drift(0,-20,20,50,-60);	cw
	}

	else if(index == 8){
		At(GetAng(0),60);	cw
		Axis(-40,-10,0,30,140);
	}

	else if(index == 9){
		At(GetAng(-70),60);	wa = -70;
		Ot(35,90,0,50,60);	wa = 20;
		
		Avoid(4,12,16,0,-50,50,0);	
		Avoid(4,12,16,30,-1000,50,0);
		Ot(-18,90,50,40,0);	wa = -20;
		Drift(0,-18,20,50,-60);	cw
	}
	SS();

	Axis(3,3,0,10,120);

	ar ab245(30);
}


void GoA(int dir){
	
	
	if(dir == 0){
		ar ab245(30);
		CPsd(12,1,5,3,14,0,50,60);
		CPsd(10,1,10,3,14,0,50,0);
		Ot(38,-90,-90,45,0);
		Ot(38,-90,-70,45,0);	wa = 20;
		Axis(0,34,0,50,0);
		Ot(40,-90,50,50,0);	wa = -20;
	
		CPsd(12,4,-5,3,18,0,50,0);
		CPsd(10,4,-10,5,18,0,50,0);
		CPsd(-5,4,-1000,5,18,20,50,0);
		Drift(0,-12,20,50,-60);

		ar ab245(30);
	}
	else{
		ar ab745(30);
		CPsd(12,8,5,3,18,0,50,60);
		CPsd(10,8,10,3,18,0,50,0);
		Ot(38,90,90,40,0);
		Ot(38,90,110,35,0);	wa = 20;
		Axis(0,37,0,40,0);
		Ot(40,-90,50,50,0);	wa = -20;
	
		CPsd(12,4,-5,3,18,0,50,0);
		CPsd(10,4,-10,5,18,0,50,0);
		CPsd(-5,4,-1000,5,18,20,50,0);
		Drift(0,-12,20,50,-60);

		ar ab245(30);
	}
}



void ScanA(){
	At(-90,100);
	Axis(-2,2,0,10,120);
	ar ab745(30);
	storage[3] = ScanGoal(124,136);

	Axis(-50,0,0,60,170);  SS();
	
	storage[2] = ScanGoal(124,136); SS();

	At(50,100);	wa = 50; SS();

	storage[0] = ScanGoal(56,40); SS();
	storage[1] = ScanGoal(138,83); SS();

	Ad(-25,0,-50,40,150,60);	cw
	
	lcs lcd(1,1,"%d %d %d %d",storage[0],storage[1],storage[2],storage[3]);SS();
	wait

	Drift(20,0,70,30,40);	wa = -20;
	Avoid(5,14,18,0,-30,40,0);
	Avoid(5,14,18,20,-1000,40,0);
	Drift(0,-10,20,40,-50);	cw

	ar ab245(30);


}


void GetPuck(int index){
	At(-90,100);
	
	Axis(-2,2,0,10,120);
	ar ab745(30);
	
	if(index == 0){
		
		At(10,10);	wa = 10;

		Axis(-120,0,0,50,60);	wa = -80;
		Avoid(2,14,18,20,1000,50,0);	wa = 10;
		Drift(-10,0,-10,50,-60);	cw
		ar ab45(30);
		wa = 90; AB(2,160,4,194,5,163,50,20); cw 

		CPsd(12,1,5,3,14,0,30,40);
		CPsd(10,1,15,3,14,0,30,0);
		Ad(0,30,-100,30,-40,100);	wa = 80;

		CPsd(12,6,5,3,18,0,40,50);
		CPsd(10,6,5,5,18,0,40,0);
		CPsd(-7,6,1000,5,18,20,40,0);
		Drift(0,12,10,40,-50);
		
		
		ar ab45(30);
		ar ab745(30);
		
		At(-20,20);	wa = -20;
		Axis(0,38,0,40,50);
		Ad(4,40,-70,40,-50,80);	cw
		
	}
	else if(index == 1){
		Axis(-70,0,0,50,60);
		MoveLineCheck(0,-1000,0,0,50,0);
		Axis(-5,0,0,40,-50);
		MoveLineCheck(0,10,0,0,10,120);
				
		MoveLine(10,0,20,30,40);
		Ad(0,30,-90,30,-40,80);
		Axis(0,0,-90,80,120);	
		ar ab45(20);
		MLC();

		Axis(-25,50,0,40,150);
	}
	else if(index == 2){
		Axis(-40,0,0,40,50);
		MoveLineCheck(0,-100,0,0,40,0);
		Axis(-5,0,0,40,-50);
		MoveLineCheck(0,10,0,0,10,120);

		MoveLine(10,0,20,30,40);
		Ad(0,30,90,30,-40,80);
		Axis(0,0,90,80,120);	
			
		ar ab45(20);
		MLC();
		
		Axis(25,50,0,40,150);
	}
	else{
		wa = 90; AB(7,120,4,194,5,163,50,20); cw

		CPsd(12,8,5,3,18,0,30,40);
		CPsd(10,8,15,3,18,0,30,0);
		Ad(0,30,100,30,-40,100);	wa = -80;

		CPsd(12,3,5,3,14,0,40,50);
		CPsd(10,3,5,5,14,0,40,0);
		CPsd(-2,3,1000,5,14,20,40,0);
		Drift(0,12,-10,40,-50);

		ar ab45(30);
		ar ab245(30);

		At(20,20);	wa = 20;
		Axis(0,38,0,40,50);
		Ad(-4,40,70,40,-50,100);	cw

	}
	SS();
}

void GoGetPoint(){
	cw
	Drift(-30,0,-70,30,40);	wa= 20;
	
	CPsd(12,3,-5,3,18,0,40,0);
	CPsd(10,3,-10,5,18,0,40,0);
	CPsd(-4,3,-1000,5,18,20,40,0);
	Ad(0,-10,-20,40,-50,40);	cw

	ar ab245(30); wait
}

void PutPuck(int index){
	if(index == 8){
		At(27,60);	cw
		Axis(0,55,0,40,150);
		Axis(0,-55,0,40,150);	wa = 27;
		At(-27,60);	cw
	}
	else if(index == 9){
		At(-27,60);	cw
		Axis(0,55,0,40,150);
		Axis(0,-55,0,40,150);	wa = -27;
		At(27,60);	cw
	}
	else if(index == 6){
		set

		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,62,0,40,-50);
			
		int y = pos[1];

		Axis(0,-25,0,30,140);
		At(20,40);	wa = 20;
		Axis(0,-y + 27,0,40,150);
		At(-20,40);	cw
	}
	else if(index == 5){
		MoveLineCheck(0,0,1000,0,40,50);
		Ad(0,10,40,40,-50,60);	wa = 40;
		
		Axis(0,25,0,40,50);
		Ot(10,90,0,30,0);	wa = -50;
		Axis(0,48,0,40,0);
		Ad(0,20,-40,30,-40,60);	cw
		
		ar ab13(30);
		ar AB(0,0,1,92,0,0,30,30);	
		CPsd(13,1,30,3,21,0,20,30);
		Axis(0,3,0,20,-30);
		
		Axis(-5,-50,0,50,160);
		
		ar ab45(20);
		ar ab245(20);
		SS();

		GoA(0);
	}
	else if(index == 7){
		MoveLineCheck(0,0,1000,0,40,50);
		Ad(0,10,-40,40,-50,60);	wa = -40;
		
		Axis(0,25,0,40,50);
		Ot(10,-90,0,30,0);	wa = 50;
		Axis(0,48,0,40,0);
		Ad(0,20,40,30,-40,60);	cw
		
		ar ab86(30);
		ar AB(0,0,8,66,0,0,30,30);
		CPsd(14,8,30,3,30,0,20,30);
		Axis(0,3,0,20,-30);
		
		
		Axis(5,-50,0,50,160);
		
		ar ab45(20);
		ar ab745(20);
		SS();

		GoA(1);
	}
	else if(index == 3){
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,20,0,40,0);
		Ad(0,25,40,40,-50,40);	wa = 40; 
		Ad(-37,0,-40,30,140,40);	cw
		
		int y = pos[1];		

		set
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,60,0,40,-50);	
		

		Axis(0,-pos[1],0,40,150);
		Ad(37,0,40,30,140,40);	wa  = 40;
		
		Axis(0,-20,0,40,50);
		Ad(0,-y + 20,-40,40,-50,40);	cw

		GoGetPoint();
			
	}

	else if(index == 4){
		
		set
		MoveLineCheck(0,0,1000,0,40,50);

		Ad(0,25,-40,40,-50,40);	wa = -40; 
		Ad(37,0,40,30,140,40);	cw

		int y = pos[1];		

		set
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,60,0,40,-50);	
		

		Axis(0,-pos[1],0,40,150);
		Ad(-37,0,gyro-40,30,140,40);	wa  = -40;
		
		Axis(0,-20,0,40,50);
		Ad(0,-y + 20,40,40,-50,40);	cw

		GoGetPoint();
	}
	else if(index == 0){
		set
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,20,0,40,0);
		Ad(0,25,40,40,-50,40);	wa = 40; 
		Ad(-35,0,-40,30,140,40);	cw
		int fy = pos[1];

		MoveLineCheck(0,0,1000,0,40,50);
		
		Axis(0,20,0,40,0);
		Ad(0,25,40,40,-50,40);	wa =40;

		
		Ad(-35,0,-40,30,140,40);	cw
		set

		CPsd(12,1,10,3,21,0,40,50);
		CPsd(10,1,10,5,21,0,40,0);
		CPsd(13,1,1000,3,21,0,40,0);
		Axis(0,3,0,40,-50);
		int y = pos[1];		

		Axis(0,-20,0,30,140);
		At(20,40);	wa = 20;
		
		CPsd(12,3,-5,3,42,0,40,50);
		CPsd(10,3,-y + 25,5,42,0,40,-50);
		
		Axis(15,0,0,30,40);
		Ad(20,0,20,30,-40,20);	wa = 40;

		Axis(0,-35,0,40,50);
		Ad(0,-35,-40,40,-50,40);	cw

		Ad(35,0,40,30,140,40);	wa = 40;
		
		Drift(0,-40,-40,40,50);	cw
		Axis(0,-fy + 40,0,45,-55);

		wait
		
		GoGetPoint();		
	}
	else if(index == 2){
		set
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,20,0,40,0);
		Ad(0,25,-40,40,-50,40);	wa = -40; 
		Ad(35,0,40,30,140,40);	cw
		int fy = pos[1];

		MoveLineCheck(0,0,1000,0,40,50);
		
		Axis(0,20,0,40,0);
		Ad(0,25,-40,40,-50,40);	wa =-40;

		
		Ad(35,0,40,30,140,40);	cw
		set

		CPsd(12,8,10,3,30,0,40,50);
		CPsd(10,8,10,5,30,0,40,0);
		CPsd(13,8,1000,3,30,0,40,0);
		Axis(0,3,0,40,-50);
		int y = pos[1];		

		Axis(0,-20,0,30,140);
		At(-20,40);	wa = -20;
		
		CPsd(12,6,-5,3,21,0,40,50);
		CPsd(10,6,-y + 25,5,21,0,40,-50);
		
		Axis(-15,0,0,30,40);
		Ad(-20,0,-20,30,-40,20);	wa = -40;

		Axis(0,-35,0,40,50);
		Ad(0,-35,40,40,-50,40);	cw

		Ad(-35,0,-40,30,140,40);	wa = -40;
		
		Drift(0,-40,40,40,50);	cw
		Axis(0,-fy + 40,0,45,-55);

		wait
		
		GoGetPoint();	
	}
	else if(index == 1){
		set
		MoveLineCheck(0,0,1000,0,40,50);
		Axis(0,20,0,40,0);
		Ad(0,25,-40,40,-50,40);	wa = -40; 
		Ad(35,0,40,30,140,40);	cw
		int fy = pos[1];

		MoveLineCheck(0,0,1000,0,40,50);
		
		Axis(0,20,0,40,0);
		Ad(0,25,-40,40,-50,60);	wa =-40;

		Ad(35,0,40,30,140,40);	cw
		CPsd(12,8,5,3,30,0,40,50);
		CPsd(10,8,30,5,30,0,40,0);
		Ad(0,30,30,40,-50,60);	wa = 30;
				
		Axis(-35,0,0,40,50);
		MoveLineCheck(2,-1000,0,0,40,0);
		Axis(-2,0,0,40,-50);
		MLC();	cw
		set

		MoveLine(4,0,1000,40,50);
		Axis(0,3,0,40,-50);
		Axis(0,-20,0,30,140);
		wa = 30;

		Axis(28,0,0,40,50);	
		Ad(30,0,60,40,-50,60);	cw

		ar ab45(30);

		Drift(-35,2,-70,30,40);	wa = 20;
		CPsd(12,5,-5,3,21,0,40,0);
		CPsd(10,5,-30,5,21,0,40,-50);

		Ad(-35,0,-60,30,140,60);	wa = -40;

		Axis(0,-35,0,40,50);
		Ad(0,-35,40,40,-50,40);	cw

		Ad(-35,0,-40,30,140,40);	wa = -40;
		
		Drift(0,-40,40,40,50);	cw
		Axis(0,-fy + 40,0,45,-55);


	}

}

void Fin(){	
	ar ab245(30);
	Axis(25,77,0,50,160); SS();
	LED_ON(3);
	_delay_ms(2000);
	LED_OFF(3);
}
