#include <stdio.h>
   int m[10]={0,};
void RGB(int x){
   if(x==9){
      for(int i=0;i<9;i++){
         printf("%d",m[i]);
         if(i%3==2)
         printf("\n");
      }
      
      printf("\n");
      m[x]=0;
      return;
   }
      for(int i=1;i<=3;i++){ 
        if(x%3 && m[x-1] == i) continue;
        if(m[x-3] == i) continue;   
         
		m[x]=i;            
        RGB(x+1);
         
        m[x]=0;
      }   
   }

int main(){
   //1ÆÄ 2 »¡ 3³ë 
   RGB(0);

   
}
