#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int node[13] = {
	0,3,0,
	 1,1,
	0,1,0,
	 2,0,
	0,0,0
};

int dist[2], route[2][10];
int seq[8] = { 1,4,7,9,11,8,5,3 };
int nowPos;
//하나의 인덱스에서 왼쪽으로 갈 수 있는 노드, 오른쪽으로 갈 수 있는 노드를 각각 구함 

int GetIdx(int point) { //Seq 좌표 리턴  
	for (int i = 0; i < 8; i++) {
		if (point == seq[i])	return i;
	}
	return -1;
}

int GetListIdx(int val, int* str, int len) {
	for (int i = 0; i < len; i++)
		if (val == str[i]) return i;
	return -1;
}

void GetIdxInform(int f_idx, int t) {
	//type : get 0/ put 1

	int point[2];
	int close[2] = { 0,0 };
	int cnt = 1;
	route[0][0] = route[1][0] = f_idx;

	while (close[0] + close[1] != 2) {
		point[0] = seq[(GetIdx(f_idx) + cnt) % 8];
		point[1] = seq[abs(GetIdx(f_idx) - cnt)];

		for (int i = 0; i < 2; i++) {
			if (node[point[i]]) {
				close[i] = 1;
				dist[i] = cnt + 1 - t;
			}

			route[i][cnt] = point[i];
		}
		cnt++;
		//printf("cnt : %d\n",cnt);
	}
	return;
}

int check[8] = { 0 };

int GetLen(int idx) { //seq 좌표  
	check[idx] = 1;

	if (node[seq[idx]])
		return 1 + GetLen((idx + 1) % 8);
	return 0;
}

int GetMaxLenIdx() {
	int max = 0, idx = 0;
	int stp = 1; //Real Index

	for (int i = 0; i < 8; i++) {
		int pt = (GetIdx(stp) + i) % 8;
		if (check[pt]) continue;

		int val = GetLen(pt);
		if (val > max) max = val, idx = pt;
	}
	return idx;
}


void dis() {
	printf("%d %d %d\n", node[0], node[1], node[2]);
	printf(" %d %d\n", node[3], node[4]);
	printf("%d %d %d\n", node[5], node[6], node[7]);
	printf(" %d %d\n", node[8], node[9]);
	printf("%d %d %d\n", node[10], node[11], node[12]);

	system("pause");
	system("cls");
}

int main() {
	memset(node, 0, sizeof(node));
	node[seq[0]] = 1;
	node[seq[1]] = 0;
	node[seq[2]] = 2;
	node[seq[3]] = 1;
	node[seq[4]] = 0;
	node[seq[5]] = 0;
	node[seq[6]] = 3;
	node[seq[7]] = 0;
	node[0] = node[2] = 2;
	node[10] = node[12] = 3;
	node[6] = 1;

	int m_idx = GetMaxLenIdx();
	memset(check, 0, sizeof(check));

	nowPos = 4;

	if (GetLen(m_idx) != 1) {
		while (GetLen(m_idx) < 4) { //4개가 다 붙을때까지  
		out:
			int get_route[10], get_dist;
			GetIdxInform(GetIdx(nowPos), 0);

			for (int i = 0; i < 2; i++) {
				if (check[route[i][dist[i]]]) continue;
				memcpy(get_route, route[i], sizeof(route[i]));
				get_dist = dist[i];
				nowPos = route[i][dist[i]];
				//GoPos(get_route);

				GetIdxInform(GetIdx(nowPos), 0); //Put으로 검색  

				for (int j = 0; j < 2; j++) {
					if (!dist[j]) continue;
					if (check[route[j][dist[j]]]) {
						
						//GoPos();
						//printf("NOWPOS %d -> ",nowPos);
						swap(node[route[j][dist[j]]], node[get_route[get_dist]]);
						nowPos = route[j][dist[j] - 1];
						//printf("%d\n",nowPos);
					}
				}
				//dis();
				goto out;
			}
		}
	}
}