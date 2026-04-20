
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>

using namespace std;

static const int SIZE = 9, BOX = 3;
using Grid = array<int,81>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int idx(int r,int c){return r*SIZE+c;}
int row(int i){return i/SIZE;}
int col(int i){return i%SIZE;}
int box(int i){return (row(i)/BOX)*BOX+(col(i)/BOX);}

bool canPlace(const Grid& g,int i,int v){
    int r=row(i),c=col(i);
    for(int j=0;j<SIZE;j++)
        if(g[idx(r,j)]==v||g[idx(j,c)]==v)return false;
    int br=(r/BOX)*BOX,bc=(c/BOX)*BOX;
    for(int dr=0;dr<BOX;dr++)for(int dc=0;dc<BOX;dc++)
        if(g[idx(br+dr,bc+dc)]==v)return false;
    return true;
}

bool solve(Grid& g,bool shuffle=false){
    for(int i=0;i<81;i++){
        if(g[i]!=0)continue;
        vector<int>nums={1,2,3,4,5,6,7,8,9};
        if(shuffle)std::shuffle(nums.begin(),nums.end(),rng);
        for(int v:nums){
            if(canPlace(g,i,v)){g[i]=v;if(solve(g,shuffle))return true;g[i]=0;}
        }
        return false;
    }
    return true;
}

int countSol(Grid g,int lim=2){
    for(int i=0;i<81;i++){
        if(g[i]!=0)continue;
        int cnt=0;
        for(int v=1;v<=9;v++){
            if(canPlace(g,i,v)){g[i]=v;cnt+=countSol(g,lim);g[i]=0;if(cnt>=lim)return cnt;}
        }
        return cnt;
    }
    return 1;
}

enum Diff{EASY=36,MEDIUM=46,HARD=54,EXPERT=60};

Grid makePuzzle(int remove,Grid& sol){
    sol.fill(0);
    solve(sol,true);
    Grid puzzle=sol;
    vector<int>pos(81);iota(pos.begin(),pos.end(),0);
    shuffle(pos.begin(),pos.end(),rng);
    int removed=0;
    for(int i:pos){
        if(removed>=remove)break;
        int bak=puzzle[i];puzzle[i]=0;
        if(countSol(puzzle)==1)removed++;
        else puzzle[i]=bak;
    }
    return puzzle;
}

void printGrid(const Grid& g,const Grid& given_mask){
    cout<<"\n  +---------+---------+---------+\n";
    for(int r=0;r<SIZE;r++){
        cout<<"  |";
        for(int c=0;c<SIZE;c++){
            int v=g[idx(r,c)];
            if(v==0)cout<<" . ";
            else cout<<" "<<v<<" ";
            if((c+1)%BOX==0)cout<<"|";
        }
        cout<<"\n";
        if((r+1)%BOX==0)cout<<"  +---------+---------+---------+\n";
    }
    cout<<"     1  2  3    4  5  6    7  8  9\n\n";
}

bool checkWin(const Grid& g,const Grid& sol){
    return g==sol;
}

int main(){
    cout<<"╔══════════════════════════════╗\n";
    cout<<"║       SUDOKU  C++ GAME       ║\n";
    cout<<"╚══════════════════════════════╝\n\n";
    cout<<"Difficulty: (1) Easy  (2) Medium  (3) Hard  (4) Expert\n> ";
    int d; cin>>d;
    int remove=(d==1?EASY:d==2?MEDIUM:d==3?HARD:EXPERT);

    Grid solution,grid;
    grid=makePuzzle(remove,solution);
    Grid given=grid;

    int mistakes=0;
    cout<<"\nGame started! Enter row col value (e.g. 3 5 7), or 0 0 0 to quit, or 9 9 0 to solve.\n";

    while(true){
        printGrid(grid,given);
        if(checkWin(grid,solution)){
            cout<<"★ Congratulations! Puzzle solved! Mistakes: "<<mistakes<<"\n";
            break;
        }
        cout<<"  Move (row col val) > ";
        int r,c,v; cin>>r>>c>>v;
        if(r==0&&c==0&&v==0)break;
        if(r==9&&c==9&&v==0){grid=solution;printGrid(grid,given);cout<<"Auto-solved!\n";break;}
        r--;c--;
        if(r<0||r>=9||c<0||c>=9){cout<<"Invalid coordinates.\n";continue;}
        if(given[idx(r,c)]!=0){cout<<"That cell is fixed.\n";continue;}
        if(v<0||v>9){cout<<"Value must be 1-9 (0 to erase).\n";continue;}
        grid[idx(r,c)]=v;
        if(v!=0&&v!=solution[idx(r,c)]){
            mistakes++;
            cout<<"  ✗ Incorrect! (mistakes: "<<mistakes<<")\n";
        }else if(v!=0){
            cout<<"  ✓ Correct!\n";
        }
    }
    return 0;
}
