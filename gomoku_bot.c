// gcc prog.c -o prog.exe [...other .c and .o files...] -lws2_32 ...

#include "winsock2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SQUARE 15

//scoring
void boardScoring();
int matchingScore(int *checkBoard, int whichPlayer, int putPlayer);
void output_array(int *in, int len);
void printScoreBoard(int whichPlayer);
void printBoard(void);

//cotaro
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);
int forbidden_hand_judgement(int bx, int by);

//bot
void gomokuBot(int array[2]);

// int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
int board[BOARD_SQUARE][BOARD_SQUARE]={{0}};
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};
int player_number;
int enemy_number;
int first_player = 1;//先攻
int second_player = 2;//後攻

int main(void) {
    char *dst[10]; //bufferを分割した文字列を格納する
	int count;	 //分割した文字数を格納する
	int bx, by;	//ボードに格納する座標を一時的に記録する

	//接続するサーバの情報の構造体を用意

	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(dest));

	//サーバの情報を入力

	printf("input an address: \n");

	char destination[32]; //アドレス

	scanf("%s", destination);

	printf("imput a port\n");

	int port; //アドレス

	scanf("%d", &port);

	dest.sin_port = htons(port); //ポート番号

	dest.sin_family = AF_INET;

	dest.sin_addr.s_addr = inet_addr(destination);

	//ポート番号によってmySrtikeとenemy_numberを代入する
	if (port == 12345)
	{
		player_number = first_player;
		enemy_number = second_player;
	}
	else
	{
		player_number = second_player;
		enemy_number = first_player;
	}

	//ソケット通信の準備・生成

	WSADATA data;

	WSAStartup(MAKEWORD(2, 0), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//サーバへの接続

	if (connect(s, (struct sockaddr *)&dest, sizeof(dest)))
	{

		printf("you could not connect to%s\n", destination);

		return -1;
	}

	printf("you could connect to %s\n", destination);

    int start_count = 0;
    //以下bot
    while (1)
	{
		char buffer[1024];
		
		recv(s, buffer, 1024, 0);

		printf("->%s\n\n", buffer);
        
        if(start_count==0){
            char message[32];

		    scanf("%s", message);

		    send(s, message, strlen(message), 0);

            start_count++;
            continue;
        }
		//サーバーからの受信情報(buffer)をboard配列に挿入する
		count = split(dst, buffer, ',');

		bx = atoi(dst[0]);
		by = atoi(dst[1]);

		if (bx == 0 || by == 0)
		{
		}
		else
		{
			if (enemy_number == first_player)
			{ //相手先行で、手を置いた時
				if (forbidden_hand_judgement(bx, by) > 0)
				{
					printf("It is forbidden_hand!");
					puts("I am Winner!!");
					while (1);
				}
			}
            board[by - 1][bx - 1] = enemy_number;
            printBoard();
		}

		//サーバにデータを送信

		printf("bot turn\n");
        
        int array[2];
        gomokuBot(array);
        bx = *array;
        by = *(array + 1);
		if (bx == 0 || by == 0)
		{
		}
		else
		{
				board[by - 1][bx - 1] = player_number;
                printBoard();
		}
        char message[32];
        sprintf(message, "%d,%d", bx, by);
        
        puts(message);
        send(s, message, strlen(message), 0);
		// Windows でのソケットの終了
		//closesocket(s);
		//WSACleanup();
	}
    // printBoard();

    // boardScoring();
    // printScoreBoard(1);
    // printScoreBoard(2);
	return 0;

}

void gomokuBot(int array[2]){
    int bx, by;
    
   
    *array = rand()%15+1;
    *(array+1) = rand()%15+1;
}

int forbidden_hand_judgement(int bx, int by)
{
    int tmp_board[BOARD_SQUARE][BOARD_SQUARE];
    int tmp_i, tmp_j;

    //配列のコピー
    for(tmp_i=0; tmp_i<BOARD_SQUARE; tmp_i++){
        for(tmp_j=0; tmp_j<BOARD_SQUARE; tmp_j++){
            tmp_board[tmp_i][tmp_j] = board[tmp_i][tmp_j];
        }
    }
    tmp_board[by-1][bx-1];
	//三三禁
	char xsigns[8] = "++0-"; //対象座標から八方への走査に必要な符号
	char ysigns[8] = "0+++";

	int ki = 0;
	int mj = 0;
	for (ki = 1; ki < BOARD_SQUARE + 1; ki++)
	{


		for (mj = 1; mj < BOARD_SQUARE + 1; mj++)
		{
			int san_counter = 0; //三三禁になりうる三を数えるもの
			int yon_counter = 0; //四四禁になりうる四を数えるもの
			int choren_flag = 0;

			if (tmp_board[mj - 1][ki - 1] == 1)
			{ //対象座標が、先行の相手が置いたもの
				int ik = 0;
				for (ik = 0; ik < 4; ik++)
				{				 //対象座標から八方（４X２）を走査
					int xvector; //対象座標から進む向きをここに入れる
					int yvector;

					switch (xsigns[ik])
					{ //対象座標から進む向きを決定
					case '+':
						xvector = 1;
						break;
					case '-':
						xvector = -1;
						break;
					case '0':
						xvector = 0;
						break;
					}

					switch (ysigns[ik])
					{
					case '+':
						yvector = 1;
						break;
					case '-':
						yvector = -1;
						break;
					case '0':
						yvector = 0;
						break;
					}

					int jm = 0;
					int xweight = 0; //対象座標が進む向きと大きさをここに入れる
					int yweight = 0;
					int xmax = mj - 1; //対象の座標を含む直線上にあり、判定に必要な最大の座標と最小の座標をここに入れる
					int ymax = ki - 1;
					int xmin = mj - 1;
					int ymin = ki - 1;
					int black_counter = 0;  //対象の座標を含む直線上にある先攻の手を数えるもの
					int san_reach_flag = 0; //三が一つ見つかった時点でこのフラグを１にする
					int yon_reach_flag = 0;
					int white_in_plus_flag = 0;
					int white_in_minus_flag = 0;

					for (int jm = 0; jm < 4; jm++)
					{						//対象座標から三々禁となりうる直線上の範囲を走査
						xweight += xvector; //対象座標が進む向きと大きさを決定
						yweight += yvector;

						if (tmp_board[mj - 1 + xweight][ki - 1 + yweight] == 1 && white_in_plus_flag == 0)
						{							 //進んだ先の座標が先攻（相手）の手であれば-(*)
							black_counter++;		 //対象の座標を含む直線上にある先攻の手をカウント
							xmax = mj - 1 + xweight; //その手（座標）を最大座標として保存
							ymax = ki - 1 + yweight;
						}
						else if (tmp_board[mj - 1 + xweight][ki - 1 + yweight] == 2)
						{
							white_in_plus_flag = 1;
						}

						if (black_counter == 2 && san_reach_flag == 0)
						{						//対象座標の他に直線上にある先攻の手が２つあれば
							san_counter++;		//三々禁となりうる三をカウント
							san_reach_flag = 1; //三フラグを立てる
												//break;	//今回の直線の走査は終了
						}
						else if (black_counter == 3 && yon_reach_flag == 0)
						{						//対象座標の他に直線上にある先攻の手が２つあれば
							yon_counter++;		//四四禁となりうる四をカウント
							yon_reach_flag = 1; //四フラグを立てる
												//break;	//今回の直線の走査は終了
						}

						if (abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4)
						{
							if (black_counter == 4)
							{   //相手の手が５つ並んでいたら
								//長連の可能性あり
							}
						}
						else if (abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4)
						{
							if (black_counter == 5)
							{					 //相手の手が６つ並んだら
								choren_flag = 1; //長連フラグを立てる
								break;
							}
							else
							{
								break; //禁じ手となり得ない範囲まで走査したら、今回の直線の走査は終了
							}
						}

						if (tmp_board[mj - 1 - xweight][ki - 1 - yweight] == 1 && white_in_minus_flag == 0)
						{							 //（*)とは反対向きに進んだ先の座標が先攻（相手）の手であれば
							black_counter++;		 //対象の座標を含む直線上にある先攻の手をカウント
							xmin = mj - 1 - xweight; //その手（座標）を最小座標として保存
							ymin = ki - 1 - yweight;
						}
						else if (tmp_board[mj - 1 + xweight][ki - 1 + yweight] == 2)
						{
							white_in_minus_flag = 1;
						}

						if (black_counter == 2 && san_reach_flag == 0)
						{				   //対象座標の他に直線上にある先攻の手が２つあれば
							san_counter++; //三々禁となりうる三をカウント
							san_reach_flag = 1;
							//break;	//今回の直線の走査は終了
						}
						else if (black_counter == 3 && yon_reach_flag == 0)
						{				   //対象座標の他に直線上にある先攻の手が２つあれば
							yon_counter++; //四四禁となりうる四をカウント
							yon_reach_flag = 1;
							//break;	//今回の直線の走査は終了
						}

						if (abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4)
						{
							if (black_counter == 4)
							{   //相手の手が５つ並んでいたら
								//長連の可能性
							}
						}
						else if (abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4)
						{
							if (black_counter == 5)
							{					 //相手の手が６つ並んだら
								choren_flag = 1; //長連フラグを立てる
								
							}
							else
							{
								 //三となり得ない範囲まで走査したら、今回の直線の走査は終了
							}
						}
					}
				}

				if (yon_counter == 2)
				{ //四四禁があれば
					puts("It is forbidden hand!:yonyonkin");
					//end_flag = 1;
					return 1;
					 //禁じ手のプログラムを終了
				}
				else if (san_counter == 2 && yon_counter == 0)
				{ //三三禁があれば
					puts("It is forbidden hand!:sansankin");
					//end_flag = 1;
					return 1;
					 //禁じ手のプログラムを終了
				}
				else if (choren_flag == 1)
				{ //長連があれあば
					puts("It is forbidden hand!:choren");
					//end_flag = 1;
					return 1;
					//禁じ手のプログラムを終了
				}
			}
			else
			{
			}

			//if(end_flag) break;
		}
		//if(end_flag) break;
	}
    return 0;
	// if(end_flag){
	// 	puts("I am Winner!!");
	// 	while(1);
	// }
}

void printBoard(void){
    int i,j;

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
}

void printScoreBoard(int whichPlayer){
    printf("SCOREBOARD(%dstPlayer)\n",whichPlayer);
    int i=0;
    int j=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            printf("%2d", scoreBoard[i-1][j-1][whichPlayer-1]);
        }
        puts("");
    }
}

void boardScoring(){

	int sx,sy;
	int cd, cb;
	int checkBoard[9];
	int cdIni[4][2]={
		{0, -4},	
		{-4, 0},
		{-4, -4},
		{4, -4}		
	};
	int cdGap[4][2]={
		{0, 1},		
		{1, 0},		
		{1, 1},	
		{-1, 1}		
	};
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sy][sx]==0){		
                //printf("x=%d, y=%d\n", sx, sy);
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
                    //printf("tempX=%d ,tempY=%d\n", tempX, tempY);
					for(cb=0;cb<9;cb++){
						if(tempY<0 | tempX<0){
                            checkBoard[cb]=0;
                        }else{
                            checkBoard[cb]=board[tempY][tempX];	
                        }
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					
					//output_array(checkBoard, sizeof(checkBoard)/sizeof(int));
					
					scoreBoard[sy][sx][player_number-1]+=matchingScore(checkBoard, player_number, 1);
					scoreBoard[sy][sx][enemy_number-1]+=matchingScore(checkBoard, enemy_number, 2);
				}
			}
		}
	}
}
void output_array(int *in, int len){
    int i;
	for(i=0; i<len; i++){
		printf("%d-",in[i]);
	}
	puts("");
}


int matchingScore(int *checkBoard, int whichPlayer, int putPlayer){
	int mi, mj;
	int cont=0;
    int tmp=0;
    checkBoard[4] = putPlayer;
	
	for(mi=0; mi<9; mi++){
        if(checkBoard[mi]==whichPlayer){
            cont++;
        }else{
            if(tmp<cont){
                tmp=cont;
            }
            cont=0;
        }
    }
    if(tmp<cont){
        tmp=cont;
    }
    return tmp-1;
}

int isDelimiter(char p, char delim)
{
	return p == delim;
}

int split(char *dst[], char *src, char delim)
{
	int count = 0;

	for (;;)
	{
		while (isDelimiter(*src, delim))
		{
			src++;
		}

		if (*src == '\0')
			break;

		dst[count++] = src;

		while (*src && !isDelimiter(*src, delim))
		{
			src++;
		}

		if (*src == '\0')
			break;

		*src++ = '\0';
	}
	return count;
}