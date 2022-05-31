#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define swap(a,b) temp = a; a = b; b = temp;

int map[25] = {
0,0,0,0,0,
0,2,0,0,0,
0,1,1,0,0,
0,3,0,0,0,
0,0,0,0,0
};
int remainCnt = 0,insideCnt = 0;
int seetPointList[4] = {0},seetCnt = 0;
int dir,dis[4][3];
int call=  0;
int posList[16],posCnt = 0;
int maxPos,mPos,min = 0,flg = 0,maxVal;
int spot[5][3],g_spot[5][3];

int Finish(int *node){
   for(int i = 0 ; i < posCnt ; i++)
      if(node[posList[i]]) return 0;   
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
        if (!c[i])
            return 0;
    }
    
    return c[0] < 0 == c[1] < 0 && c[1] < 0 == c[2] < 0;
}

void TriangleDFS(int posIndex, int countIndex, int tcnt){  
	call++;
    if(tcnt > min || flg) return;
   
    if(tcnt == min){//삼각형 완성 이후
        if(Finish(map) && !flg){
        	flg = 1;
        	memcpy(g_spot,spot,sizeof(spot));
        }
    }
    else{	
        if(countIndex < 3){    //아직 점 3개 Select 안된 경우 
			for(int i = posIndex ; i >= 0 && i < 16 ; i += dir){				
				spot[tcnt][countIndex] = posList[i];
				
				if(countIndex == 0){
					mPos = 0;
					
					for(int j = 0 ; j < seetCnt ; j++)
						if(map[mPos] < map[seetPointList[j]]) mPos = seetPointList[j];
					
					if(mPos == 0) return;
					
					for(int j = 0 ; j < 2 ; j++)
						if(i / 5 == mPos / 5 || i / 5 == mPos / 5) continue;
					
					if(dir > 0 && posList[i] >= 15) return;
					if(dir < 0 && posList[i] <= 9) return;
					
					for(int j = 0 ; j < 2 ; j++){
						if(dir > 0 && mPos <= posList[i]) return;
						if(dir < 0 && mPos >= posList[i]) return;
					}
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
   return;
}

int main(){

	for(int i = 0 ; i < 25 ; i++){
		if(map[i])	seetPointList[seetCnt++] = i;
		if(i / 5 == 0 || i / 5 == 4 || i % 5 == 0 || i % 5 == 4){
			posList[posCnt++] = i;
		}
		if(!i || map[maxPos] < map[i]) maxPos = i;
   	}

   	maxVal = map[maxPos];
	dir = (maxPos <= 12)? 1 : -1;
    
    for(int i = map[maxPos] ; i <= map[maxPos] + (maxPos >= 0) ;  i++){
       min = i;
       TriangleDFS((dir > 0)? 0 : 15,0,0);
       if(flg) break;
    }   
    
    printf("%d\n",call);
    
    for(int i = 0 ; i < 25 ; i++){
		printf("%d",map[i]);
		if(i % 5 == 4)printf("\n");
	}
    
    
	for(int i = 0 ; i < min ; i++){
		
		printf("%d %d %d\n",g_spot[i][0],g_spot[i][1],g_spot[i][2] );
		
		for(int j = 0 ;j < seetCnt ; j++){
			if(IsInsideOfTriangle(seetPointList[j],g_spot[i])){
				map[seetPointList[j]]--;
			}
		}
		for(int i = 0 ; i < 25 ; i++){
		printf("%d",map[i]);
		if(i % 5 == 4)printf("\n");
	}
	}
	
	

}
