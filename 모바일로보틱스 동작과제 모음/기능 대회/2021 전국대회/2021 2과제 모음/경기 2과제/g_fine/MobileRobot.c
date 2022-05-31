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
#define M 11

int node[32];
int map[32] = {
   0
};

int mapGoal[32] = {0};
int seet[3] = {0};


int dfs_route[20],g_dfs_route[20],dfs_min = 20,check[32];
int g_route[M][2],route[M][2],len,flg = 0;

int link[32][4];

unsigned char nowDir,nowPos,moveContinue;

void Link(int p){
   memset(link[p],-1,sizeof(int) * 4);
   
   if(p / 4 > 0) link[p][0] = p - 4;
   if(p % 4 > 0) link[p][1] = p - 1;
   if(p / 4 < 7) link[p][2] = p + 4;
   if(p % 4 < 3) link[p][3] = p + 1;
}


int Sorted(){
   for(int i = 0 ; i < 32 ; i++){  
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}


int CanPass(int color, int p,int np){
   if(color <= 0) return 1; //퍽을 먹지 않은 상태면 상관 없음 
   if(abs(np - p) != 4) return 1;
   
   if((np / 4 <= 3 && p / 4 <= 3 )|| (np / 4 >= 4 && p / 4 >= 4)) return 1;
   for(int i = 0; i < 3 ; i++)
      if(seet[i] == color && (p%4 == i || p%4 == i + 1)) return 1;
   
   return 0;
}


void DFS(int color, int p, int wp, int dp){ //색깔 보유시  
   if(dp >= dfs_min) return;
   
   dfs_route[dp] = p;
   
   if(p == wp){
      dfs_min = dp;
      memcpy(g_dfs_route,dfs_route,sizeof(dfs_route));
      return;
   }
   
   check[p] = 1;
   
   for(int i = 0 ; i < 4 ; i++){
      int np = link[p][i];
      if(np < 0 || check[np] || !CanPass(color,p,np) || (np != wp && node[np])) continue;
      DFS(color,np,wp,dp+1);
   }
   
   check[p] = 0;
}

int CheckDFS(int color, int p, int wp){
   dfs_min = 15;
   memset(check,0,sizeof(check));
   DFS(color,p,wp,0);
   return dfs_min != 15;
}

void MirrorSort(int p, int dp, int *node){
   if(flg) return;
   
   //printf("p : %d dp : %d\n\n",p,dp);
   //display(node);
   
   
   if(Sorted()) {
      len = dp;
      memcpy(g_route,route,sizeof(g_route));
      flg = 1; 
      return;
   }
   
   //first Check
   
   for(int i = 31 ; i >= 16 && !flg; i--){
       if(!mapGoal[i] || node[i]) continue;
      if(!CheckDFS(0,p,i)) return; //정렬이 되지 않았는데 막혔으므로 버려야함  
   }
   
   for(int i = 31 ; i >= 16 && !flg; i--){
      if(!mapGoal[i] || node[i]) continue;
      if(!CheckDFS(0,p,i)) return; //정렬이 되지 않았는데 막혔으므로 버려야함  
      
      for(int j = 0 ; j < 16 && !flg; j++){
         if(!node[j] || node[j] != mapGoal[i] || !CheckDFS(mapGoal[i],i,j)) continue;
         swap(node[i],node[j]);
         
         route[dp][0] = j; 
         route[dp][1] = i;
         
         MirrorSort(g_dfs_route[dfs_min-1],dp+1,node);
         swap(node[i],node[j]);
      }
   }
}



int GetDir(int p, int np){
	for(int i = 0  ; i < 4;  i++)
		if(link[p][i] == np) return i;
}

int GetAng(int wd){
	int rd = wd - nowDir;

	if(abs(rd) == 3) rd/=-3;
	if(abs(rd) == 2) rd = (g_psd[2] < 25 && g_psd[7] > 25)? -2 : 2; 
	return rd * 90;
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
	for(int i = 0; i < 4 ; i++){
		lcd(i,0,"%d %d %d %d",map[i*4],map[i*4+1],map[i*4+2],map[i*4+3]);
	}

	for(int i = 0; i < 4 ; i++){
		lcd(i,8,"%d %d %d %d",mapGoal[16 + i*4],mapGoal[16 + i*4+1],mapGoal[16 + i*4+2],mapGoal[16 + i*4+3]);
	}
}


void GoPos(int type, int color, int from, int to){
	int mc = 0; //moveConnection
	//type 0 : get
	//type 1 : put
	
	CheckDFS(color,from,to);

	for(int i = 0 ; i < dfs_min ; i++){
		int p = g_dfs_route[i];
		int np = g_dfs_route[i+1];
		int nnp = g_dfs_route[i+2];
		
		int nd = GetDir(p,np);
		int nnd = GetDir(np,nnp);
		
		if(i == dfs_min - 1){
			if(type == 1){
				MoveLine(10,0,28 - (5*(p/4 + np/4 == 7)),30,(mc)? -40 : 140);
				Axis(0,-28,0,30,140);
				MLC();

				return;
			}
			
			else{
				int prior[4] = {0,1,3,2};
				for(int j = 0 ; j < 4 ; j++){
					nnd = (nd + prior[j])%4;

	 				if(link[np][nnd] < 0) continue;
					break;
				}
			}
		}
		
		if(nowDir != nd){
			Axis(0,0,GetAng(nd),60,130);
			nowDir = nd;
			mc = 0;
			MLC();
		}
		
		if(nd != nnd){
			nowDir = nd;
			int rAng = GetAng(nnd);
			MoveLine(10,0,20,40,50 * !mc);	
			Od2(0,25 - (5 * ((p/4 + np/4 == 7))),rAng,40,-50,100,120);
			wa = rAng;
			MoveLineCheck(0,0,1000,0,10,120);	cw
			mc = 0;
			nowDir = nnd;
		}
		else{
			MoveLine(1,0,1000,40,50 * !mc);
			MoveLine(10,0,14,40,-50 * (i == dfs_min-1));
			mc = (i != dfs_min-1);
		}
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

case 0:
case 1:

	
	SS();
	seet[1] = Scan(123,58);   

	Axis(-60,50,0,50,160);

	MoveLineCheck(0,-1000,0,0,20,30);
	Axis(-5,0,0,20,0);

	
	ab13(20);	
	case 2:
	ar ab13(20);

	MoveLineCheck(0,1000,0,0,30,40);
	Axis(25,0,0,30,-40);	SS();

	seet[0] = Scan(129,150);
	wait
	map[8] = Scan(70,40);
	map[9] = Scan(171,38);//map[9] = Scan(139,41);
	
	map[12] = Scan(37,89);
	map[13] = Scan(203,84);
	
	lcs	lcd(1,1,"%d %d %d %d",map[8],map[9],map[12],map[13]);	
	lcd(2,1,"Seet : %d %d",seet[0],seet[1]);
	//wait

	Avoid(0,14,18,0,30,50,60);
	MoveLineCheck(0,0,1000,0,50,0);
	Avoid(0,14,18,0,35,50,-60);
	
	wait
	map[0] = Scan(74,38);
	map[1] = Scan(173,37);
	map[4] = Scan(43,82);
	map[5] = Scan(206,81);
	lcs	lcd(1,1,"%d %d %d %d",map[0],map[1],map[4],map[5]);	
	
	Avoid(0,14,18,0,40,40,50);
	MoveLineCheck(0,0,1000,0,40,0);
	Ad(2,40,-90,40,-50,100);	SS();

	Axis(0,-22,0,20,130);

	case 3:
	ar ab45(30);
	ar ab45(30);

	Avoid(0,14,18,0,30,40,150);
	
	map[3] = Scan(74,38);
	map[7] = Scan(173,37);
	map[2] = Scan(43,82);
	map[6] = Scan(206,81);
	lcs	lcd(1,1,"%d %d %d %d",map[3],map[7],map[2],map[6]);	
	
	//wait

	Avoid(0,14,18,0,50,40,50);	
	MoveLineCheck(0,0,1000,0,40,0);
	Ad(2,40,-90,40,-50,100);	SS();

	Axis(0,-27,0,20,130);

	case 4:

	
	ar ab45(30);
	ar ab45(30);
	Avoid(0,12,16,0,35,40,150);	

	map[15] = Scan(74,38);
	map[14] = Scan(173,37);
	map[11] = Scan(43,82);
	map[10] = Scan(206,81);

	lcs	lcd(1,1,"%d %d %d %d",map[15],map[14],map[11],map[10]);	

	//wait
	SS();
	cw

	Avoid(0,14,18,0,100,50,60);
	MoveLineCheck(0,0,1000,0,50,0);
	Avoid(0,14,18,0,27,50,0);
	Ot(15,90,0,45,0);	cw
	MovePsd(0,2,-1000,0,0,15,40,0);
	Axis(-3,0,0,40,-50);

	ar ab13(30);

	MoveLineCheck(0,100,0,0,10,120);
	MoveLine(1,0,1000,20,30);
	MoveLine(10,0,14,20,-30);
	SS();

	seet[2] = 6 - seet[0] - seet[1];

	nowDir = 2;
	nowPos = 19;

	for(int i = 0 ; i < 32 ; i++)   Link(i);
   
	for(int i = 0 ; i < 16 ; i++){
		int p = 28 - (i / 4)*4 + (i % 4);
      	mapGoal[28 - (i/4)*4 + (i % 4)] = map[i]; 
   	}

	
	LED_ON(3);
	_delay_ms(300);
	LED_OFF(3);
	lcs
	display();
	wait


   	memcpy(node,map,sizeof(map));
	
   	MirrorSort(nowPos,0,node);
   	
	LED_ON(3);
	_delay_ms(500);
	LED_OFF(3);
	
	lcs
	lcd(1,1,"FLG : %d",flg);

	//wait
   	memcpy(node,map,sizeof(map));
   	display();
	wait
   
   
   	for(int i = 0 ; i < len ; i++){
		lcs lcd(1,1,"%d -> %d -> %d",nowPos,g_route[i][0],g_route[i][1]); wait
		GoPos(0,0,nowPos,g_route[i][0]);
      	GoPos(1,node[g_route[i][0]],g_route[i][0],g_route[i][1]);
	  	
      	node[g_route[i][1]] = node[g_route[i][0]];
      	node[g_route[i][0]] = 0;

      	nowPos = g_dfs_route[dfs_min-1];
   	}

	SS();

	GoPos(0,0,nowPos,9);
	Axis(0,0,GetAng(2),100,120);

	Axis(-25,-25,0,40,150);
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

			if(abs(pro[i][1] - x) < 20 && abs(pro[i][2] - y) < 15) return pro[i][0];
		}
	}

	return 0;
}

