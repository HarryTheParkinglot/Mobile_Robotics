#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define swap(a,b) temp = a; a = b; b = temp;

int map[25] = {
0,0,0,0,0,
0,2,0,2,0,
0,0,1,0,0,
0,0,0,3,0,
0,0,0,0,0
};

int call = 0,rTime = 0;
int remainCnt = 0,insideCnt = 0;
int seetPointList[4] = {0},seetCnt = 0;
int dir,dis[4][3];
int posList[16],posCnt = 0;
int maxPos,mPos,min = 0,flg = 0;
int spot[5][3],g_spot[5][3];

int Finish(int *node){
   for(int i = 0 ; i < seetCnt ; i++)
      if(node[seetPointList[i]]) return 0;   
   return 1;
}

int ccw(int p1, int p2, int p3) {
    double d1[2] = { p1 % 5, p1 / 5 };
    double d2[2] = { p2 % 5, p2 / 5 };
    double d3[2] = { p3 % 5, p3 / 5 };

    return (d2[0] - d1[0]) * (d3[1] - d1[1]) - (d2[1] - d1[1]) * (d3[0] - d1[0]); 
}

int IsInsideOfTriangle(int p, int* tri) {
    int c[3];
    for (int i = 0; i < 3; i++) {
        c[i] = ccw(tri[i], tri[(i + 1) % 3], p);
        if (!c[i])	return 0;
    }
    return c[0] < 0 == c[1] < 0 && c[1] < 0 == c[2] < 0;
}


void TriangleDFS(int posIndex, int countIndex, int tcnt){  
	call++;
	
	if(call > 1000) {
		flg = (flg == 1)? 1 : 2;
		return;
	}
	
	if(tcnt > min) return;
	
    if(countIndex == 0){
		if(Finish(map)){
        	flg = 1;
        	min = tcnt;
        	memcpy(g_spot,spot,sizeof(spot));
        	return;
        }
    }
      
    if(tcnt < min && countIndex < 3){    //아직 점 3개 Select 안된 경우 
		for(int i = posIndex ; i >= 0 && i < 16 ; i += dir){				
			spot[tcnt][countIndex] = posList[i];
			
			if(countIndex == 0){
				mPos = 0;
				
				for(int j = 0 ; j < seetCnt ; j++)
					if(map[mPos] < map[seetPointList[j]]) mPos = seetPointList[j]; 
				
				if(i / 5 == mPos / 5 || i / 5 == mPos / 5) continue;
				
				if(mPos == 0) return;
				if(dir > 0 && (posList[i] >= 15 || mPos <= posList[i])) return;
				if(dir < 0 && (posList[i] <= 9 || mPos >= posList[i])) return;
			}
			
			else if(countIndex == 1){
				if(mPos % 5 == spot[tcnt][0] % 5 && mPos % 5 == spot[tcnt][1] % 5) continue;
           		if(mPos / 5 == spot[tcnt][0] / 5 && mPos / 5 == spot[tcnt][1] / 5) continue;
           		if(abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][0]/5 + spot[tcnt][0]%5) && abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][1]/5 + spot[tcnt][1]%5)) continue;
        		if(abs(mPos / 5 - mPos % 5) == abs(spot[tcnt][0]/5 - spot[tcnt][0]%5) && abs(mPos / 5 + mPos % 5) == abs(spot[tcnt][1]/5 - spot[tcnt][1]%5)) continue;
    		}
					
			else if(countIndex == 2){
				memset(dis,0,sizeof(dis));
				
				for(int j = 0 ; j < 3 ; j++)
					dis[tcnt][j] += (abs(spot[tcnt][j]/5 - spot[tcnt][(j+1%3)]/5) + abs(spot[tcnt][j]%5 - spot[tcnt][(j+1)%3]%5));
				
				if(dis[tcnt][0] == 1 || dis[tcnt][1] == 1 || dis[tcnt][2] == 1)
					if(dis[tcnt][0] + dis[tcnt][1] + dis[tcnt][2] < 12) continue;
					
				else if(dis[tcnt][0] + dis[tcnt][1] + dis[tcnt][2] < 10) continue;
           	}
           	TriangleDFS(i+dir,countIndex+1,tcnt); //다음 점부터 검사     
   		}
	}
     
    else{   //세 점의 Select 완료된 경우          
        for(int j = 0 ; j < tcnt ; j++){
           for(int i = 0 ; i < 3 ; i++){
              if(spot[tcnt][i] != spot[j][i]) break; //중복제거 
              if(i == 2) return;
           }
       	}
		insideCnt = remainCnt = 0;
		
		if(!IsInsideOfTriangle(mPos,spot[tcnt])) return;
			
		for(int i = 0 ; i < seetCnt ; i++)
			if(map[seetPointList[i]]) remainCnt ++; 
		
		for(int t = 0 ; t < 2 ; t++){
            for(int i = 0 ; i < seetCnt ; i++){
				if(!IsInsideOfTriangle(seetPointList[i],spot[tcnt])) continue;
				if(!map[seetPointList[i]]) return;
				insideCnt ++;
				
				if(!t) continue;
				map[seetPointList[i]] --;		
			}
			
			if(insideCnt < ((remainCnt > 3)? 3 : remainCnt - (remainCnt != 1))) return;
		}
			
		TriangleDFS((dir > 0)? 0 : 15,0,tcnt+1); 
      	
      	for(int i = 0 ; i < seetCnt ; i++){
			if(!IsInsideOfTriangle(seetPointList[i],spot[tcnt])) continue;
			map[seetPointList[i]] ++;
		}
    }
}

void Rotate(int *str){
	int copy[25];
	
	for(int i = 0 ; i < 5 ; i++)
		for(int j = 0 ; j < 5 ; j++)
			copy[i*5 + j] = str[(4 - j)*5 + i];

	memcpy(str,copy,sizeof(copy));
}

int GetRotatedVal(int val, int time){
	for(int i = 0 ; i < time ; i++)
		val = (4 - val % 5)*5 + val/5;
	return val;
}

int main(){
	
	int put[25] = {0};
	for(int t = 0 ;t < 4 ; t++){
		rTime = t;
		seetCnt = posCnt = flg = 0;
		
		
		
		for(int i = 0 ; i < 25 ; i++){
			if(map[i])	seetPointList[seetCnt++] = i;
			if(i / 5 == 0 || i / 5 == 4 || i % 5 == 0 || i % 5 == 4) posList[posCnt++] = i;
			if(!i || map[maxPos] < map[i]) maxPos = i;
		}
		
		min = map[maxPos] + 1;
		
		for(int i = 0 ; i < 2 ; i++){
			call = 0;
			dir = (i)? 1 : -1;
			
		   	TriangleDFS((dir > 0)? 0 : 15,0,0);
		   	
		   	if(flg == 1) goto exit;
	   	}
	   	
	   	Rotate(map);
	}
    
	exit:
    
	printf("FLG  %d M %d\n",flg,min);
	for(int i = 0; i < min ; i++){
		for(int j = 0 ; j < 3 ; j++){
			g_spot[i][j] = GetRotatedVal(g_spot[i][j],rTime);
			printf("%d ",g_spot[i][j]);
		}
		printf("\n");
	}
}
