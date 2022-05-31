#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define color(n) colorPos[tri_cnt][n]
#define colorX(n) colorPos[tri_cnt][n] % 5 
#define colorY(n) colorPos[tri_cnt][n] / 5

int temp = 0;
int cnt = 0;
int first = 0; 
int threePos = -1;

#define swap(a,b) temp = a; a = b; b = temp;

//������ ������ ���� / --> dx / dy 


int map[25] = {
0,0,0,0,0,
0,0,0,0,0,
0,0,3,0,0,
0,2,1,2,0,
0,0,0,0,0
};

int posList[16];
int min = 0;
int colorPos[5][3] = {0};
int g_colorPos[5][3] = {0};
int territory[25] = {0};
//int array[25] = {0};

int f_flg = 0;//b / r�� ��� 1�̸� f_flg = 1 

int Finish(int *node, int *nodeStack){
   for(int i = 0 ; i < 25 ; i++){
      if(!node[i]) continue; // ���� ��ŵ 
      if(nodeStack[i] != node[i]) return 0;
   }
   return 1;
}

int SetTerritory(int *points, int *output) {
    int cpPoints[3] = {0};
   
	memcpy(cpPoints,points,sizeof(int) * 3);
	memset(output,0,25 * sizeof(int));

    int tmp = 0;
 
   
  	for(int j =  0 ; j < 2 ; j++)
   		for(int i = 0  ; i < 2 ; i++)
       		if(cpPoints[i] % 5 > cpPoints[i+1] % 5){
           		swap(cpPoints[i],cpPoints[i+1]);
       		}
   
   //������ x��ǥ ������ ����  
   	int p[3][2] = {0};
   	float grad[3] = {0}; // 0 : 0 - 1 | 1 : 1 - 2 | 2 : 2 - 3 

    for(int i = 0 ; i < 3 ; i++){
      p[i][0] = cpPoints[i]/5; //����Ʈ�� y��ǥ    
      p[i][1] = cpPoints[i]%5; //����Ʈ�� x��ǥ 
      output[p[i][0]*5 + p[i][1]] = 2;   
   }
   
   int dx[3],dy[3];
   
   for(int i = 0 ; i < 3 ; i++){
      dx[i] = p[i][1] - p[(i+1)%3][1];
      dy[i] = p[i][0] - p[(i+1)%3][0];
       
      grad[i] = (dy[i])? (float)dx[i]/dy[i] : 99;
   }

   for(int i = 0 ; i < 3 ; i++){
      if(grad[i] == 99 && abs(dx[i]) > 1){ 
           int s = (dx[i] > 0)? p[(i+1)%3][1] : p[i][1];
           int e = (dx[i] > 0)? p[i][1] : p[(i+1)%3][1];
         
           for(int j = s + 1 ; j < e ; j++)   output[p[i][0] * 5 + j] = 2;
        }
      
      else{
         int s = p[i][0];
         int e = p[(i+1)%3][0];
         int sx = p[i][1];
          
         if(s > e){
                swap(s,e);
              sx = p[(i+1)%3][1];
           }
       
            for(int j = s + 1 ; j < e ; j++){
               float nx = sx + ((j-s) * grad[i]);
               
               if(int(nx) < nx){
                   nx = (nx > p[1][1])? (int)ceil(nx) : (int)floor(nx);
                   output[j * 5 + (int)nx]= 1;
               }   
                  else   output[j * 5 + (int)nx]= 2;            
            }
          }
   }
   int width = 0,flg = 0;
   
   	for(int i = 0 ; i < 5 ; i++){
       int cnt = 0,sx = -1;
      
       for(int j = 0 ; j < 5 ; j++){   
         if(output[i*5 + j]) {
               cnt ++;
               sx = (sx == -1)? j : sx;
           }
       }
       
      if(cnt == 2){   
      		if(threePos < 0 || territory[threePos] == 3) flg = 1;
      	
			for(int j = sx + 1 ; !output[i*5 + j] ; j++){       
	            int point = i * 5 + j;
	            if(point == threePos && territory[threePos] < 3) flg = 1; // threePos�� ä������ ���� ���¿��� 3�� �������� �ʴ� ��� 
				
				output[point] = 3;         
	               
	           	if(map[point]){
	            	if(map[point] <= territory[point])   return 0;
	        		width++;   
	           	}
	        }
    	}
    }  
	if(!flg) return 0;
   	return width;
}


void TriangleDFS(int posIndex, int countIndex, int tri_cnt){   
      cnt++;
      
      if(posIndex > 16 || tri_cnt > min || f_flg) return;
   
      if(tri_cnt == min){//�ﰢ�� �ϼ� ����
         //cnt++;
         f_flg = Finish(map,territory);  //�ϼ��� ��� return 1 �ƴϸ� 0
            
      for(int i = 0 ; i < 3 ; i++)
           memcpy(g_colorPos[i],colorPos[i],sizeof(int) * 3);
      }
      
      else{
         if(countIndex < 3){    //���� �� 3�� Select �ȵ� ��� 
           if(posIndex > 15) return;
         
           int nowPos = posList[posIndex];
           colorPos[tri_cnt][countIndex] = nowPos;
            
           int start = 1; 
          
           if(countIndex > 1)
               if(abs(colorX(0) - colorX(1)) <= 1 && abs(colorY(0) - colorY(1)) <= 1) return;
            
           else if(countIndex > 1){
               if(abs(colorX(2) - colorX(1)) <= 1 && abs(colorY(2) - colorY(1)) <= 1) start = 0;
              if(abs(colorX(2) - colorX(0)) <= 1 && abs(colorY(2) - colorY(0)) <= 1) start = 0;
           }
            
           for(int i = start; i >= 0 && !f_flg; i--)
               TriangleDFS(posIndex+1,countIndex+i,tri_cnt); //���� ������ �˻�     
         }
         
         else{   //�� ���� Select �Ϸ�� ��� 
            if(colorX(0) == colorX(1) && colorX(1) == colorX(2))  return; //x y�� ��� ���� ��� ��ŵ 
            if(colorY(0) == colorY(1) && colorY(1) == colorY(2))  return;
         
            for(int j = 0 ; j < tri_cnt ; j++){
               for(int i = 0 ; i < 3 ; i++){
                  if(colorPos[tri_cnt][i] != colorPos[j][i]) break; //�ߺ����� 
                  if(i == 2) return;
               }
           }
        
        int min_width = 0;
        
        for(int i = 0  ; i< 25 ; i++)
        	if(map[i] > territory[i]) min_width++;
        
		min_width = (min_width > 3)? 3 : min_width;
        
        	
         int array[25] = {0};
            int wid = SetTerritory(colorPos[tri_cnt],array);
            if(!wid || (wid < min_width)) return; //�� ó���� ��� 3�� ����!!  
            
            
            for(int i = 0 ; i < 25 ; i++)   
             if(array[i] == 3 && map[i])   territory[i]++;
            
           int last_index;
           for(int i = 0 ; i < 16 ; i++)
              if(posList[i] == colorPos[tri_cnt][0]) last_index = i;
        
          TriangleDFS(0,0,tri_cnt+1); 
        
           	for(int i = 0 ; i < 25 ; i++)
            	if(array[i] == 3 && map[i]) territory[i]--;         
           	
         }
   } 
   
   return;
}

int main(){
clock_t start = clock(); // ���� �ð� ����
/*

0  1  2  3  4  
5  6  7  8  9
10 11 12 13 14
15 16 17 18 19
20 21 22 23 24
  
*/  
   
      int index = 0,max = 0;
      for(int i = 0; i < 25 ; i++){
       if(i / 5 == 0 || i / 5 == 4 || i % 5 == 0 || i % 5 == 4) {
           posList[index] = i;
           index++;
           printf("%d ",posList[index-1]);
           
       }
       if(map[i]) first ++;
       if(map[i] == 3) threePos = i;
	   max = (max < map[i])? map[i] : max;
   }
   
   first = (first > 3)? 3 : first;
      printf("index : %d\n",index);
         

    
    for(int i = max ; i <= 5 ;  i++){
       min = i;
       TriangleDFS(0,0,0);
       if(f_flg) break;
    }
      
   double cal = (double)(clock() - start)/CLOCKS_PER_SEC;
    printf("%d\n\n",min);    
      
      for(int j = 0; j < 25 ; j++){   
        printf("%d ",territory[j]);
        if(j % 5 == 4)printf("\n");
    }
    
    for(int i = 0; i < min ; i++){
       printf("\n%d %d %d\n\n",g_colorPos[i][0],g_colorPos[i][1],g_colorPos[i][2]);
      
       int arr[25] = {0};
       SetTerritory(g_colorPos[i],arr);
       
       for(int j = 0 ; j < 25 ; j++)
          if(arr[j] == 3 && map[j]) territory[j]++;  
    }
    
    for(int j = 0; j < 25 ; j++){   
        printf("%d ",territory[j]);
        if(j % 5 == 4)printf("\n");
    }
   printf("f_flg : %d\n",f_flg);
   printf("cnt : %d",cnt);
   
   
   printf("%\n\n===\n%lf",cal);
   
   

}
