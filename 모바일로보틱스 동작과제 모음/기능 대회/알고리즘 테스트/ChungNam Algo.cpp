#include <stdio.h>
#include <memory.h>
#include <windows.h>

signed char tmp;
#define swap(a,b) tmp = a, a = b,  b = tmp;



signed char line[6] = {3,2,1,2,1,2};
signed char storage[6][2];
signed char goal[6]=  {2,3,2,1,2,1};

signed char node[8] = {
	1,2,2,3,
	2,0,3,2	
};
signed char link[8][4] = {0};
signed char route[8] = {0};
signed char g_route[8] = {0};
signed char check[8] = {0};
signed char min = 10;
signed char flg = 0;

signed char orderList[4] = {0};

void CreateLink(signed char p, signed char *str){
	memset(str,-1,sizeof(signed char) *4);
	
	if(p / 4 == 1) 		str[0] = p - 4;
	if(p % 4 - 1 >= 0) 	str[1] = p - 1;
	if(p / 4 == 0) 		str[2] = p + 4;
	if(p % 4 + 1 <= 3) 	str[3] = p + 1;
}


void display(){
	printf("\n  ");		
	for(int i = 1 ; i <= 4 ; i++)
		if(storage[i][1])	printf("%d ",storage[i][0]);
		else	printf("  ");
		
	printf("\n");		
	for(int i = 0 ; i <= 5 ; i++)
		printf("%d ",line[i]);
	printf("\n  ");		
	
	for(int i = 1 ; i <= 4 ; i++)
		if(!storage[i][1])	printf("%d ",storage[i][0]);
		else	printf("  ");
	printf("\n\n");	
	
	for(int i = 0 ; i <= 5 ; i++)
		printf("%d ",goal[i]);
		
	system("pause");
	system("cls");
}

void LtoS(signed char from, signed char to){
	swap(line[from], storage[to][0]);
	display();
}
void LtoL(signed char from, signed char to){
	swap(line[from], line[to]);
	display();
}
void StoS(signed char from, signed char to){
	swap(storage[from][0], storage[to][0]);
	display();
}
void StoL(signed char from, signed char to){
	swap(storage[from][0], line[to]);
	display();
}

void Puck_Change(signed char from, signed char to){

	if(from > to) swap(from,to);
	
	if(from % 5 == 0 || to % 5 == 0){
		if(from == 0){
			StoL(to,to+1);
			LtoS(from,to);
			LtoL(to+1,from);
		}
		else{
			StoL(from,from-1);
			LtoS(to,from);
			LtoL(from-1,to);
		}
	}
		
	else{
		if(from == 1){
			StoL(from,to+1);
			StoS(to,from);
			LtoS(to+1,to);		
		}	
		else{
			StoL(from,from-1);
			StoS(to,from);
			LtoS(from-1,to);		
		}	
	}
}

void DFS(signed char from, signed char to , signed char dp){
	if(dp >= min) return;
	
	route[dp]= from;
	
	if(from == to){
		min = dp;
		for(int i = 0 ; i <= dp; i++)	g_route[i] = route[i];
		
		return;
	}
	check[from] = 1;
	for(int i = 0 ; i < 4; i++){
		int next = link[from][i];
		if(next < 0 || check[next] || (to != next && node[next])) continue;
		DFS(next,to,dp+1);
	}
	check[from] = 0;
}

void MakeOrder(signed char *colorN, signed char order){
	if(flg) return;
	
	if(order == 4)	flg = 1;
	
	signed char save;
	
	for(int i = 0 ; i < 8 ; i++){
		if(!node[i] || !colorN[node[i]]) continue;
		
		min = 10;
		DFS(5,i,0);		
		if(min == 10) continue;
		save = node[i];
		node[i] = 0;	colorN[node[i]]--;	orderList[order] = i;
		
		MakeOrder(colorN,order+1);
		
		node[i] = save;	colorN[node[i]]++;
		
		if(flg) return;
	}
	
}

int main(){
	
	int index = 1;
	storage[index++][1] = 1;
	storage[index++][1] = 0;
	storage[index++][1] = 1;
	storage[index++][1] = 0;

	
	for(int i = 1;  i <= 4;  i++)
		swap(line[i],storage[i][0]);
	
	display();
	
	if(line[0] != goal[0]){
		zero_rt:
		int flg = 0;
		for(int i = 1 ; i <= 3 ; i++){
			if((storage[i][0] == goal[i] || storage[i][0] != goal[0])) continue;
			Puck_Change(0,i);
			flg = 1;
			break;
		}
		if(!flg){
			if(storage[4][0] != goal[4] && storage[4][0] == goal[0]){
				for(int i = 3 ; i >= 2 ; i--){
					if(storage[i][0] == goal[i]) continue;
					Puck_Change(4,i);
					break;
				}
			}
			else{
				for(int i = 4 ; i >= 2 ; i--){
					if(storage[i][0] == goal[i]) continue;
					Puck_Change(5,i);
					break;
				}
			}
			goto zero_rt;
		}
	}
	printf("ZERO : FIN\n");
	if(line[5] != goal[5]){
		five_rt:
		int flg = 0;
		for(int i = 4 ; i >= 2 ; i--){
			if((storage[i][0] == goal[i] || storage[i][0] != goal[5])) continue;
			Puck_Change(5,i);
			flg = 1;
			break;
		}
		if(!flg){
			if(storage[1][0] != goal[1] && storage[1][0] == goal[5]){
				for(int i = 2 ; i >= 3 ; i++){
					if(storage[i][0] == goal[i]) continue;
					Puck_Change(1,i);
					break;
				}
			}
			else{
				for(int i = 1 ; i >= 3 ; i++){
					if(storage[i][0] == goal[i]) continue;
					Puck_Change(0,i);
					break;
				}
			}
			goto five_rt;
		}
	}
	
	printf("FIVE : FIN\n");
	for(int i = 1 ; i <= 4 ; i++){
		if(storage[i][0] == goal[i]) continue;
		for(int j = i+1 ; j <= 4 ; j++){
			if(goal[i] != storage[j][0] || storage[j][0] == goal[j]) continue; //필요한 퍽이 아닌경우  
			if(abs(i - j) == 3)	{Puck_Change(i,2); j = 2;}
			Puck_Change(i,j);	break;
		}
	}
	
	signed char colors[4] = {0};
	
	for(int i = 1;  i <= 4;  i++){
		colors[goal[i]]++;
	 	swap(line[i],storage[i][0]);
	}
	
	for(int i = 0 ; i < 8 ; i++)
		CreateLink(i,link[i]);
		
	MakeOrder(colors,0);
	
	for(int i = 0 ; i < 4; i ++)
		printf("%d ",orderList[i]);

	
}
