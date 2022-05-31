//오름차순 수 출력 
#include <stdio.h>


int back(int i){
    if(i<=0) return 1;
    back(i-1);
    printf("%d ",i);
}

int main()
{
    int n;

    scanf("%d",&n);
    back(n);

    return 0;
}
