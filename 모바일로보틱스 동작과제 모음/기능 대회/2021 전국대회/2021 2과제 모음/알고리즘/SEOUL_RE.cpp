#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>


#define swap(a,b) temp = a; a = b; b = temp;

int map[25] = {
0,0,0,0,0,
0,2,0,0,0,
0,2,3,0,0,
0,0,1,0,0,
0,0,0,0,0
};

int posList[16];
int seetPointList[4] = {0},seetCnt = 0;
int dir;

int threePos = -1,min = 0,flg = 0;
int spot[5][3],g_spot[5][3];

int Finish(int *node){
   for(int i = 0 ; i < 25 ; i++)
      if(node[i]) return 0;   
   return 1;
}

int ccw(int p1, int p2, int p3) {
    double d1[2] = { p1 % 5, p1 / 5 };
    double d2[2] = { p2 % 5, p2 / 5 };
    double d3[2] = { p3 % 5, p3 / 5 };

    return (d2[0] - d1[0]) * (d3[1] - d1[1]) - (d2[1] - d1[1]) * (d3[0] - d1[0]); 
}

bool IsInsideOfTriangle(int p, int* tri) {
    int c[3];
    for (int i = 0; i < 3; i++) {
        c[i] = ccw(tri[i], tri[(i + 1) % 3], p);
        if (!c[i])
            return false;
    }
    
    return c[0] < 0 == c[1] < 0 && c[1] < 0 == c[2] < 0;
}

void TriangleDFS(int posIndex, int countIndex, int tcnt){  
    if(posIndex < -1 || posIndex > 25 || tcnt > min || flg) return;
   
    if(tcnt == min){//삼각형 완성 이후
        if(Finish(map) && !flg){
        	flg = 1;
        	memcpy(g_spot,spot,sizeof(spot));
        }
    }
      
    else{
        if(countIndex < 3){    //아직 점 3개 Select 안된 경우 
           	if(posIndex < 0 || posIndex > 24) return;
         
           	//int now = posList[posIndex],start = 1;
           	int start = 1;
           	
			spot[tcnt][countIndex] = posIndex;
            
			if(tcnt < 2 && ((posIndex == 24 && dir == 1) || (posIndex == 0 && dir == -1))) return;
			
           	if(countIndex >= 1)
            	if(tcnt <= 2 && abs(spot[tcnt][0]/5 - spot[tcnt][1]/5) + abs(spot[tcnt][0]%5 - spot[tcnt][1]%5) <= 1) return;
            
           	if(countIndex > 1){
           		int dis[3];
           		
				for(int i = 0 ; i < 3 ; i++)
					dis[i] += (abs(spot[tcnt][i]/5 - spot[tcnt][(i+1%3)]/5) + abs(spot[tcnt][i]%5 - spot[tcnt][(i+1)%3]%5));
				
				if(dis[0] == 1 || dis[1] == 1 || dis[2] == 1)
					if(dis[0] + dis[1] + dis[2] < 10) return;
					
				else if(dis[0] + dis[1] + dis[2] < 6)return;
           	}
           	
            if(map[posIndex]) start = 0;
           	for(int i = start; i >= 0 && !flg; i--)
            	TriangleDFS(posIndex + dir,countIndex+i,tcnt); //다음 점부터 검사     
        }
         
        else{   //세 점의 Select 완료된 경우          
        
            for(int j = 0 ; j < tcnt ; j++){
               for(int i = 0 ; i < 3 ; i++){
                  if(spot[tcnt][i] != spot[j][i]) break; //중복제거 
                  if(i == 2) return;
               }
           	}
           
         	int last_index,remainCnt = 0,insideCnt = 0;
			if(tcnt <= 2 && threePos >= 0 && !IsInsideOfTriangle(threePos,spot[tcnt])) return;
			
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
				if(tcnt == 0 && insideCnt < ((remainCnt > 3)? 3 : remainCnt - 1)) return;
				if(tcnt      && insideCnt < ((remainCnt > 2)? 2 : remainCnt - 1)) return;
				if(!insideCnt) return;
			}
	
          	TriangleDFS((dir > 0)? 0 : 24,0,tcnt+1); 
          	
          	for(int i = 0 ; i < seetCnt ; i++){
				if(!IsInsideOfTriangle(seetPointList[i],spot[tcnt])) continue;
				map[seetPointList[i]] ++;
			}
        }
   } 
   return;
}

int main(){
	
	
	for(int i = 0, index = 0; i < 25 ; i++){
		if(map[i])	seetPointList[seetCnt++] = i;
		if(map[i] == 3) threePos = i;
		//if(i / 5 == 0 || i / 5 == 4 || i % 5 == 0 || i % 5 == 4) posList[index++] = i;
   	}
   	dir = (threePos <= 10)? 1 : -1; 
    
    for(int i = 3 ; i <= 4 ;  i++){
       min = i;
       TriangleDFS((dir > 0)? 0 : 24,0,0);
       if(flg) break;
    }   
    
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
		
	}
	
	for(int i = 0 ; i < 25 ; i++){
		printf("%d",map[i]);
		if(i % 5 == 4)printf("\n");
	}

}
