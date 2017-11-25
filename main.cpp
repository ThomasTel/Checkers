#include <bits/stdc++.h>

using namespace std;

const int N = 8;
const int TAKE_TYPE = 3;
const int N_DIRS = 5;
const int delta[][2] = {{-1,-1}, {-1,0}, {-1,1}, {-2,-2}, {-2,2}};

struct Move
{
    int lin;
    int col;
    int dir;
    int coeff;
    
    Move() {}
    Move(int p_lin, int p_col, int p_dir, int p_coeff) :
    lin(p_lin), col(p_col), dir(p_dir), coeff(p_coeff) {}
    
};

ostream& operator<<(ostream& os, const Move& move)  
{  
    os << move.lin << " " << move.col << " " << move.dir << " " << move.coeff << endl;
    return os;  
}
    
int t[N][N]; //white > 0, black < 0, empty = 0 ; pawn = 1, queen = 2

inline bool isIn(int lin, int col)
{
    return 0 <= lin && lin < N && 0 <= col && col < N;
}

bool playable(Move m)
{
    m.coeff *= t[m.lin][m.col] < 0 ? -1 : 1;
    int lin = m.lin + delta[m.dir][0]*m.coeff;
    int col = m.col + delta[m.dir][1]*m.coeff;
    if(!isIn(lin, col))
        return false;
    if(t[lin][col] != 0)
        return false;
    if(m.dir >= TAKE_TYPE)
    {
        lin -= delta[m.dir][0]/2*m.coeff;
        col -= delta[m.dir][1]/2*m.coeff;
        return t[lin][col] * t[m.lin][m.col] < 0;
    }
    return true;
}

vector<Move> getMoves(int p)
{
    vector<Move> moves;
    for(int lin=0; lin<N; lin++)
        for(int col=0; col<N; col++)
            if(t[lin][col]*p > 0)
                for(int coeff=1; coeff>=-1; coeff-=4-abs(t[lin][col])) //queen: 1, -1
                    for(int dir=0; dir<N_DIRS; dir++)
                    {
                        Move move(lin, col, dir, coeff);
                        if(playable(move))
                            moves.push_back(move);
                    }
    return moves;
}

void play(Move& m, bool takeback=false)
{
    
}

int f(int p)
{
    
}

int main()
{
    ios_base::sync_with_stdio(false);
    string p; int n;
    cin >> p >> n;
    string s[n];
    for(int i=0; i<n; i++)
        cin >> s[i];
    string vals = "Bb_wW";
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            for(int k=0; k<vals.size(); k++)
                if(s[i][j] == vals[k])
                    t[i][j] = k-2;
    for(int i=0; i<n; i++)
        cout << s[i] << endl;
    for(Move move : getMoves(p[0] == 'w' ? 1 : -1))
    {
       cout << move;
    }
    return 0;
}