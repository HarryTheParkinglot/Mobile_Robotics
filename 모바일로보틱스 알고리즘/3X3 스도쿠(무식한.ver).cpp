#include <stdio.h>
int arr[10]={0};
int main()
{
    int t;
    for(long long int i=123456789; i<=987654321; i++){
        t=i;
        for(int j=1; j<=9; j++){
            arr[j]=i%10;
            i/=10;
        }
        i=t;
        if(arr[1]!=arr[2]&&arr[1]!=arr[3]&&arr[1]!=arr[4]&&arr[1]!=arr[5]&&arr[1]!=arr[6]&&arr[1]!=arr[7]&&arr[1]!=arr[8]&&arr[1]!=arr[9]
        &&arr[2]!=arr[3]&&arr[2]!=arr[4]&&arr[2]!=arr[5]&&arr[2]!=arr[6]&&arr[2]!=arr[7]&&arr[2]!=arr[8]&&arr[2]!=arr[9]
        &&arr[3]!=arr[4]&&arr[3]!=arr[5]&&arr[3]!=arr[6]&&arr[3]!=arr[7]&&arr[3]!=arr[8]&&arr[3]!=arr[9]
        &&arr[4]!=arr[5]&&arr[4]!=arr[6]&&arr[4]!=arr[7]&&arr[4]!=arr[8]&&arr[4]!=arr[9]
        &&arr[5]!=arr[6]&&arr[5]!=arr[7]&&arr[5]!=arr[8]&&arr[5]!=arr[9]
        &&arr[6]!=arr[7]&&arr[6]!=arr[8]&&arr[6]!=arr[9]
        &&arr[7]!=arr[8]&&arr[7]!=arr[9]
        &&arr[8]!=arr[9]&&arr[1]!=0&&arr[2]!=0
        &&arr[3]!=0&&arr[4]!=0&&arr[5]!=0&&arr[6]!=0&&arr[7]!=0&&arr[8]!=0&&arr[9]!=0){
            for(int j=1; j<=9; j++){
                printf("%d ",arr[j]);
                if(j%3==0)
                printf("\n");
            }
            printf("\n");
        }
    }
    return 0;
}
