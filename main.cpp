/*开发日志：
* 项目：黑白棋
* 时间：12.4-12.19
* 小组名：贝克街221B
* 小组人员：龙奕涵、李婉宁
* 特色：精美用心的GUI和动画
*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<graphics.h>//使用了easyX目前最新版本的库
#define maxsize   8

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")//music

const int MAPPOINTCOUNT[8][8] =                                             //棋盘各点权值估值
{
   {90,-60,10,10,10,10,-60,90},
    {-60,-80,5,5,5,5,-80,-60},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {-60,-80,5,5,5,5,-80,-60},
    {90,-60,10,10,10,10,-60,90}
};
//函数声明


bool judge_bound(int x, int y);//判断是否出界
int judge_search(int color, int board[][maxsize], int arrput[][maxsize]);//判断是否能下
void change(int board[][maxsize], int i, int j, int thiscolor);//翻转棋子
int calScore(int board[][maxsize], int color);//计算分数
int minmax(int x, int y, int boardnow[][maxsize], int arrputnow[][maxsize], int depin, int depinmax, int color);
void aiplay(int color);//ai下棋
bool load(int board[][maxsize]);//读档
bool save(int board[][maxsize]);//存档
void initialize(int board[][maxsize]);//棋盘初始化
int judge(int x, int y, int color, int board[][maxsize]);

void countScore(int a);
void choice();
void textPoint();
void menu(int a);

#define WIN_WIDTH  643
#define WIN_HIGH   800//宏定义便于之后修改
using namespace std;
const int black =2,white=  1;
int chessPixel[maxsize+1][maxsize+1][2];//像素位置记录点，在棋盘显示的函数里头得到具体数值
int board[maxsize][maxsize] = {};
int arrput[maxsize][maxsize]{};
int expect[maxsize][maxsize]{};
int flg[maxsize][maxsize] = { 0 };
static int color = black,AIcolor = white;
int plPoint = 0, AIPoint = 0;
bool re = 0;//是否重来

IMAGE cBg;//棋盘背景
IMAGE Black;
IMAGE White;
IMAGE MenuB,MenuS;
IMAGE bttnMenu;
IMAGE avs[12]; 
IMAGE bttn5, bttn51, bttn4, bttn41;//startUI
IMAGE menuP, bttn1, bttn11, bttn2, bttn21, bttn3, bttn31, bttnR;//menu
void gameBg() {//加载棋盘，需要修改项目属性字符集为多字节字符集使图片导入可行
	loadimage(&cBg, "res/bg.jpg");
	loadimage(&Black, "res/black.jpg");
	loadimage(&White, "res/white.jpg");
	loadimage(&MenuB, "res/menuBg.jpg");
    loadimage(&bttnMenu, "res/bttn/buttonMenu.jpg");//
    loadimage(&MenuS, "res/menuStart.jpg");
    loadimage(&bttn5, "res/bttn/button1.jpg");
    loadimage(&bttn51, "res/bttn/button1a.jpg");
    loadimage(&bttn4, "res/bttn/button2.jpg");
    loadimage(&bttn41, "res/bttn/button2a.jpg");//startUI
    loadimage(&bttn1, "res/bttn/button3.jpg");
    loadimage(&bttn11, "res/bttn/button3a.png");
    loadimage(&bttn2, "res/bttn/button4.jpg");
    loadimage(&bttn21, "res/bttn/button4a.png");
    loadimage(&bttn3, "res/bttn/button5.png");
    loadimage(&bttn31, "res/bttn/button5a.png");
    loadimage(&menuP, "res/menuBg.jpg");
    loadimage(&bttnR, "res/bttn/buttonBack.jpg");//menu
    char name[64];
    for (int i = 1; i < 12; i++) {
        sprintf_s(name, sizeof(name), "res/aversi%d.jpg", i);
        loadimage(&avs[i], name);
    }
    int x = 0, y = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            x = 100 + j * 56; y = 220 + i * 56;

            if (j > 3) {
                x = 323 + (j - 4) * 56;
            }
            if (i > 3) {
                y = 442 + (i - 4) * 56;
            }
            chessPixel[i][j][0] = x;
            chessPixel[i][j][1] = y;
           
        }
}
//图片加载，并非显示
void startUI() {
    putimage(0, 0, &MenuS);
    putimage(190, 236, &bttn5);
    putimage(190, 440, &bttn4);
    bool flag = 0;
    while (1) {
        BeginBatchDraw();
        ExMessage msg;
        if (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x > 190 && msg.x < 469 && (msg.y > 236 && msg.y < 353 || msg.y>440 && msg.y < 557))
                    flag = 1;
            }
            else if (msg.message == WM_LBUTTONUP && flag) {
                if (msg.x > 190 && msg.x < 469 && msg.y>236 && msg.y < 353)//Start
                {
                    putimage(190, 236, &bttn51);
                    EndBatchDraw();
                    choice();
                    Sleep(450);
                    return;
                }//start
                else if (msg.x > 190 && msg.x < 469 && msg.y>440 && msg.y < 557)//Load
                {
                    putimage(190, 440, &bttn41);
                    EndBatchDraw();
                    load(board);
                    Sleep(450);
                    return;
                }//load
            }
        }
        EndBatchDraw();
    }
}
void countScore(int a) {
    plPoint=0, AIPoint=0;
    for(int i=0;i<maxsize;i++)
        for (int j = 0; j < maxsize; j++) {
            if (board[i][j] == color)plPoint++;
            else if (board[i][j] == AIcolor)AIPoint++;
        } 
    if (a == 2) {
            if (plPoint > AIPoint) {
                menu(1);
            }
            else if (plPoint < AIPoint) {
                menu(2);
            }
            else if (plPoint == AIPoint) {
                menu(0);
            }
    }
}
void upWindow() {
	BeginBatchDraw();//开始缓冲
	putimage(0, 0, &cBg);
    putimage(10, 704, &bttnMenu);
	int x, y;
	for(int i=0;i<8;i++)
		for (int j = 0; j < 8; j++) {
			x = 100+j*56; y = 220+i*56;
			
			if (j > 3) { 
				x = 323 + (j-4)* 56; 
			}
			if (i > 3) {
				y = 442 + (i - 4) * 56;
			}//循环导致部分像素点偏离了，所以手调了中间的像素位置
			chessPixel[i][j][0] = x;
			chessPixel[i][j][1] = y;
            if (board[i][j] == black)putimage(x, y, &Black);
			else if (board[i][j] == white)putimage(x, y, &White);
		}
	EndBatchDraw();//结束双缓冲
}
void choice() {
    IMAGE bttn1, bttn2;
    loadimage(&bttn1, "res/bttn1.png");
    loadimage(&bttn2, "res/bttn2.jpg");
    putimage(0, 0, &MenuS);
    putimage(190, 236, &bttn1);
    putimage(190, 440, &bttn2);
    initialize(board);
    board[3][3] = white;
    board[4][3] = black;
    board[3][4] = black;
    board[4][4] = white;
    bool flag = 0;
    while (1) {
        ExMessage msg;
        if (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x > 190 && msg.x < 469 && (msg.y > 236 && msg.y < 353 || msg.y>440 && msg.y < 557))
                    flag = 1;
            }
        else if (msg.message == WM_LBUTTONUP && flag) {
             if (msg.x > 190 && msg.x < 469 && msg.y>236 && msg.y < 353)//First
                {
                 color = black;
                 AIcolor = white;
                    Sleep(20);
                    return;
                }
                else if (msg.x > 190 && msg.x < 469 && msg.y>440 && msg.y < 557)//Last
                {
                    color = white;
                    AIcolor = black;
                    if (judge_search(black, board, arrput) > 0)
                    {
                        upWindow();
                        aiplay(AIcolor);
                        upWindow();
                    }
                  countScore(0);
                    Sleep(20);
                    return;
                }
            }
        }
    }
}
void menu(int a) {
    putimage(0, 0, &menuP);  
    putimage(194, 244, &bttn1);
    putimage(194, 396, &bttn2);
    putimage(194, 544, &bttn3);
    putimage(540, 664, &bttnR);
    if (a != 3) {
        IMAGE w, l, p;
        loadimage(&w, "res/winM.jpg");
        loadimage(&l, "res/loseM.jpg");
        loadimage(&p, "res/p.jpg");
        mciSendString("stop bgMci", 0, 0, 0);
        mciSendString("close bgMci", 0, 0, 0);
        if (a == 1) {
            putimage(0, 0, &w);
            mciSendString("open res/winMusic.mp3 alias wMci", 0, 0, 0);
            mciSendString("play wMci repeat", 0, 0, 0);
        }
        else if (a == 2)
        {
            putimage(0, 0, &l);
            mciSendString("open res/loseMusic.mp3 alias lMci", 0, 0, 0);
            mciSendString("play lMci repeat", 0, 0, 0);
        }
        else if (a == 0)
            putimage(0, 0, &p);
        putimage(194, 396, &bttn21);
    }
    int flag = 0;
    while (1) {
        BeginBatchDraw();
        ExMessage msg;
        if (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x > 194 && msg.x < 452&&
                    (msg.y > 396 && msg.y < 504|| msg.y > 244 && msg.y < 352|| msg.y > 544 && msg.y < 652)
                    ||msg.x>540&&msg.x<628&&msg.y>664&&msg.y<664+88)
                       flag = 1;
            }
            else if (msg.message == WM_LBUTTONUP && flag) { 
                if (a == 1) {
                    mciSendString("stop wMci", 0, 0, 0);
                    mciSendString("close wMci", 0, 0, 0);
                }
                else if (a == 2) {
                    mciSendString("stop lMci", 0, 0, 0);
                    mciSendString("close lMci", 0, 0, 0);
                }
                    if (msg.y > 244 && msg.y < 352) {
                        putimage(194, 244, &bttn11); 
                        EndBatchDraw();
                        startUI();
                        Sleep(450);
                        re = 1;
                        mciSendString("open res/bgmusic.mp3 alias bgMci", 0, 0, 0);
                        mciSendString("play bgMci repeat", 0, 0, 0);
                        return;
                    }//restart
                    if (msg.y > 396 && msg.y < 504&&a==3) {
                        putimage(194, 396, &bttn21);
                        EndBatchDraw();
                        if (save(board)) {
                            startUI();
                            re = 1;
                        }
                        return;
                    }//save in the same time restart
                    if (msg.y > 544 && msg.y < 652) {
                        putimage(194, 544, &bttn31);
                        EndBatchDraw();
                        Sleep(450);
                        initialize(board);
                        closegraph();
                        mciSendString("stop bgMci", 0, 0, 0);
                        mciSendString("close bgMci", 0, 0, 0);
                        return;
                    }//exit
                    else if (msg.x > 540 && msg.x < 628 && msg.y>664 && msg.y < 664 + 88) {
                        EndBatchDraw();
                        Sleep(250);
                        upWindow();
                        return;
                    }
                }
            }
        EndBatchDraw();
    }
}
void reversiChess(int x1,int y1,int color) {
    int i,jud=0;
    int x = chessPixel[x1][y1][0];
    int y = chessPixel[x1][y1][1];
    if (color == black) { 
        i = 11; 
        jud = 1;
        while (i < 12 && i>0) {
            Sleep(18);
            putimage(x, y, &avs[i]);
          i--;
           
        }
    }
    else { 
        i = 1;
        while (i < 12 && i>0) {
            Sleep(20);
        putimage(x, y, &avs[i]);
        i++;
        
        }
    }
    if (jud) putimage(x, y, &Black);
    else putimage(x, y, &White);
    return;
}
void playerClickChess(int color) {
    BeginBatchDraw();
    textPoint();
    IMAGE hint;
    loadimage(&hint, "res/hint.jpg");
    if (re)judge_search(color, board, arrput);
    for (int i = 0; i < maxsize; i++)
        for (int j = 0; j < maxsize; j++)
            if (arrput[i][j]&&!board[i][j]) {
                putimage(chessPixel[i][j][0], chessPixel[i][j][1], &hint);
            }
    EndBatchDraw();
	ExMessage msg;
    while (1) {
        if (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.y > 710 && msg.y < 780 && msg.x > 17 && msg.x < 90) {
                    menu(3);
                    return;
                }
                for (int i = 0; i < maxsize; i++)
                    for (int j = 0; j < maxsize; j++) {
                        if (msg.x >= chessPixel[i][j][0] && msg.x <= chessPixel[i][j + 1][0]
                            && msg.y >= chessPixel[i][j][1] && msg.y <= chessPixel[i + 1][j][1])
                        {
                            if (arrput[i][j]&&!board[i][j]) {
                                if (color== black)putimage(chessPixel[i][j][0], chessPixel[i][j][1], &Black);
                                else putimage(chessPixel[i][j][0], chessPixel[i][j][1], &White);
                                change(board, i, j, color);
                                for (int i = 0; i < maxsize; i++) {
                                    for (int j = 0; j < maxsize; j++) {
                                        if (flg[i][j]) {
                                            reversiChess(i,j,color);
                                        }
                                    }
                                }
                                countScore(0);
                            upWindow();
                            Sleep(50);
                            return;
                            }
                            
                        }
                    }
            }
        }
    }
}
void pass(int n) {
     HWND wnd = GetHWnd();										//获取窗口句柄
     MessageBox(wnd, n == color? "可惜！你无处可下！" : "对方竟然无子可下！", "回合跳过", MB_OK);
     return;
}
void textPoint() {
    countScore(0);
    char scoreText[8];
    if (color == black) {
        sprintf_s(scoreText, sizeof(scoreText), "%d", plPoint);
    }
    else sprintf_s(scoreText, sizeof(scoreText), "%d", AIPoint);
    outtextxy(196, 160, scoreText);
    if (color == white) {
        sprintf_s(scoreText, sizeof(scoreText), "%d", plPoint);
    }
    else sprintf_s(scoreText, sizeof(scoreText), "%d", AIPoint);
    outtextxy(422, 160, scoreText);
}

int main() {
	gameBg();//棋盘棋子图片的加载
    initgraph(WIN_WIDTH, WIN_HIGH);//游戏图形界面
    /*用于音乐的：music*/
    mciSendString("open res/bgmusic.mp3 alias bgMci", 0, 0, 0);
    mciSendString("play bgMci repeat", 0, 0, 0);

    startUI();
    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 31;
    f.lfWeight = 15;
    _tcscpy_s(f.lfFaceName, _T("宋体"));
    f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿效果
    settextstyle(&f);
    setbkmode(TRANSPARENT);
    setcolor(BLACK);
	while (1) {
        if (plPoint + AIPoint == 64) { 
            countScore(2);
        }
        int flag=0;//判断胜负
		upWindow(); 
        textPoint();
        re = 0;
        if (judge_search(color, board, arrput))
            playerClickChess(color);//click->reversi->upWindow
        else {
            flag++;
            pass(color);
        }
        if (re) {
            continue;
        }
        if (judge_search(AIcolor, board, arrput)) {
            aiplay(AIcolor);
            Sleep(20);
            countScore(flag);
            upWindow();
        }
        else {
            flag++;
            if (flag == 2) {
                countScore(2);
                continue;
            }
            pass(AIcolor);
        }
	}
	system("pause");
	return 0;
}
bool judge_bound(int x, int y) {
    if (x >= 0 && x < maxsize && y >= 0 && y < maxsize)
        return true;
    else return false;
}
int judge_search(int color, int board[][maxsize], int arrput[][maxsize]) {
    int step = 0;
    int player = 0, foe = 0;
    if (color == black) {
        player = black;
        foe = white;
    }
    else {
        player = white;
        foe = black;
    }
    initialize(arrput);//初始化
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            if (board[i][j] != 0) {
                continue;
            }
            for (int id = -1; id <= 1; id++) {
                for (int jd = -1; jd <= 1; jd++) {
                    if (judge_bound(i + id, j + jd) == false || (id == 0 && jd == 0))
                        continue;
                    if (board[i + id][j + jd] == foe) {
                        int x = i + id;
                        int y = j + jd;
                        while (1) {
                            x += id;
                            y += jd;
                            if (judge_bound(x, y) == false)
                                break;
                            if (board[x][y] == 0)
                                break;
                            if (board[x][y] == player) {
                                arrput[i][j] = 1;
                                step++;
                                break;
                            }
                        }
                    }

                }
            }
        }
    }
    return step;
}
int judge(int x, int y, int color, int board[][maxsize]) {
    if (board[x][y] != 0)return 0;
    int player = 0, foe = 0;
    if (color == black) {
        player = black;
        foe = white;
    }
    else {
        player = white;
        foe = black;
    }
    int count = 0, flag;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if ((i == 0 && j == 0) || judge_bound(x + i, y + j) == false)continue;
            flag = 0;
            int xx = x + i;
            int yy = y + j;
            while (judge_bound(xx, yy) == true && board[xx][yy] != 0) {
                if (board[xx][yy] == foe)flag++;
                else {
                    count += flag;
                    break;
                }
                xx += i;
                yy += j;
            }
        }
    }return count;
}
void change(int board[][maxsize], int i, int j, int thiscolor) {
    if (thiscolor == color)initialize(flg);
    int foe = 0, player = 0;
    if (thiscolor == black) {
        foe = white;
        player = black;
    }
    else {
        player = white;
        foe = black;
    }
    board[i][j] = player;
    initialize(flg);
    for (int id = -1; id <= 1; id++) {
        for (int jd = -1; jd <= 1; jd++) {
            if (judge_bound(i + id, j + jd) == false || (id == 0 && jd == 0))
                continue;
            if (board[i + id][j + jd] == foe) {
                int x = i + id;
                int y = j + jd;
                while (1) {
                    x += id;
                    y += jd;
                    if (!judge_bound(x, y))
                        break;
                    if (board[x][y] == 0)
                        break;
                    if (board[x][y] == player) {
                        while (board[x -= id][y -= jd] == foe) {
                            flg[x][y] = 1;
                            board[x][y] = player;
                        }
                        break;
                    }
                }
            }
        }
    }

}
int calScore(int board[][maxsize], int color) {
    int foe = 0, player = 0;
    int score = 0;
    if (color == black) {
        foe = white;
        player = black;
    }
    else {
        player = white;
        foe = black;
    }
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            score -= (board[i][j] == foe);
            score += (board[i][j] == player);
        }
    }
    return score;
}
void copymap(int board1[][maxsize], int board2[][maxsize]) {
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            board1[i][j] = board2[i][j];
        }
    }
}
int minmax(int x, int y, int boardnow[][maxsize], int expectnow[][maxsize], int depin, int depinmax, int color) {
    if (depin >= depinmax)return 0;
    int player = 0, foe = 0;
    if (color == black) {
        player = black;
        foe = white;
    }
    else {
        player = white;
        foe = black;
    }
    int maxx = -10005;
    int expectnow2[maxsize][maxsize] = { 0 }, boardnow2[maxsize][maxsize] = { 0 }, expectlast[maxsize][maxsize] = { 0 }, boardnext[maxsize][maxsize] = { 0 };
    copymap(boardnow2, boardnow);
    int me = MAPPOINTCOUNT[x][y] + expectnow[x][y];
    change(boardnow2, x, y, player);
    int max_expect = 0, line_expect = 0, count = 0;
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            expectnow2[i][j] = judge(i, j, foe, boardnow2);
            if (expectnow2[i][j] > 0) {
                ++max_expect;
                if ((i == 0 && j == 0) || (i == 0 && j == maxsize - 1) || (i == maxsize - 1 && j == maxsize - 1) || (i == maxsize - 1 && j == maxsize - 1))return -1800;
                if ((i < 2 && j < 2) || (i < 2 && maxsize - j - 1 < 2) || (maxsize - 1 - i < 2 && j < 2) || (maxsize - 1 - i < 2 && maxsize - 1 - j < 2))++line_expect;

            }
        }
    }
    if (line_expect * 10 > max_expect * 7)return 1400;
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            if (expectnow2[i][j] > 0) {
                int you = MAPPOINTCOUNT[i][j] + expectnow2[i][j];
                copymap(boardnext, boardnow2);
                change(boardnext, i, j, foe);//都下完一轮了
                //  int arrput2[maxsize][maxsize]={0};
                for (int k = 0; k < maxsize; k++) {
                    for (int l = 0; l < maxsize; l++) {
                        expectlast[k][l] = judge(k, l, player, boardnext);
                    }
                }
                for (int k = 0; k < maxsize; k++) {
                    for (int l = 0; l < maxsize; l++) {
                        int nowm = me - you + minmax(k, l, boardnext, expectlast, depin + 1, depinmax, color);
                        maxx = maxx < nowm ? nowm : maxx;
                    }
                }
            }
        }
    }return maxx;
}
void aiplay(int color) {
    int player = 0, foe = 0;
    if (color == black) {
        player = black;
        foe = white;
    }
    else {
        player = white;
        foe = black;
    }
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            expect[i][j] = 0;//归零
        }
    }
    int maxx = -10005;
    int x = 0, y = 0;
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            expect[i][j] = judge(i, j, player, board);
            if (expect[i][j]) {
                if ((i == 0 && j == 0) || (i == 0 && j == maxsize - 1) || (i == maxsize - 1 && j == maxsize - 1) || (i == maxsize - 1 && j == 0))
                {
                    x = i; y = j;
                    if (player == black)putimage(chessPixel[x][y][0], chessPixel[x][y][1], &Black);
                    else putimage(chessPixel[x][y][0], chessPixel[x][y][1], &White);
                    change(board, x, y, player); //如果在角，返回角坐标
                    for (int i = 0; i < maxsize; i++) {
                        for (int j = 0; j < maxsize; j++) {
                            if (flg[i][j]) {
                                reversiChess(i, j, color);
                            }
                        }
                    }
                    return;//maybe
                }
                int k = minmax(i, j, board, expect, 0, 2, player);
                if (k >= maxx) {
                    maxx = k;
                    x = i; y = j;
                }
            }
        }
    }
    if (player == black)putimage(chessPixel[x][y][0], chessPixel[x][y][1], &Black);
    else putimage(chessPixel[x][y][0], chessPixel[x][y][1], &White);
    change(board, x, y, player);
    for (int i = 0; i < maxsize; i++) {
        for (int j = 0; j < maxsize; j++) {
            if (flg[i][j]) {
                reversiChess(i, j, color);
            }
        }
    }
}
bool load(int board[][maxsize]) {
    HWND wnd = GetHWnd();
    ifstream fin("reversi.txt");
    int a = 0;
    fin >> a;
    color = a;
    if (color == black)AIcolor = white;
    else AIcolor = black;
    if (!a) {
        MessageBox(wnd, "暂无存档", "Load", MB_OK);
        fin.close();
        return false;
    }
    else {
        for (int i = 0; i < maxsize; i++) {
            for (int j = 0; j < maxsize; j++) {
                fin >> board[i][j];
            }
        }
        MessageBox(wnd, "读档成功!", "Load", MB_OK);
        fin.close();
        return true;
    }
}
bool save(int board[][maxsize]) {
    HWND wnd = GetHWnd();	//获取窗口句柄
    ofstream fout("reversi.txt");
    if (!fout) {					
        MessageBox(wnd, "存档失败!", "Save", MB_OK);
        return false;
    }
    else {
       fout << color << endl;
       for (int i = 0; i < maxsize; i++) {
       for (int j = 0; j < maxsize; j++) {
           fout << board[i][j] << " ";
                }
       fout << endl;
            }
       MessageBox(wnd, "存档成功!\n已覆盖之前存档", "Save", MB_OK);
       return true;
        }
}
void initialize(int board[][maxsize]) {
    for (int row = 0; row < maxsize; row++)//设置棋盘的初始值
    {
        for (int col = 0; col < maxsize; col++)
        {
            board[row][col] = 0;
        }
    }
}
