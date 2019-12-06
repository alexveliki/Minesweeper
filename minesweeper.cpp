#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<queue>
#include<windows.h>
using namespace std;
int field[32][64],highScoreType;
int first_move_flag;
char visible_field[32][64];
struct st
{
    int x,y;
};
struct st2
{
    double Time;
    char name[256];
};
bool cmp(st2 x,st2 y)
{
    return x.Time<y.Time;
}
st dir[]={{0,-1},{0,1},{-1,0},{1,0},{-1,-1},{-1,1},{1,-1},{1,1}};

clock_t Begin,time_since_last_click;

bool check_save_exists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}
void get_field_from_save(int &n,int &m,int &remaining_mines,int &remaining_tiles,double &Time)
{
    highScoreType=-1;

    ifstream fin("AutoSave.txt");

    fin>>n>>m>>remaining_mines>>remaining_tiles>>Time;

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            fin>>visible_field[i][j];
        }
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            fin>>field[i][j];
        }
    }


    fin.close();
}

void save_field(int n,int m,int remaining_mines,int remaining_tiles,double Time)
{
    ofstream fout("AutoSave.txt");
    fout<<n<<" "<<m<<" "<<remaining_mines<<" "<<remaining_tiles<<" "<<Time<<'\n';
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            fout<<visible_field[i][j]<<" ";
        }
        fout<<'\n';
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            fout<<field[i][j]<<" ";
        }
        fout<<'\n';
    }
    fout.close();
}
void markField(int n,int m)
{
    int cnt;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(field[i][j]!=-1)
            {
                field[i][j]=0;
                cnt=0;
                for(int k=0;k<8;k++)
                {
                    if(field[i+dir[k].x][j+dir[k].y]==-1)cnt++;
                }
                field[i][j]=cnt;
            }
        }
    }
}
void generate_new_field(int n,int m,int mines)
{
    srand(time(0));
    first_move_flag=1;
    memset(field,0,sizeof(field));
    for(int i=0;i<=n+1;i++)
    {
        field[i][0]=-2;
        field[i][m+1]=-2;
    }
    for(int i=0;i<=m+1;i++)
    {
        field[0][i]=-2;
        field[n+1][i]=-2;
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            visible_field[i][j]='@';
        }
    }

    vector<st>v;
    st x;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            x.x=i;
            x.y=j;
            v.push_back(x);
        }
    }
    int r;
    for(int i=1;i<=mines;i++)
    {

        r=rand()%v.size();
        field[v[r].x][v[r].y]=-1;
        v.erase(v.begin()+r);
    }
    markField(n, m);
    save_field(n,m,mines,n*m-mines,0);
}
void choose_new_field(int &n,int &m,int &remaining_mines,int &remaining_tiles,double &Time)
{
    Time=0.0;
    bool fl;
    string difficulty;
    cout<<"Choose difficulty (beginner/intermediate/expert/custom)\n";
    do
    {
        fl=0;
        cin>>difficulty;
        if(difficulty=="beginner"){generate_new_field(8,8,10);n=8;m=8;remaining_mines=10;remaining_tiles=8*8-10;highScoreType=1;}
        else if(difficulty=="intermediate"){generate_new_field(16,16,40);n=16;m=16;remaining_mines=40;remaining_tiles=16*16-40;highScoreType=2;}
        else if(difficulty=="expert"){generate_new_field(16,30,99);n=16;m=30;remaining_mines=99;remaining_tiles=16*30-99;highScoreType=3;}
        else if(difficulty=="custom")
        {
            highScoreType=4;
            int x,y,z;
            cout<<"Enter size of the field and number of mines: ";

            do
            {
                cin>>x>>y>>z;
            }while((x-1)*(y-1)<z||(x<1||y<1||x>20||y>50));
            n=x;
            m=y;
            remaining_mines=z;
            remaining_tiles=n*m-remaining_mines;
            generate_new_field(x,y,z);
        }
        else fl=1;
    }while(fl==1);
}
void board_ini(int &n,int &m,int &remaining_mines,int &remaining_tiles,double &Time)
{
    if(check_save_exists("AutoSave.txt"))
    {
        cout<<"Do you want to continue your previous game? (yes/no)\n";
        string continue_from_save;
        do
        {
            cin>>continue_from_save;
            if(continue_from_save=="yes")
            {
                get_field_from_save(n,m,remaining_mines,remaining_tiles,Time);
            }
            else if(continue_from_save=="no")
            {
                choose_new_field(n,m,remaining_mines,remaining_tiles,Time);
            }
        }while(continue_from_save!="yes"&&continue_from_save!="no");
    }
    else
    {
        cout<<"No save found.\n\n";
        choose_new_field(n,m,remaining_mines,remaining_tiles,Time);
    }
}
void print_visible_field(int n,int m,int remaining_mines,double Time)
{
    printf("Remaining mines: %d        Time: %.2f\n",remaining_mines,Time);
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(visible_field[i][j]=='*')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
            else if(visible_field[i][j]=='0')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0);
            else if(visible_field[i][j]=='1')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
            else if(visible_field[i][j]=='2')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            else if(visible_field[i][j]=='3')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
            else if(visible_field[i][j]=='4')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
            else if(visible_field[i][j]=='5')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
            else if(visible_field[i][j]=='6')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
            else if(visible_field[i][j]=='7')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
            else if(visible_field[i][j]=='8')SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
            printf("%c ",visible_field[i][j]);
        }
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
}
void GameOver()
{
    cout << '\a';
    cout<<"You hit a mine.\n\n\n";
    cout<<"   GAME OVER\n\n\n";
    remove("AutoSave.txt");
    exit(0);
}
void moveMine(int n,int m,int x,int y)
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(field[i][j]!=-1&&(!(i==x&&j==y)))
            {
                field[i][j]=-1;
                field[x][y]=0;
                return;
            }
        }
    }
}
void MakeMove_R(int x,int y,int &remaining_mines)
{
    if(visible_field[x][y]=='@')
    {
        visible_field[x][y]='*';
        remaining_mines--;
    }
    else if(visible_field[x][y]=='*')
    {
        visible_field[x][y]='@';
        remaining_mines++;
    }
}
void MakeMove_L(int n,int m,int x,int y,int &remaining_tiles)
{
    if(visible_field[x][y]!='@')return;
    if(field[x][y]==-1)
    {
        if(first_move_flag)
        {
            moveMine(n,m,x,y);
            markField(n,m);
        }
        else GameOver();
    }
    queue<st>q;
    st s,ss;
    s.x=x;
    s.y=y;
    q.push(s);
    while(!q.empty())
    {
        s=q.front();
        q.pop();
        visible_field[s.x][s.y]=field[s.x][s.y]+'0';
        remaining_tiles--;
        if(field[s.x][s.y]==0)
        for(int i=0;i<8;i++)
        {
            if(field[s.x+dir[i].x][s.y+dir[i].y]>=0&&visible_field[s.x+dir[i].x][s.y+dir[i].y]=='@')
            {
                ss.x=s.x+dir[i].x;
                ss.y=s.y+dir[i].y;
                visible_field[ss.x][ss.y]=field[ss.x][ss.y]+'0';
                q.push(ss);
            }
        }
    }
}
void MakeMove(int n,int m,string type,int x,int y,int &remaining_mines,int &remaining_tiles)
{
    if(type=="R")
    {
        MakeMove_R(x,y,remaining_mines);
    }
    if(type=="L")
    {
        MakeMove_L(n,m,x,y,remaining_tiles);
    }
    if(type=="M")
    {
        if(visible_field[x][y]=='*')return;
        int cnt=0;
        for(int i=0;i<8;i++)
        {
            if(visible_field[x+dir[i].x][y+dir[i].y]=='*')cnt++;
        }
        if(cnt==field[x][y])
        {
            for(int i=0;i<8;i++)
            {
                MakeMove_L(n,m,x+dir[i].x,y+dir[i].y,remaining_tiles);
            }
        }
    }
}

void playerWon(int n,int m,double Time)
{
    system("cls");
    print_visible_field(n,m,0,Time);
    cout<<"\n\n\n\n";
    cout<<"YOU WON!";
    cout<<"\n\n\n";
    char name[256];

    vector<st2>v;
    st2 q;

    string files[3]={"HighScoresBeginner.txt","HighScoresIntermediate.txt","HighScoresExpert.txt"};

    if(highScoreType>=1&&highScoreType<=3)
    {
        cout<<"Enter your name: ";
        cin.get();
        cin.getline(name,64);
        q.Time=Time;
        strcpy(q.name,name);
        v.push_back(q);
        ifstream fin(files[highScoreType-1]);
        while(fin>>q.Time)
        {
            fin.get();
            fin.getline(q.name,256);
            v.push_back(q);
        }
        fin.close();
        sort(v.begin(),v.end(),cmp);
        ofstream fout(files[highScoreType-1]);

        int szv=v.size();
        for(int i=0;i<szv;i++)
        {
            fout<<v[i].Time<<" "<<v[i].name<<'\n';
        }
        fout.close();
        cout<<"Time saved in high scores";
    }
    else cout<<"This game did not meet the requirements for valid high score.";
    remove("AutoSave.txt");
    exit(0);
}

bool checkMouseCoord(int n,int m,int x,int y)
{
    if(x>=1&&x<=n&&y>=1&&y<=m)return true;
    return false;
}

void revealTile(int n,int m,int &remaining_tiles)
{
    highScoreType=-1;
    vector<st>v;
    st q;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(visible_field[i][j]=='@'&&field[i][j]>=0)
            {
                q.x=i;
                q.y=j;
                v.push_back(q);
            }
        }
    }
    int sz=v.size();
    srand(time(0));
    int r=rand()%sz;
    MakeMove_L(n,m,v[r].x,v[r].y,remaining_tiles);

}

void MakeMoveWithMouse(int n,int m,int &remainning_mines,int &remaining_tiles)
{
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);

    DWORD dwPreviousMode = 0;
    GetConsoleMode(hConsole, &dwPreviousMode);
    DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;

    dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hConsole, dwNewMode | ENABLE_EXTENDED_FLAGS);

    while(1)
    {
        HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
        INPUT_RECORD InputRecord;
        DWORD Events;
        COORD coord;
        ReadConsoleInput(hin, &InputRecord, 1, &Events);

        if( InputRecord.EventType == MOUSE_EVENT)
        {
            if((double)(clock()-time_since_last_click)/CLOCKS_PER_SEC>=0.1)
            {
                if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    if(InputRecord.Event.MouseEvent.dwMousePosition.Y==0&&InputRecord.Event.MouseEvent.dwMousePosition.X==27)
                    {
                        revealTile(n,m,remaining_tiles);
                        return;
                    }

                    if(InputRecord.Event.MouseEvent.dwMousePosition.X%2==1)continue;

                    time_since_last_click=clock();

                    coord.X = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                    coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.X/2+1;
                    if(!checkMouseCoord(n,m,coord.X,coord.Y))continue;
                    if(visible_field[coord.X][coord.Y]=='@')
                    {
                        MakeMove(n,m,"L",coord.X,coord.Y,remainning_mines,remaining_tiles);
                        return;
                    }
                    else if(visible_field[coord.X][coord.Y]!='*')
                    {
                        MakeMove(n,m,"M",coord.X,coord.Y,remainning_mines,remaining_tiles);
                        return;
                    }
                }
                if(InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
                {
                    if(InputRecord.Event.MouseEvent.dwMousePosition.X%2==1)continue;

                    time_since_last_click=clock();
                    coord.X = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                    coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.X/2+1;
                    if(!checkMouseCoord(n,m,coord.X,coord.Y))continue;
                    if(visible_field[coord.X][coord.Y]=='@'||visible_field[coord.X][coord.Y]=='*')
                    {
                        MakeMove(n,m,"R",coord.X,coord.Y,remainning_mines,remaining_tiles);
                        return;
                    }
                }
            }
        }
   }
}



void clearscreen()
{
    COORD coordScreen = {0,0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

int main()
{
    int n,m,remaining_mines,remaining_tiles;
    double Time;
    board_ini(n,m,remaining_mines,remaining_tiles,Time);
    Begin=clock();
    time_since_last_click=clock();
    string type;
    int x,y,fl=0;
    system("cls");
    while(remaining_tiles>0)
    {
        clearscreen();
        clock_t curTime=clock();
        if(fl==0)
        {
            Begin=clock();
            print_visible_field(n,m,remaining_mines,Time + double(curTime-Begin)/CLOCKS_PER_SEC);
            MakeMoveWithMouse(n,m,remaining_mines,remaining_tiles);
            Begin=clock();
            fl=1;
            first_move_flag=0;
        }
        else
        {
            print_visible_field(n,m,remaining_mines,Time + double(curTime-Begin)/CLOCKS_PER_SEC);
            MakeMoveWithMouse(n,m,remaining_mines,remaining_tiles);
        }
        curTime=clock();
        save_field(n,m,remaining_mines,remaining_tiles,Time + double(curTime-Begin)/CLOCKS_PER_SEC);
    }
    clock_t endTime=clock();
    Time=Time+double(endTime-Begin)/CLOCKS_PER_SEC;
    playerWon(n,m,Time);
    return 0;
}
