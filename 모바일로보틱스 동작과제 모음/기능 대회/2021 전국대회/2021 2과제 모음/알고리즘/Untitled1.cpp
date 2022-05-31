#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[13] = {
	0,0,0,
	 3,0,
    1,1,0,
     2,1,
    0,0,0
};

int dist[2],way[2][10];
int seq[8] = {1,4,7,9,11,8,5,3};
int nowPos;
int check[8] = {0};


//하나의 인덱스에서 왼쪽으로 갈 수 있는 노드, 오른쪽으로 갈 수 있는 노드를 각각 구함 

int GetIdx(int point){ //Seq 좌표 리턴  
	for(int i = 0 ; i < 8 ; i++){
		if(point == seq[i])	return i;
	}
}

void GetIdxInform(int idx){ //스퀘어 좌표 
	//type : get 0/ put 1
	int point[2];
	int close[2] = {0,0};
	int cnt = 1;
	way[0][0] = way[1][0] = seq[idx];
	
	while(close[0] + close[1] < 2){
		int val = idx - cnt;
		if(val < 0) val += 8;
		
		point[0] = seq[(idx + cnt)%8]; //right way
		point[1] = seq[val]; //left Route
		
		//printf("%d %d\n",point[0],point[1]);
		for(int i = 0 ; i < 2;  i++){
			if(map[point[i]] && !close[i]){
				close[i] = 1;
				dist[i] = cnt;
			}
			
			way[i][cnt] = point[i];
		}
		cnt++;
	}
	
	return;
} 



//Square 기준 서술 
int GetLen(int idx){ //seq 좌표  
	
//	printf("idx %d\n",idx);
	if(map[seq[idx]]) {
		check[idx] = 1;
		int l = (idx - 1) + (idx == 0) * 8;
		int r = (idx + 1) % 8;
		int tot = 0;
		
		if(!check[l])  tot += GetLen(l);
		if(!check[r])  tot += GetLen(r);

		return 1 + tot;
	}
	return 0;
}

int GetMaxLenIdx(){
	int max = 0,idx = 0;
	
	for(int i = 0 ; i < 8 ; i++){
		if(check[i]) continue;
		int val = GetLen(i);
		if(val > max) max = val, idx = i;
	}
	return idx;
}
		

void dis(int nowPos){
	int save = map[nowPos];
	map[nowPos] = 8;
	
	
	printf("\n\n%d %d %d\n",map[0],map[1],map[2]);
	printf(" %d %d\n",map[3],map[4]);
	printf("%d %d %d\n",map[5],map[6],map[7]);
	printf(" %d %d\n",map[8],map[9]);
	printf("%d %d %d\n\n",map[10],map[11],map[12]);
	
	map[nowPos] = save;
	
	printf("  %d\n",check[0]);
	printf(" %d %d\n",check[7],check[1]);
	printf("%d   %d\n",check[6],map[2]);
	printf(" %d %d\n",check[5],check[3]);
	printf("  %d\n",check[4]);
	system("pause");
	system("cls");
}

int main(){
	
	int m_idx = GetMaxLenIdx();
	memset(check,0,sizeof(check));

	nowPos = 4;
	
	if(GetLen(m_idx) != 1){
		while(GetLen(m_idx) < 4){ //4개가 다 붙을때까지  
			dis(nowPos);
			int get_way[2][10],get_dist[2];
			//printf("1");
			for(int i = 0 ; i < 2; i++){
				GetIdxInform(GetIdx(nowPos));
				
				if(check[GetIdx(way[i][dist[i]])]) continue;
	
				memcpy(get_way,way,sizeof(way));
				memcpy(get_dist,dist,sizeof(dist));

				for(int k = 0 ; k <= get_dist[i] ; k++)
					printf("%d -> ",get_way[i][k]);
					
				GetIdxInform(GetIdx(get_way[i][get_dist[i]]));
				
				for(int j = 0 ; j < 2 ; j++){
					if(check[GetIdx(get_way[j][get_dist[j]])]){
						//GoPos();
						
						swap(map[way[j][dist[j]-1]],map[get_way[i][get_dist[i]]]);
						printf("From %d\n ",nowPos);
						printf("TO %d Get %d",way[j][dist[j] - 1],get_way[i][get_dist[i]]);
						
						nowPos = way[j][dist[j]-2];
						printf("\nNOW %d",nowPos);
						dis(nowPos);
						//printf("%d\n",nowPos);
					}	
				}
				
				goto out;
			}
			out:
			memset(check,0,sizeof(check));
			GetMaxLenIdx();
			memset(check,0,sizeof(check));
		}
		
		int std_idx;
		for(int i = 0 ; i < 8 ; i++)
			if(check[i] && !check[(i+1)%8]) std_idx = i;
		
		printf("std_idx %d\n" , std_idx);
		
		for(int i = 0 ; i < 3 ; i++){
			int get = (std_idx + 5 + i) % 8;
			int put = (std_idx + 2 + 2*i) % 8;
			
			for(int i = 0 ; i < 2 ; i++){
				
			}		
			printf("get : %d",get);
			printf("put : %d",put);
			
			swap(map[seq[put]],map[seq[get]]);		
			dis(0);
		}
	}
}
