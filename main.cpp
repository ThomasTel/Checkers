#include <bits/stdc++.h>

using namespace std;

const int N = 8;
const int TAKE_TYPE = 2;
const int delta[][2] = {{-1,-1}, {-1,1}, {-2,-2}, {-2,2}};
const string vals = "Bb_wW";
const int P_MAX = 9;
const int NOT_ZERO = 42;
const int INF = 1e9;
const pair<int, int> NO_MULTIPLE = {-1, -1};

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

void newPos(Move& m, int pre, int& lin, int& col)
{
    m.coeff *= pre < 0 ? -1 : 1;
    lin = m.lin + delta[m.dir][0]*m.coeff;
    col = m.col + delta[m.dir][1]*m.coeff;
}

bool playable(Move m)
{
    int lin, col; newPos(m, t[m.lin][m.col], lin, col);
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

vector<Move> getMoves(int p, const pair<int, int>& last)
{
    vector<Move> moves;
    for(int take=TAKE_TYPE; take>=0; take-=2)
    {
        for(int lin=0; lin<N; lin++)
            for(int col=0; col<N; col++)
            {
                if(last != NO_MULTIPLE && make_pair(lin, col) != last)
                    continue;
                if(t[lin][col]*p > 0)
                    for(int coeff=1; coeff>=-1; coeff-=4-abs(t[lin][col])) //queen: 1, -1
                        for(int dir=take; dir<take+2; dir++) //2,3 then 0,1
                        {
                            Move move(lin, col, dir, coeff);
                            if(playable(move))
                                moves.push_back(move);
                        }
            }
        if(moves.size())
            return moves;
        if(last != NO_MULTIPLE)
            return {};
    }
    return moves;
}

int play(Move m, int pre, int taken=0)
{
    int lin, col; newPos(m, pre, lin, col);
    t[lin][col] = taken ? 0 : pre;
    if(abs(pre) == 1 && (lin == 0 && t[lin][col] > 0 || lin == N-1 && t[lin][col] < 0))
        t[lin][col] *= 2;
    t[m.lin][m.col] = taken ? pre : 0;
    int r = NOT_ZERO;
    if(m.dir >= TAKE_TYPE)
    {
        lin -= delta[m.dir][0]/2*m.coeff;
        col -= delta[m.dir][1]/2*m.coeff;
        r = t[lin][col];
        t[lin][col] = taken;
    }
    return r;
}

inline int dist(pair<int, int>& a, pair<int, int>& b)
{
    return (a.first-b.first)*(a.first-b.first)
    + (a.second-b.second)*(a.second-b.second);
}

int eval(int p)
{
    const int X = 100;
    const int Y = 100000*X; // > 2*8*8 * 12*12 * X
    int cnt = 0;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            cnt += t[i][j];
    //the less pawns the better, the lest dist the better
    vector<pair<int, int>> c[2];
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            if(t[i][j])
                c[t[i][j] > 0].push_back({i, j});
    int distSum = 0;
    for(auto a : c[0])
        for(auto b : c[1])
            distSum += dist(a, b);
    return -X * distSum + Y * cnt / p;
}

int nbExplored;
pair<int, vector<Move>> f(int p, int pr, pair<int, int> multipleMoves=NO_MULTIPLE, int alpha=-INF, int beta=INF)
{
    nbExplored++;
    if(pr == P_MAX)
        return {eval(p), vector<Move>()};
    auto moves = getMoves(p, multipleMoves);
    if(multipleMoves != NO_MULTIPLE && moves.empty())
    {
        auto r = f(p*(-1), pr+1, NO_MULTIPLE);
        return {-r.first, vector<Move>()};
    }
    pair<int, vector<Move>> best = {-INF, vector<Move>()};
    for(auto move : moves)
    {
        int pre = t[move.lin][move.col];
        int taken = play(move, pre);
        int nextP = p, nextPr = pr;
        pair<int, int> nextMultipleMoves;
        Move move_ = move;
        newPos(move_, p, nextMultipleMoves.first, nextMultipleMoves.second);
        if(move.dir < TAKE_TYPE)
            nextP *= -1, nextPr++, nextMultipleMoves=NO_MULTIPLE;
        auto val = f(nextP, nextPr, nextMultipleMoves, -beta, -alpha);
        if(move.dir < TAKE_TYPE)
            val.first *= -1;
        if(val.first > best.first)
        {
            if(nextMultipleMoves == NO_MULTIPLE)
                val.second = {};
            val.second.insert(val.second.begin(), move);
            best = val;
        }
        play(move, pre, taken);
        alpha = max(alpha, val.first);
        if(alpha > beta)
            break;
    }
    return best;
}

void display()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
            cerr << vals[t[i][j]+2];
        cerr << endl;
    }
    cerr << endl;
}

int hashT()
{
    const int M = 222222227;
    int r = 0;
    for(int i=0, h=1; i<N; i++)
        for(int j=0; j<N; j++, h=(h*vals.size())%M)
            r = (r + (t[i][j]+2)*h) % M;
    return r;
}

int main()
{
    ios_base::sync_with_stdio(false);
    string x; int n;
    cin >> x >> n;
    string s[n];
    for(int i=0; i<n; i++)
        cin >> s[i];
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            for(int k=0; k<vals.size(); k++)
                if(s[i][j] == vals[k])
                    t[i][j] = k-2;
    int p = x[0] == 'w' ? 1 : -1;
    vector<Move> moves = f(p, 0).second;
    cout << moves.size() << endl;
    for(auto move : moves)
        cout << move.lin << " " << move.col << endl;
    int lin, col; newPos(moves.back(), p, lin, col);
    cout << lin << " " << col << endl;
    //cerr << nbExplored << endl;
    return 0;
}