// #include "winsock2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SQUARE 15

//プロトタイプ宣言
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);
void boardScoring();
int matchingScore(int *checkBoard, int whichPlayer)

//先行のプレイヤー(黒，1)のアルゴリズムの手をすべて羅列する(向きの逆も含む)
int firstPatternEx[35][8]={
	{0,1,1,1,1,1},
	{1,0,1,1,1,1,0},
	{2,0,1,1,1,2},
	{2,2,1,1,1,0},
	{2,0,1,0,1,1,1,0},
	{2,0,1,1,1,0,1,0},
	{2,0,1,1,0,1,1,0},
	{2,0,1,1,0,1,1,0},
	{3,0,1,1,1,0},
	{3,0,1,0,1,1,0},
	{3,0,1,1,0,1,0},
	{4,0,0,1,1,1,2},
	{4,2,1,1,1,0,0},
	{4,0,1,0,1,1,2},
	{4,2,1,1,0,1,0},
	{4,0,1,1,0,1,2},
	{4,2,1,0,1,1,0},
	{4,1,0,0,1,1},
	{4,1,1,0,0,1},
	{4,1,0,1,0,1},
	{4,2,0,1,1,1,0,2},
	{5,0,0,1,1,0},
	{5,0,1,1,0,0},
	{5,0,1,0,1,0},
	{5,0,1,0,0,1,0},
	{6,0,0,0,1,1,2},
	{6,2,1,1,0,0,0},
	{6,0,0,1,0,1,2},
	{6,2,1,0,1,0,0},
	{6,0,1,0,0,1,2},
	{6,2,1,0,0,1,0},
	{6,1,0,0,0,1},
	{6,2,0,1,0,1,0,2},
	{6,2,0,1,1,0,0,2},
	{6,2,0,0,1,1,0,2}
};
//後攻のプレイヤー(白，2)のアルゴリズムの手をすべて羅列する(向きの逆も含む)
int secondPatternEx[35][8]={
	{0,2,2,2,2,2},
	{1,0,2,2,2,2,0},
	{2,0,2,2,2,1},
	{2,1,2,2,2,0},
	{2,0,2,0,2,2,2,0},
	{2,0,2,2,2,0,2,0},
	{2,0,2,2,0,2,2,0},
	{2,0,2,2,0,2,2,0},
	{3,0,2,2,2,0},
	{3,0,2,0,2,2,0},
	{3,0,-2,2,0,2,0},
	{4,0,0,2,2,2,2},
	{4,1,2,2,2,0,0},
	{4,0,2,0,2,2,1},
	{4,1,2,2,0,2,0},
	{4,0,2,2,0,2,1},
	{4,1,2,0,2,2,0},
	{4,2,0,0,2,2},
	{4,2,2,0,0,2},
	{4,2,0,2,0,2},
	{4,1,0,2,2,2,0,1},
	{5,0,0,2,2,0},
	{5,0,2,2,0,0},
	{5,0,2,0,2,0},
	{5,0,2,0,0,2,0},
	{6,0,0,0,2,2,1},
	{6,1,2,2,0,0,0},
	{6,0,0,2,0,2,1},
	{6,1,2,0,2,0,0},
	{6,0,2,0,0,2,1},
	{6,1,2,0,0,2,0},
	{6,2,0,0,0,2},
	{6,1,0,2,0,2,0,1},
	{6,1,0,2,2,0,0,1},
	{6,1,0,0,2,2,0,1}
};
int patternScore[7]={100, 80, 70, 60, 50, 40, 30};

//盤面の情報を記録しておく配列
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
//最適な打ち手を探すために盤面のパターンによって行う点数付けを記録する盤面．自分と敵の2つのスコアボードを保持する．
//先行は0番目に，後攻は1番目に格納する
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};

//自分が先行か後攻かを記録する(先行(黒)→1, 後攻(白)→2)
int player_number=1;
int enemy_number=2;

//勝利フラッグ(自分が勝ったら1)
int isWin=0;

int main(void) {
    int i,j;
    //盤面を0,1,2でランダムに埋める
    for(i = 0; i < BOARD_SQUARE; i++ ){
        for(j=0;j<BOARD_SQUARE; j++){
            board[i][j]=rand() % 3;     //0,1,2 の乱数を発生
        }
    }
    
    i=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            
        if(board[i-1][j-1]==0) printf(" -");
        if(board[i-1][j-1]==1) printf(" o");
        if(board[i-1][j-1]==2) printf(" x");

        }
        puts("");
    }

    boardScoring();

    //点数付けした盤面を出力する(杉村)
    printf("SCOREBOARD(1stPlayer)");
    i=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            printf("%d", scoreBoard[i-1][j-1][0]);
        }
        puts("");
    }

    //点数付けした盤面を出力する(杉村)
    printf("SCOREBOARD(2ndPlayer)");
    i=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            printf("%d", scoreBoard[i-1][j-1][1]);
        }
        puts("");
    }
    
    while(1);

	return 0;

}

int isDelimiter(char p, char delim){
  return p == delim;
}

int split(char *dst[], char *src, char delim){
  int count = 0;

  for(;;) {
    while (isDelimiter(*src, delim)){
      src++;
    }

    if (*src == '\0') break;

    dst[count++] = src;

    while (*src && !isDelimiter(*src, delim)) {
      src++;
    }

    if (*src == '\0') break;

    *src++ = '\0';
  }
  return count;
}

//一回置き終わったら点数を消さないとあかんかも？（一個前の手を使うならいける)
//盤面を全面探索して，まだ石が置かれていない場所の点数付けをする(点数は加算していく)．その値は，3次元配列scoreBoardに記録する．
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[9];
	int cdIni[4][2]={
		{0, -4},	//縦
		{-4, 0},	//横
		{-4, -4},	//右下がり斜め
		{4, -4}		//右上がり斜め
	};
	int cdGap[4][2]={
		{0, 1},		//縦
		{1, 0},		//横
		{1, 1},		//右下がり斜め
		{-1, 1}		//右上がり斜め
	};
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sy][sx]==0){		//石がおかれていないとき、４方向を見る
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
					for(cb=0;cb<9;cb++){
						checkBoard[cb]=board[tempY][tempX];		//checkBoardに一方向の盤の情報を格納
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					//スコアを加算して仮の盤面に格納する
					scoreBoard[sy][sx][player_number-1]+=matchingScore(checkBoard, player_number);
					scoreBoard[sy][sx][enemy_number-1]+=boardScore(checkBoard, enemy_number);
				}
			}
		}
	}
}

//マッチングしたかどうか+マッチングしたパターンと対象配列の0が一致しているか+それにより点数を加点する
//場所依存やから，マッチングだけのメソッドにはできない
//1つの方向(要素数9の一次配列)に対して，35種類全てのパターンを探索する
int matchingScore(int *checkBoard, int whichPlayer){
	int pNum=lenPEX-1;
	int mNum=10-pNum;
	int mi, mj;
	int ms;
	int sumScore=0;
	if(whichPlayer==1){
		for(ms=0;ms<35;ms++){
			for(mi=0;mi<mNum;mi++){
				//見てるパターンの真ん中が0ではない→加算しないから飛ばす
				if(firstPatternEx[ms][5-mi]!=0){
					continue;
				}
				for(mj=0;mj<pNum;mj++){
					if(checkBoard[mi+mj]!=firstPatternEx[ms][mj+1]){
						continue;
					}
				}//最後までミスなし
				sumScore+=patternScore[firstPatternEx[ms][0]];
			}
		}
	}
	else{
		for(ms=0;ms<35;ms++){
			for(mi=0;mi<mNum;mi++){
				//見てるパターンの真ん中が0ではない→加算しないから飛ばす
				if(secondPatternEx[ms][5-mi]!=0){
					continue;
				}
				for(mj=0;mj<pNum;mj++){
					if(checkBoard[mi+mj]!=secondPatternEx[ms][mj+1]){
						continue;
					}
				}//最後までミスなし
				sumScore+=patternScore[secondPatternEx[ms][0]];
			}
		}
	}
	return sumScore;
}