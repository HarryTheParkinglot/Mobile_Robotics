#include <stdio.h>
int cnt,re= 1;

int fact(int i){
    if(i==1) return 1;
    return i * fact(i-1);
}

int main()
{
    scanf("%d",&cnt);
    printf("%d",fact(cnt));
    return 0;
}
