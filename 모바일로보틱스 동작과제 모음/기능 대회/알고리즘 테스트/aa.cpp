#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define ar array

//»¡=1, ÆÄ=2, ³ë=3, ·Îº¿=9
int puck=0;
int _map[4][2]={//¸Ê °ª ÀúÀå
    {0,2},
    {1,3},
    {2,0},
    {3,1}
};

int chk[4][2]={0};

void display(int a, int b){
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            cout << _map[i][j] << " ";
        }
        cout << "\n";
    }
    cout<<"\n";
}

//cout << _map[a][b] <<endl;
void solve(int a, int b){
    chk[a][b]=1;
    
    if(puck>=6){
        _map[a][b]=9;
        display(a,b);
        exit(0);
    }
    
    if(_map[a][b]==1||_map[a][b]==2||_map[a][b]==3){
        puck+=1;
        
        _map[a][b]=9;
        display(a,b);
        
        _map[a][b]=0;
        solve(2,1);
    }
    else{
        _map[a][b]=9;
        display(a,b);
    }
    
    
    


//·Îº¿ µ¿ÀÛ ÄÚµå
    if(b==1){
        if(a==3){
            _map[a][b]=0;
            solve(a,b-1);
        }
        else{
            _map[a][b]=0;
            solve(a+1,b);
        }
    }
    else if(b==0){
        if(a==0){
            _map[a][b]=0;
            solve(a,b+1);
        }
        else{
            _map[a][b]=0;
            solve(a-1,b);
        }
        
    }
}



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve(2,1);
    return 0;
}
