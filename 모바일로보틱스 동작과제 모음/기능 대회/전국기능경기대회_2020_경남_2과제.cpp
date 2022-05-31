#include <stdio.h>

int arr[2][4]={0,}; // A 구역 배열
int trash[100]={0,}; //쓰래기 값 저장
int line[4]; //C 구역 선 확인
int block[4][2]; //블록 색 저장

int c1=0,c2=0, c3=0, c4=0; //퍽 수 증가량 저장
int t1=0; // 쓰래기값 증가량 저장
//빨강 1, 파랑 2, 노랑 3
//검 1, 금 2
int main()
{
    //선 탐색
    printf("선 값을 적어주시오(검=1, 금=2)\n");
    for(int i=0;i<4;i++)
        scanf("%d", &line[i]);
    //좌우탐색
    printf("좌우 퍽 값을 적어주시오(빨강=1, 파랑=2, 노랑=3)\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            scanf("%d", &block[i][j]);
        }
    }
    //arr 0열 이동
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[0]==2&&block[i][j]==1){
                trash[t1]=1;
                t1++;
                block[i][j]=0;
                c1++;
            }
            else if(block[i][j]==1){
                arr[c1][0]=1;
                c1++;
                block[i][j]=0;
            }
             if(c1==2){i=4;
			 break;}
        }
    }

    //arr 1열 이동
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[1]==2&&block[i][j]==2){
                trash[t1]=2;
                t1++;
                block[i][j]=0;
                c2++;
            }
            else if(block[i][j]==2){ 
                arr[c2][1]=2;
                c2++;
                block[i][j]=0;
            }
            if(c2==2){i=4;
			 break;}
    	}
    }

    //arr 2열 이동
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[2]==2&&block[i][j]==3){
                trash[t1]=3;
                t1++;
                block[i][j]=0;
                c3++;
            }
            else if(block[i][j]==3){ 
                arr[c3][2]=3;
                c3++;
                block[i][j]=0;
            }
            if(c3==2){i=4;
			 break;}
        }
    }

//arr 3열 이동
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[3]==2&&block[i][j]==2){
                trash[t1]=2;
                t1++;
                block[i][j]=0;
                c4++;
            }
            else if(block[i][j]==2){ 
                arr[c4][3]=2;
                c4++;
                block[i][j]=0;
            }
            if(c4==2){i=4;
			 break;}
        }
    }
    printf("A 구역 퍽 배치 출력(0=빈칸, 1=빨강, 2=파랑, 3=노랑)\n");
    for(int i=0; i<2; i++){
        for(int j=0; j<4; j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
    printf("쓰래기값 출력\n");
    for(int i=0; i<t1; i++)
        printf("%d",trash[i]);

    printf("\n====오류 출력====\n");
    printf("c1=%d, c2=%d, c3=%d, c4=%d, t1=%d\n",c1,c2,c3,c4,t1);

    return 0;
}
