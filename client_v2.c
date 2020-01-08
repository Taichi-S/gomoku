//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define BOARD_SQUARE 15

//プロトタイプ宣言
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);

//盤面の情報を記録しておく配列
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
//最適な打ち手を探すために盤面のパターンによって行う点数付けを記録する盤面．自分と敵の2つのスコアボードを保持する．
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};

//自分が先行か後攻かを記録する(先行(黒)→1, 後攻(白)→2)
int player_number;
int enemy_number;

//勝利フラッグ(自分が勝ったらtrue)
bool isWin;

int main(void) {
	
	char *dst[10];		//bufferを分割した文字列を格納する
	int count;			//分割した文字数を格納する
	int bx, by;		//ボードに格納する座標を一時的に記録する
  
	
	//接続するサーバの情報の構造体を用意

	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(dest));

	//サーバの情報を入力

	printf("アドレスを入力してください\n");


	char destination[32];  //アドレス

	scanf("%s", destination);

	
	printf("ポート番号を入力してください\n");

	int port;  //アドレス

	scanf("%d", &port);


	dest.sin_port = htons(port);  //ポート番号

	dest.sin_family = AF_INET;

	dest.sin_addr.s_addr = inet_addr(destination);

	//ポート番号によってmySrtikeとenemy_numberを代入する
	if(port==12345){
		player_number = 1;
		enemy_number = 2;
	}else{
		player_number = 2;
		enemy_number = 1;
	}

	//ソケット通信の準備・生成

	WSADATA data;

	WSAStartup(MAKEWORD(2, 0), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



	//サーバへの接続

	if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%sに接続できませんでした\n", destination);

		return -1;

	}

	printf("%sに接続しました\n", destination);

  while(1){
	char buffer[1024];
	int i,j;

	//サーバからデータを受信

	recv(s, buffer, 1024, 0);

	printf("→ %s\n\n", buffer);

	//サーバーからの受信情報(buffer)をboard配列に挿入する
	count = split(dst, buffer, ',');
	
	bx=atoi(dst[0]);
	by=atoi(dst[1]);
	
	if(bx==0||by==0){
		
	}else{
		if(player_number==1){		//自分が先行の時、相手の手は2だから
			board[bx-1][by-1]=enemy_number;
		}else{
			board[bx-1][by-1]=enemy_number;
		}
		
	    int i=0;
	    printf("  ");
	    for(i = 1; i < BOARD_SQUARE+1; i++ ){
	        printf("%2d",i);
	    }
	    puts("");


		int j=0;
		for(i = 1; i < BOARD_SQUARE+1; i++ ){        
	        printf("%2d",i);        
	        for(j = 1; j < BOARD_SQUARE+1; j++ ){
	            
		    if(board[j-1][i-1]==0) printf(" -");
		    if(board[j-1][i-1]==1) printf(" o");
		    if(board[j-1][i-1]==2) printf(" x");

	        }
	        puts("");
	    }


		//点数付けした盤面を出力する(杉村)
		printf("SCOREBOARD");
		int i=0;
	    printf("  ");
	    for(i = 1; i < BOARD_SQUARE+1; i++ ){
	        printf("%2d",i);
	    }
	    puts("");


		int j=0;
		for(i = 1; i < BOARD_SQUARE+1; i++ ){        
	        printf("%2d",i);        
	        for(j = 1; j < BOARD_SQUARE+1; j++ ){
				printf("%d", scoreBoard[j-1][i-1]);
	        }
	        puts("");
	    }
    }
    
    //サーバにデータを送信

	printf("メッセージを入力してください\n");

	char message[32]; 

	scanf("%s", message);

	send(s, message, strlen(message), 0);
  	
  	//標準入力からの受信情報(message)をboard配列に挿入する
	count = split(dst, message, ',');
	
	bx=atoi(dst[0]);
	by=atoi(dst[1]);
	
	if(bx==0||by==0){
		
	}else{
		if(player_number==1){		//自分が先行の時、相手の手は2だから
			board[bx-1][by-1]=enemy_number;
		}else{
			board[bx-1][by-1]=enemy_number;
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
	            
		    if(board[j-1][i-1]==0) printf(" -");
		    if(board[j-1][i-1]==1) printf(" o");
		    if(board[j-1][i-1]==2) printf(" x");

	        }
	        puts("");
	    }

		//点数付けした盤面を出力する(杉村)
		printf("SCOREBOARD");
		int i=0;
	    printf("  ");
	    for(i = 1; i < BOARD_SQUARE+1; i++ ){
	        printf("%2d",i);
	    }
	    puts("");


		int j=0;
		for(i = 1; i < BOARD_SQUARE+1; i++ ){        
	        printf("%2d",i);        
	        for(j = 1; j < BOARD_SQUARE+1; j++ ){
				printf("%d", scoreBoard[j-1][i-1]);
	        }
	        puts("");
	    }
    }


	// Windows でのソケットの終了

	//closesocket(s);

	//WSACleanup();
  }


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
//盤面を全面探索して，まだ石が置かれていない場所の点数付けをする(点数は加算していく)．その値は，２次元配列scoreBoardに記録する．
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[9];
	int cdIni[4]={
		{0, -4},	//縦
		{-4, 0},	//横
		{-4, -4},	//右下がり斜め
		{4, -4}		//右上がり斜め
	}
	int cdGap[4]={
		{0, 1},		//縦
		{1, 0},		//横
		{1, 1},		//右下がり斜め
		{-1, 1}		//右上がり斜め
	}
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sx][sy]==0){		//石がおかれていないとき、４方向を見る
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
					for(cb=0;cb<9;cb++){
						checkBoard[cb]=board[tempX][tempY];		//checkBoardに一方向の盤の情報を格納
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					//スコアを加算して仮の盤面に格納する
					scoreBoard[sx][sy][player_number]+=boardScore(checkBoard[], player_number);
					scoreBoard[sx][sy][enemy_number]+=boardScore(checkBoard[], enemy_number);
				}
			}
		}
	}
}

//checkBoardを引数にそれをアルゴリズムの手と比べて、点数付けをしてそれらの点数を合計した値を返す
int boardScore(int checkBoard[], int whichPlayer){
	char checkStr[];
	int bs;
	int tempScore=0;
	//先行のプレイヤー(黒，1)のアルゴリズムの手をすべて羅列する(向きの逆も含む)
	int firstPatternEx[35]={
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
	}
	//後攻のプレイヤー(白，2)のアルゴリズムの手をすべて羅列する(向きの逆も含む)
	int secondPatternEx[35]={
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
	}
	int patternScore[7]={100, 80, 70, 60, 50, 40, 30}
	checkStr[] = convertInt(checkBoard[]);
	for(bs=0;bs<35;bs++){
		if(whichPlayer==1){		//先行のプレイヤーの点数を知りたいとき
			if(strstr(checkStr, firstPatternEx[bs]!){	//パターンが存在する
				tempScore+=patternScore[firstPatternEx[bs][0]];
			}
		}
		else{		//後攻のプレイヤーの点数を知りたいとき
			if(strstr(checkStr, secondPatternEx[bs]!){	//パターンが存在する
				tempScore+=patternScore[secondPatternEx[bs][0]];
			}
		}
	}
	return tempScore;
}

//intの配列をcharに変換する
char convertInt(int array[]){
	int arrayNumber = sizeof array / sizeof array[0];
	char str[arrayNumber+1];
	str[arrayNumber]='\0';
	int ci=0;
	for(ci=0;ci<arrayNumber;ci++){
		str[ci]=(char)array[ci];
	}
	return str[];
}





//ここから下は使わない(杉村)

//一回置き終わったら点数を消さないとあかんかも？（一個前の手を使うならいける)
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[4];
	int cdGap[8]={
		{-1, 0},	//左
		{-1, -1},	//左上
		{0, -1},	//上
		{1, -1},
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}		//左下
	}
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sx][sy]!=0){		//石がおかれている。ここを中心に８方位に４コマ分計算する
				for(cd=0;cd<8;cd++){
					for(cb=0;cb<4;cb++){
						checkBoard[cb]=board[sx+cdGap[cd][0]][sy+cdGap[cd][1]];		//checkBoardに一方向の盤の情報を格納
					}
					boardScore(checkBoard[]);
				}
			}
		}
	}
}

//checkBoardを引数にそれをアルゴリズムの手と比べて、点数付けをしてその点数を盤に埋め込む
void boardScore(int checkBoard[]){
	//アルゴリズムの手をすべて羅列する(向きの逆も含む)
	int pattenEx[]={
		{1,1,1,1,100},
		{1,1,1,0,80},
		{1,1,1,2,70},
		{0,1,1,1,70},
		{1,0,1,1,70},
		{1,1,0,0,60},//微妙
		{1,1,0,1,60},
		
}

//2つの配列の要素がすべての一致しているか調べる
bool array_equal(const int* array1, const int* array2){	
	for (size_t i = 0; i < size1; ++i) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

//ここは使わないよ(杉村)