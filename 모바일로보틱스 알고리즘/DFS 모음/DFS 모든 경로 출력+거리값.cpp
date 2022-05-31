#include <stdio.h>

#define node_number 5

int node_distance[node_number+1][node_number+1] = {
   {0},
   {0,0,1,3,2,0},// node_1 connection distance (I don't use node_distance[0][0])
   {0,1,0,1,0,3},
   {0,3,1,0,1,2},
   {0,2,0,1,0,1},
   {0,0,3,2,1,0}
};

int node_link[node_number+1][node_number-1] = {
   {0},
   {2,3,4}, //1st node
   {1,3,5}, //2nd node
   {1,2,4,5},
   {1,3,5},
   {2,3,4}
};

int route[node_number] = {0};
int chk[node_number + 1] = {0};
int total_distance=0;

void DFS(int start_node, int end_node, int depth){
    route[depth] = start_node;

   if(start_node == end_node){


       printf("Node Route: ");
      for(int i = 0 ; i <= depth ; i++){
        total_distance += node_distance[route[i]][route[i+1]];
        printf("%d ",route[i]);
      }
        printf("\n");
        

         
         
      printf("Total Distance: %d\n\n",total_distance);
      total_distance=0;
      return;
   }
   
   chk[start_node] = 1;
   
   for(int i = 0 ; i < node_number-1 ; i++){
      int next_node = node_link[start_node][i];
      if(next_node <= 0 || chk[next_node]) continue;
      DFS(next_node,end_node,depth+1);
   }
   chk[start_node] = 0;
   return;

}

int main(){
   DFS(1,5,0);
}
