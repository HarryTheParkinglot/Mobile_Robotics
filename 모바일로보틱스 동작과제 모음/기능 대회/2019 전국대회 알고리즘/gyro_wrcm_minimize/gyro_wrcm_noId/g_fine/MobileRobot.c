/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

int tmp = 0;
#define swap(a,b) tmp = node[a], node[a] = node[b], node[b] = tmp;

#define cw wa = 0;
#define ar acf =cacf= 0;
#define set pos[0] = pos[1] = pos[2] = 0; gyro = rearGyro = 0;
#define V3 while(!Cmd(0,19));
#define SV1 while(!Cmd(0,18));
#define reset bar_1 = bar_2 = bar_3 = bar_4 = bar_5 = sbar_1 = sbar_2= 0;
#define wait if(key) {SS(); while(SW2); while(!SW2);}
#define lcs lcd_clear_screen();
int key= 0, key_puck = 0;

int map[28] = {0};
int gmap[28] = {0};
int node[28] = {0};

int link[28][4] = {0};
int visit[28] = {0},sc_visit[28] = {0},check[28] = {0};
int route[28],g_route[28],min,flg = 0,lastPos = 0,lastDir = 0;


int target[2] = {0,8};
int targetPos[2][4] = {0};
int answer[2][4] = {
	{1,2,3,0},
	{3,1,2,0}
};

void Link(int pos, int *str){
    memset(str,-1,sizeof(int)*4);
   
	if(pos / 7 > 0) str[0] = pos - 7; 
	if(pos % 7 > 0) str[1] = pos - 1;
	if(pos / 7 < 3) str[2] = pos + 7;
	if(pos % 7 < 6) str[3] = pos + 1;
}

void DFS(int from, int to, int dp){
    if(dp >= min) return;
   
    route[dp] = from;
   
    if(from == to){
        memcpy(g_route,route,sizeof(g_route));
       	min = dp;
    }
   
   	else{
        visit[from] = 1;
        for(int i = 0 ; i < 4 ; i++){
	        int np = link[from][i];
	            
	        if(np < 0 || visit[np] || (np != to && node[np])) continue;
	        DFS(np,to,dp+1);
       	}
      
    	visit[from] = 0;
   	}
   	
	return;
}

int GetDir(int point, int wpoint){
    for(int i = 0 ; i < 4;  i++)
        if(link[point][i] == wpoint)    return i;
}

int GetAng(int dir, int wdir){
    int rdir = wdir - dir;
    if(abs(rdir) == 3) rdir/=-3;
	if((rdir == 2 && g_psd[2] < 25) || (rdir == -2 && g_psd[7] < 25)) rdir = -rdir;

    return rdir * 90;
}

int SearchAll(int p){
	check[p] = 1;
   
    for(int i = 0 ; i < 4 ; i++){
        int np = link[p][i];
      
        if(np < 0 || check[np] || node[np])    continue;
        if(!sc_visit[np] || !SearchAll(np))   return 0;
    }
    return 1;
}



int Search(int lp, int p, int d){ 
    lcd(1,8,"LP%d P%d",lp,p);
	lcd(2,8,"d%d",d);

	sc_visit[p] = 1;
	       
    for(int i = 0 ; i < 4 && !flg ; i++){
      	int np = link[p][i]; //현재 바라보고있는 곳 부터 사방 검사 
       	if(np < 0 || sc_visit[np]) continue;//이미 방문했거나 인덱스 바깥인경우 
    	
        if(i != d)	Axis(0,0,GetAng(d,i),130,120);  //각도 회전 
        d = i;
       	
		if(T(101,3))	node[np] = Scan(147,155);  //노드 스캔  
		
		
		if(node[np]){ 
			lcs
			sc_visit[np] = 1;
			for(int i = 0 ; i < 4 ; i++){
				for(int j = 0 ; j < 7 ; j++){
					lcd(i,j,"%d",node[i * 7 + j]);
				}
			}

        }
      	else{
         	if(link[np][d] < 0){
				for(int j = 0 ; j < 8 ; j++){
					int nnp = link[np][j%4];
					if(nnp < 0 || (j / 4 == 0 && sc_visit[nnp])) continue; 
					
					MLC();
					MoveLine(10,0,15,40,50);
					Ad(0,35,GetAng(d,j % 4),40,-50,100);
			
					d = j%4;
					break;
				}
			} 	
			else{
				MLC();
				MoveLine(1,0,1000,50,60);
				MoveLine(10,0,13,50,-60);
			}

			d = Search(p,np,d); 
			//lcd(1,8,"LP%d P%d",lp,p);
			//lcd(2,8,"d%d",d);
		}
   	}
   
   memset(check,0,sizeof(int) * 28);

   	if(flg || SearchAll(p)){
      	if(!flg){

         	flg = 1;
         	lastPos = p;
         	lastDir = d;
      	}      
   }
   
   else{
        int wd = GetDir(p,lp);

        if(wd != d){	
			Axis(0,0,GetAng(d,wd),130,120);  //각도 회전 
        	d = wd;
		}

      	if(link[lp][d] < 0){
			for(int j = 0 ; j < 8 ; j++){
				int nnp = link[lp][j % 4];
				if(nnp < 0 || (j / 4 == 0 && sc_visit[nnp])) continue;
				
				
				MLC();
				MoveLine(10,0,15,40,50);
				Od2(0,35,GetAng(d,j % 4),40,-50,130,120);
				
				d = j % 4; 
				break;
			}	
		}
		else{
			MLC();
			MoveLine(1,0,1000,50,60);
			MoveLine(10,0,13,50,-60);
		}
  	}

   	return d;
}


int Fin(){
	for(int i = 0 ; i < 28 ; i++){
		if(!gmap[i]) continue;
		if(gmap[i] != node[i]) return 0;
	}
	return 1;
}

void SetTarget(int area,int pixel, int *str){
   int value = pixel / 3 * 7 + pixel % 3;
   
   for(int i = 0 ; i < 4 ; i++)
      str[i] = value + i/2*7 + i%2 + (area * 3);
      
//   printf("%d %d\n%d %d\n\n",str[0],str[1],str[2],str[3]);
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

for(int i = 0 ; i < 28 ; i++)
	Link(i,link[i]);



MoveLineCheck(0,-100,0,0,20,130);
MoveLine(1,0,1000,40,50);
MoveLine(10,0,13,40,-50);
SS();
lcs
Search(-1,7,3);

LED_ON(3);
_delay_ms(100);
LED_OFF(3);

lcs lcd("LP%d  LD%d",lastPos,lastDir);
wait


for(int i = 0  ; i < 2 ; i++){
	SetTarget(i,target[i],targetPos[i]);
         
	for(int j = 0 ; j < 4 ; j++)      
		gmap[targetPos[i][j]] = answer[i][j];
}

flg = 0;

while(!flg){
	for(int i = 0 ; i < 8 && !flg; i++){	
		int pos = targetPos[i/4][i%4];
		if(node[pos] == gmap[pos] || node[pos] < 0) continue;
		min = 12;	DFS(lastPos,i,0);
		if(min == 12) continue;
		
		if(!node[pos]){ //퍽 필요 
			for(int j = 0 ; j < 28 ; j++){
				if(node[j] < 0 || gmap[j] == node[j] || gmap[pos] != node[j]) continue;
				min = 12;	DFS(j,pos,0); //퍽을 가져다 둘 수 있는가 
				if(min == 12) continue;
				int point = g_route[min - 1];
				
				min = 12;	DFS(lastPos,j,0); //퍽을 먹으러 갈 수 있는가 
				if(min == 12) continue; 
				//printf("GET %d -> %d -> %d\n",lastPos,j,pos);
				GetPuck(lastPos,j);
				PutPuck(lastPos,pos);
				
				node[pos] = node[j];
				node[j] = 0;
				
				lastPos = point;
				//display(lastPos,0);
				break;
			}
		}
		
		else{
			for(int t = 0 ; t < 2 ; t++){
				for(int j = 0 ; j < 28 ; j++){
					if(node[j]) continue;
					if(t == 0 && (node[pos] != gmap[j])) continue;
					
					min = 12; DFS(pos,j,0); 
					if(min == 12) continue;
					
					GetPuck(lastPos,pos);	
					PutPuck(lastPos,j);
					//printf("CLEAN %d -> %d -> %d\n",lastPos,pos,j);
					
					node[j] = node[pos];
					node[pos] = 0;
					//printf("LP : %d\n\n",lastPos);
					//display(lastPos,0);
					t = 2;
					break;
				}
			}
		}
		
		flg = Fin();
	}
}

SS();

LED_ON(3);
_delay_ms(100);
LED_OFF(3);
lcs
for(int k = 0 ; k < 28 ; k++){
	lcd(k/7,k%7,"%d",node[k]);
}

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



int Scan(int x, int y){
	SS();

	for(int t = 0 ; t < 2 ; t++){
		SV1
		V1();
		V3

		for(int i = 0 ; i < 9 ; i++){
			if(!pro[i][0]) continue;

			if(abs(pro[i][1] - x) < 25 && abs(pro[i][2] - y) < 30){
				if(pro[i][3] < 40) {return -1;} //장애물 
				else {return pro[i][0];}
			}
		}
	}
	
	return 0;
}

void MLC(){
	for(int i = 1 ; i < 10 ; i++){
		if(MoveLineCheck(0,((i % 2)? 2 : -2) * i,0,0,10,120))	break;

	}
}



void GetPuck(int from, int to){
	MLC();
	int stop = 1;
	
	min = 12;
	DFS(from,to,0);
	
	
	for(int i = 0 ; i < min ; i++){
		int wd = GetDir(g_route[i],g_route[i+1]);
		int wwd = (i + 2 <= min)? GetDir(g_route[i + 1],g_route[i + 2]) : -1;
		

		if(wwd >= 0){
			//아직 가야할 경로 남아있음 
			if(lastDir != wd)	Axis(0,0,GetAng(lastDir,wd),60,130); 
			lastDir = wd;
			
			if(wd== wwd){
				MoveLine(1,0,1000,40,50 * stop);
				stop = (i >= min - 2);
				
				MoveLine(10,0,12,40,-50 * stop);
				
			}
			else{				
				MoveLine(10,0,10,40,50 * stop);
				Od2(0,40,GetAng(lastDir,wwd),40,-50,100,120);
				MLC();
				
				lastDir = wwd;
				stop = 1;
				
			}
		}
	}

	MLC();

	MoveLine(10,0,15,30,40);
	Od2(0,35,GetAng(lastDir,(lastDir + 2) % 4),30,-40,60,130);
	
	MLC();
	lastDir = (lastDir + 2) % 4;

	lastPos = to;
}

void PutPuck(int from, int to){
	
	int stop = 1;
	min = 12;
	DFS(from,to,0);
	
	MLC();

	for(int i = 0 ; i < min ; i++){
		int wd = GetDir(g_route[i],g_route[i+1]);
		int wwd = (i + 2 <= min)? GetDir(g_route[i + 1],g_route[i + 2]) : -1;
		

		if(wwd >= 0){
			//아직 가야할 경로 남아있음 
			if(lastDir != wd)	Axis(0,0,GetAng(lastDir,wd),60,130); 
			lastDir = wd;
			
			if(wd== wwd){
				MoveLine(1,0,1000,40,50 * stop);
				stop = (i >= min - 2);
				
				MoveLine(10,0,12,40,-50 * stop);
				
			}
			else{				
				MoveLine(10,0,10,40,50 * stop);
				Od2(0,40,GetAng(lastDir,wwd),40,-50,100,120);
				MLC();
				
				lastDir = wwd;
				stop = 1;
				
			}
		}	
	}
	
	MLC();
	set
	MoveLine(10,0,26,30,140);
	Axis(0,-26,0,30,140);

	lastPos = g_route[min-1];
}
