#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
int aMap[16] = {
   0,0,0,0,
   0,0,0,0,
   0,0,0,4,
   2,3,0,1
};
int block = 11;
int bMap[8] = {
   2,3,1,0,
   2,3,1,0
};

int aMapGoal[16] = {
   0,2,0,0,
   3,0,1,0
};
int bMapGoal[8] = { 0 };

int dfs_route[4][10], g_dfs_route[4][10], dfs_min[4] = { 20 }, convert_min = 10, check[16];
int g_route[15], route[15], min, flg = 0;
int wayList[4] = { -1,-1,-1,-1 };
int a_link[16][8];
int b_link[8][4];

void ALink(int p) {
    memset(a_link[p], -1, sizeof(int) * 8);

    if (p / 4 > 0) a_link[p][0] = p - 4;
    if (p % 4 > 0) a_link[p][1] = p - 1;
    if (p / 4 < 3) a_link[p][2] = p + 4;
    if (p % 4 < 3) a_link[p][3] = p + 1;

    if (a_link[p][0] > 0) {
        if (a_link[p][1] > 0) a_link[p][4] = p - 5;
        if (a_link[p][3] < 3) a_link[p][7] = p - 3;
    }

    if (a_link[p][2] < 3) {
        if (a_link[p][1] > 0) a_link[p][5] = p + 3;
        if (a_link[p][3] < 3) a_link[p][6] = p + 5;
    }
}

void BLink(int p) {
    memset(b_link[p], -1, sizeof(int) * 4);

    if (p / 4 > 0) b_link[p][0] = p - 4;
    if (p % 4 > 0) b_link[p][1] = p - 1;
    if (p / 4 < 1) b_link[p][2] = p + 4;
    if (p % 4 < 3) b_link[p][3] = p + 1;
}

int Sorted(int* node, int* goal, int len) {
    for (int i = 0; i < len; i++) {
        if (bMapGoal[i] < 0) continue;
        if (node[i] != goal[i]) return 0;
    }

    return 1;
}
/*
void display() {
    for (int i = 0; i < 16; i++) {
        printf("%-4d", aMap[i]);
        if (i % 4 == 3) printf("\n");
    }

    printf("\n\n");

    for (int i = 0; i < 16; i++) {
        printf("%-4d", aMapGoal[i]);
        if (i % 4 == 3) printf("\n");
    }

    printf("\n\n");

    for (int i = 0; i < 8; i++) {
        printf("%-4d", bMap[i]);
        if (i % 4 == 3) printf("\n");
    }

    printf("\n\n");

    for (int i = 0; i < 8; i++) {
        printf("%-4d", bMapGoal[i]);
        if (i % 4 == 3) printf("\n");
    }

    system("pause");
    system("cls");
}
*/
int GetWay(int p, int np) {
    if (abs(p - np) == 4) return 0; //Vertical
    if (abs(p - np) == 1) return 1; //Horizon
    return 2; //diagonal
}

int wayAlreadyUsed(int color, int type) {
    for (int i = 1; i < 4; i++) {
        if (i == color || wayList[i] < 0) continue;
        if (wayList[i] == type) return 1;
    }
    return 0;
}

void DFS(int color, int pastWay, int p, int wp, int dp, int wayConvert) { //색깔 보유시
    if (wayConvert >= convert_min || (wayConvert == convert_min && dp >= dfs_min[color])) return;

    dfs_route[color][dp] = p;

    if (p == wp) {
        if (wayAlreadyUsed(color, pastWay)) return;

        wayList[color] = pastWay;
        dfs_min[color] = dp;
        convert_min = wayConvert;
        memcpy(g_dfs_route, dfs_route, sizeof(dfs_route));
        return;
    }

    check[p] = 1;

    for (int i = 0; i < 8; i++) {
        int np = a_link[p][i];

        if (np < 0 || check[np] || (np != wp && aMap[np])) continue;
        int nowWay = GetWay(p, np);
        if (p / 4 == 2 && nowWay == 1) continue; //두번째 라인에서는 좌우이동 불가능

        int plus = ((nowWay != pastWay) || (np / 4 == 2 && bMap[np % 4 - (np % 4 > block % 4)] != color)); //색깔이 다른 경우 convert필요
        DFS(color, nowWay, np, wp, dp + 1, wayConvert + plus);
    }
    check[p] = 0;
}

bool CheckDFS(int color, int pastWay, int p, int wp) {
    dfs_min[color] = 8;
    convert_min = 4;
    memset(check, 0, sizeof(check));
    DFS(color, pastWay, p, wp, 0, 0);

    return dfs_min[color] != 8;
}

void Sort(int lp, int p, int dp, int* node) {
    if (dp >= min) return;

    route[dp] = p;

    if (Sorted(node, bMapGoal, 8)) {
        flg = 1;
        min = dp;

        memcpy(g_route, route, sizeof(route));
        return;
    }

    for (int i = 0; i < 4; i++) {
        int np = b_link[p][i];

        if (np < 0 || np == lp) continue;
        swap(node[p], node[np]);
        Sort(p, np, dp + 1, node);
        swap(node[p], node[np]);
    }
}

void SetGateWay(int color, int gate, int way) {
    int null;
    memset(bMapGoal, -1, sizeof(bMapGoal));

    bMapGoal[gate] = color;
    bMapGoal[way + 4] = color;

    for (int i = 0; i < 8; i++)
        if (!bMap[i])   null = i;

    for (int i = 0; i < 20; i++) {
        min = i;
        flg = 0;

        Sort(-1, null, 0, bMap);

        if (flg) {
            return;
        }
    }
}

void SetFinalGateWay() {
    memset(bMapGoal, -1, sizeof(bMapGoal));

    for (int i = 1; i <= 3; i++)
        bMapGoal[wayList[i] + 4] = i;

    for (int i = 1; i <= 3; i++) {
        int p = g_dfs_route[i][dfs_min[i] - 1];

        if (p / 4 == 2) {
            int gate = (p % 4 > block) ? p % 4 - 1 : p % 4;
            bMapGoal[gate] = i;
        }
    }

    int cnt = 0, val = 0, null[2] = { 0 };

    for (int i = 0; i < 8; i++) {
        if (!bMap[i]) {
            null[cnt++] = i;
            bMap[i] = val--;
        }
    }

    for (int i = 0; i < 20; i++) {
        min = i;
        flg = 0;

        for (int j = 0; j < 2; j++) {
            Sort(-1, null[j], 0, bMap);
            if (flg) return;
        }
    }
}

int main() {
    int val = 0;
    for (int i = 0; i < 16; i++)   ALink(i);
    for (int i = 0; i < 8; i++) {
        BLink(i);
        if (!bMap[i]) bMap[i] = val--;
    }


    //Mirror

    for (int i = 0; i < 8; i++) {
        //display();
        int color = aMapGoal[i];
        if (Sorted(aMap, aMapGoal, 16)) break;
        if (!color) continue;

        for (int j = 12; j < 16; j++) {
            if (color != aMap[j]) continue; //색이 다르면 패스

            int nowWay = -1, nowGate = -1, goalGate, goalWay;

            for (int t = 0; t < 3; t++) {
                if (bMap[t + 4] == color) nowWay = t;
                if (bMap[t] == color) nowGate = t + (t >= block % 4);
            }

            CheckDFS(color, nowWay, j, i);
            //printf("min : %d\n wayList %d\n", dfs_min[color], wayList[color]);
            for (int t = 0; t <= dfs_min[color]; t++) {
                if (g_dfs_route[color][t] / 4 == 2) goalGate = g_dfs_route[color][t] % 4 - (g_dfs_route[color][t] % 4 >= block % 4);
                printf("%d -> ", g_dfs_route[color][t]);
            }

            for (int t = 0; t < dfs_min[color] - 1; t++) {
                goalWay = GetWay(g_dfs_route[color][t], g_dfs_route[color][t + 1]);
                //printf("\nMOVE FLG %d\n",f);

                //printf("\ncolor %d gate %d way %d \n", color, nowGate, nowWay);
                //printf("\nmin %d",dfs_min);
                //printf("ggate %d gway %d\n",goalGate,goalWay);

                //display();
                //Controller 조정

                if (nowGate != goalGate || goalWay != nowWay) {
                    SetGateWay(color, goalGate, goalWay);
                    printf("ChangeExecute\n");
                    //if(t) GetOutA(g_dfs_route[t]); //현 위치에서 나가기
                    //GetInB(g_route[0]);

                    for (int w = 0; w < min; w++) {
                        int p = g_route[w];
                        int np = g_route[w + 1];

                        //if(bMap[np]) BChangeMove(p,np);
                        //else BJustMove(p,np);

                        swap(bMap[p], bMap[np]);
                        //display();
                    }
                    //GetOutB(g_route[min]);

                    nowWay = goalWay;
                    nowGate = goalGate;
                    //GetInA(g_dfs_route[0]); //
                    //Move(g_dfs_route[0],g_dfs_route[t+1]); //퍽 먹기까지
                }
                swap(aMap[g_dfs_route[color][t]], aMap[g_dfs_route[color][t + 1]]);
                //if(t + 1 != g_dfs_min)   AJustMove(g_dfs_route[t],g_dfs_route[t+1]);
                //else AChangeMove(g_dfs_route[t],g_dfs_route[t+1]);
            }
            break;
        }
    }
    SetFinalGateWay();
    //display();

    //printf("Final\n\n");
    //GetInB(g_route[0]);
    for (int w = 0; w < min; w++) {
        int p = g_route[w];
        int np = g_route[w + 1];

        //if(bMap[np]) BChangeMove(p,np);
        //else BJustMove(p,np);

        swap(bMap[p], bMap[np]);
        //display();
    }
    //GetOutB(g_route[min]);   

    int check[4] = { 0 };
    for (int j = 0; j < 2; j++) {
        for (int i = 1; i < 4; i++) {
            int m = dfs_min[i];
            int p = g_dfs_route[i][m - 1];
            int np = g_dfs_route[i][m];
            if (aMap[np] || check[i]) continue;

            //GetInA(g_dfs_route[color][m-1]);
            //printf("PutPuck %d -> %d\n", p, np);
            //PutPuck(p,np);
            swap(aMap[p], aMap[np]);
            check[i] = 1;
            //display();
        }
    }
}
