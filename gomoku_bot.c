// gcc prog.c -o prog.exe [...other .c and .o files...] -lws2_32 ...

#include "winsock2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SQUARE 15

typedef struct Map{
    int x;
    int y;
    int score;
    int enemy_score;
    int score_diff;
    int enemy_x;
    int enemy_y;
}map;
//scoring
int boardScoring(map tmp_map[BOARD_SQUARE*BOARD_SQUARE],int count, int whichPlayer,  int statusBoard[BOARD_SQUARE][BOARD_SQUARE],int print);
int matchingScore(int *checkBoard, int whichPlayer, int putPlayer);
void output_array(int *in, int len);
void printScoreBoard(int tmp_board[BOARD_SQUARE][BOARD_SQUARE]);
void printBoard(void);
void convert(map tmp_map[BOARD_SQUARE*BOARD_SQUARE], map score_map[BOARD_SQUARE*BOARD_SQUARE], int mode);
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);
int forbidden_hand_judgement(int bx, int by);

//bot
void gomokuBot(int array[2]);
int getOpposite(int whichPlayer);
void printScoreBoard2(map tmp_board[BOARD_SQUARE*BOARD_SQUARE]);
void check(int array[2], map map_array[BOARD_SQUARE*BOARD_SQUARE], int mode);
int checkDuplication(int x, int y);

// int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
int board[BOARD_SQUARE][BOARD_SQUARE]={{0}};
//int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};
int player_number;
int enemy_number;
int first_player = 1;//先攻
int second_player = 2;//後攻
int start_count = 0;
int isWin = 0;
int isDraw = 0;
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
        char message[255];
        if(isWin){
            sprintf(message, "%d,%d, win", bx, by);
            send(s, message, strlen(message), 0);
            while(1);
        }else if(isDraw){
            sprintf(message, "%d,%d, draw", bx, by);
            send(s, message, strlen(message), 0);
            while(1);
        }else{
            sprintf(message, "%d,%d", bx, by);
        }
        
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
    
    int tmp_array[2];
    int tmp_board[BOARD_SQUARE][BOARD_SQUARE];
    int tmp_i, tmp_j;

    //配列のコピー
    for(tmp_i=0; tmp_i<BOARD_SQUARE; tmp_i++){
        for(tmp_j=0; tmp_j<BOARD_SQUARE; tmp_j++){
                tmp_board[tmp_i][tmp_j] = board[tmp_i][tmp_j];
        }
    }
    int tmp;
    map tmp_map[BOARD_SQUARE*BOARD_SQUARE];
    if(player_number == second_player){
        if(start_count==1){
            start_count++;
            *array = 9;
            *(array+1) = 9;
            return;
        }
        
        tmp = boardScoring(tmp_map, 1, player_number, tmp_board, 1);
        if(tmp==1){
            puts("attack");
        }else if(tmp==0){
            puts("diffence");
        }else{
            puts("normal");
        }
        int wei;
        for(wei=0; wei<BOARD_SQUARE*BOARD_SQUARE; wei++){
            printf("%d,%d  score=%d, enemy_score=%d, diff=%d, enemy_x=%d, enemy_y=%d\n", tmp_map[wei].x, tmp_map[wei].y, tmp_map[wei].score, tmp_map[wei].enemy_score, tmp_map[wei].score_diff, tmp_map[wei].enemy_x, tmp_map[wei].enemy_y);
        }


        check(array, tmp_map, tmp);
        
        return;
    }else{
        if(start_count==1){
            start_count++;
            *array = 8;
            *(array+1) = 8;
            return;
        }
        tmp = boardScoring(tmp_map, 1, player_number, tmp_board, 1);
        if(tmp==1){
            puts("attack");
        }else if(tmp==0){
            puts("diffence");
        }else{
            puts("normal");
        }
        int wei;
        for(wei=0; wei<BOARD_SQUARE*BOARD_SQUARE; wei++){
            printf("%d,%d  score=%d, enemy_score=%d, diff=%d, enemy_x=%d, enemy_y=%d\n", tmp_map[wei].x, tmp_map[wei].y, tmp_map[wei].score, tmp_map[wei].enemy_score, tmp_map[wei].score_diff, tmp_map[wei].enemy_x, tmp_map[wei].enemy_y);
        }


        check(array, tmp_map, tmp);
    }
    
}

void check(int array[2], map map_array[BOARD_SQUARE*BOARD_SQUARE], int mode){
    int i;
    int diffence = 0;
    if(mode==diffence){
        for(i=0; i<BOARD_SQUARE*BOARD_SQUARE; i++){
            //printf("%d\n",board[map_array[i].enemy_y-1][map_array[i].enemy_x-1]);
            if(checkDuplication(map_array[i].enemy_x,map_array[i].enemy_y)){
                printf("->%d,%d  score=%d, enemy_score=%d, diff=%d, enemy_x=%d, enemy_y=%d\n", map_array[i].x, map_array[i].y, map_array[i].score, map_array[i].enemy_score, map_array[i].score_diff, map_array[i].enemy_x, map_array[i].enemy_y);
                *array = map_array[i].enemy_x;
                *(array+1) = map_array[i].enemy_y;
                return;
            }
        }
        isDraw = 1;
    }else{
        for(i=0; i<BOARD_SQUARE*BOARD_SQUARE; i++){
            if(checkDuplication(map_array[i].x, map_array[i].y)){
                *array = map_array[i].x;
                *(array+1) = map_array[i].y;
                return;
            }
        }
        isDraw = 1;
    }
}

int checkDuplication(int x, int y){
    if(x==0 || y == 0){
        return 0;
    }
    if(board[y-1][x-1]==0){
        return 1;
    }
    return 0;
}



int boardScoring(map tmp_map[BOARD_SQUARE*BOARD_SQUARE],int count, int whichPlayer, int statusBoard[BOARD_SQUARE][BOARD_SQUARE],int print){
    if(whichPlayer==enemy_number){
       // count--;
    }
    int tmp_board[BOARD_SQUARE][BOARD_SQUARE];
    int tmp_i, tmp_j;

    //配列のコピー
    for(tmp_i=0; tmp_i<BOARD_SQUARE; tmp_i++){
        for(tmp_j=0; tmp_j<BOARD_SQUARE; tmp_j++){
            tmp_board[tmp_i][tmp_j] = statusBoard[tmp_i][tmp_j];
        }
    }

    map scoreMap[BOARD_SQUARE*BOARD_SQUARE];
    int attack = 1;
    int diffence = 0;
    int normal = 2;
    int mode=normal;
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
            scoreMap[sy*BOARD_SQUARE+sx].score = 0;	
            scoreMap[sy*BOARD_SQUARE+sx].x = sx+1;	
            scoreMap[sy*BOARD_SQUARE+sx].y = sy+1;
            scoreMap[sy*BOARD_SQUARE+sx].enemy_score = 0;
            scoreMap[sy*BOARD_SQUARE+sx].score_diff = 0;
			if(tmp_board[sy][sx]==0){	
                int tmp = 0;
                //printf("x=%d, y=%d\n", sx, sy);
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
                    //printf("tempX=%d ,tempY=%d\n", tempX, tempY);
					for(cb=0;cb<9;cb++){
						if((tempY<0 || tempX<0) || (tempX>=15 || tempY>=15)){
                            checkBoard[cb]=0;
                        }else{
                            checkBoard[cb]=tmp_board[tempY][tempX];	
                        }
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					
					//output_array(checkBoard, sizeof(checkBoard)/sizeof(int));
					if(scoreMap[sy*BOARD_SQUARE+sx].score < matchingScore(checkBoard, whichPlayer, whichPlayer)){
					    scoreMap[sy*BOARD_SQUARE+sx].score = matchingScore(checkBoard, whichPlayer, whichPlayer);
                    }
                    if((scoreMap[sy*BOARD_SQUARE+sx].score >= 4) && whichPlayer==player_number){
                        
            
                        isWin=1;
                        
                    
                    }
                    scoreMap[sy*BOARD_SQUARE+sx].x = sx+1;
                    scoreMap[sy*BOARD_SQUARE+sx].y = sy+1;
					//scoreBoard[sy][sx][enemy_number-1]+=matchingScore(checkBoard, enemy_number, enemy_number);
				}
                
                if(count>0){
                    tmp_board[sy][sx] = whichPlayer;
                    map re_tmp_map[BOARD_SQUARE*BOARD_SQUARE];
                    
                    boardScoring(re_tmp_map,0, getOpposite(whichPlayer), tmp_board,0);
                    scoreMap[sy*BOARD_SQUARE+sx].enemy_x = re_tmp_map[0].x;
                    scoreMap[sy*BOARD_SQUARE+sx].enemy_y = re_tmp_map[0].y;
                    scoreMap[sy*BOARD_SQUARE+sx].enemy_score = re_tmp_map[0].score;
                    scoreMap[sy*BOARD_SQUARE+sx].score_diff = scoreMap[sy*BOARD_SQUARE+sx].score - scoreMap[sy*BOARD_SQUARE+sx].enemy_score;
                    tmp_board[sy][sx] = 0;
                    
                }
                
			}
		}
	}
    if(print){
        printScoreBoard(tmp_board);
        printScoreBoard2(scoreMap);
    }
    if(count>0){
        int index;
        for(index=0; index<BOARD_SQUARE*BOARD_SQUARE; index++){
            if((scoreMap[index].score_diff >= 0) && checkDuplication(scoreMap[index].x, scoreMap[index].y)){
                mode = attack;
            }
        }
        if(mode!=attack){

            mode= diffence;
        }
    }
    //printScoreBoard(scoreBoard);
    convert(tmp_map, scoreMap, mode);
    return mode;
}


void convert(map tmp_map[BOARD_SQUARE*BOARD_SQUARE], map score_map[BOARD_SQUARE*BOARD_SQUARE], int mode){
    int attack = 1;
    int diffence = 0;
    int normal = 2;
    map tmp_board[BOARD_SQUARE*BOARD_SQUARE];
    int tmp_i, tmp_j;
    for(tmp_i=0; tmp_i<BOARD_SQUARE*BOARD_SQUARE; tmp_i++){
        
        tmp_map[tmp_i].score = score_map[tmp_i].score;
        tmp_map[tmp_i].enemy_score = score_map[tmp_i].enemy_score;
        tmp_map[tmp_i].score_diff = score_map[tmp_i].score_diff;
        tmp_map[tmp_i].x = score_map[tmp_i].x;
        tmp_map[tmp_i].y = score_map[tmp_i].y;
        tmp_map[tmp_i].enemy_x = score_map[tmp_i].enemy_x;
        tmp_map[tmp_i].enemy_y = score_map[tmp_i].enemy_y;
        
    }
    int max;
    int max_x, max_y;
    if(mode==attack){
        for(tmp_i=0; tmp_i<BOARD_SQUARE*BOARD_SQUARE; tmp_i++){
        
            for(tmp_j=0; tmp_j<BOARD_SQUARE*BOARD_SQUARE-1; tmp_j++){
                if(tmp_map[tmp_j].score_diff<tmp_map[tmp_j+1].score_diff){
                    map tmp;
                    tmp =tmp_map[tmp_j+1];
                    tmp_map[tmp_j+1] = tmp_map[tmp_j];
                    tmp_map[tmp_j] = tmp;
                }
            }
        }
        
    }else if(mode==diffence){
        for(tmp_i=0; tmp_i<BOARD_SQUARE*BOARD_SQUARE; tmp_i++){
        
            for(tmp_j=0; tmp_j<BOARD_SQUARE*BOARD_SQUARE-1; tmp_j++){
                if(tmp_map[tmp_j].enemy_score<tmp_map[tmp_j+1].enemy_score){
                    map tmp;
                    tmp =tmp_map[tmp_j+1];
                    tmp_map[tmp_j+1] = tmp_map[tmp_j];
                    tmp_map[tmp_j] = tmp;
                }
            }
        }
        
    }else{
        for(tmp_i=0; tmp_i<BOARD_SQUARE*BOARD_SQUARE; tmp_i++){
        
            for(tmp_j=0; tmp_j<BOARD_SQUARE*BOARD_SQUARE-1; tmp_j++){
                if(tmp_map[tmp_j].score<tmp_map[tmp_j+1].score){
                    map tmp;
                    tmp =tmp_map[tmp_j+1];
                    tmp_map[tmp_j+1] = tmp_map[tmp_j];
                    tmp_map[tmp_j] = tmp;
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

int getOpposite(int whichPlayer){
    if(whichPlayer == player_number){
        return enemy_number;
    }else{
        return player_number;
    }
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

void printScoreBoard(int tmp_board[BOARD_SQUARE][BOARD_SQUARE]){
    printf("SCOREBOARD(Player)\n");
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
            printf("%2d", tmp_board[i-1][j-1]);
        }
        puts("");
    }
}

void printScoreBoard2(map tmp_board[BOARD_SQUARE*BOARD_SQUARE]){
    printf("wei=%d,%d\n");
    int i=0;
    int j=0;
    printf("  ");
    for(i = 0; i < BOARD_SQUARE; i++ ){
        printf("%2d",i+1);
    }
    puts("");


    j=0;
    for(i = 0; i < BOARD_SQUARE; i++ ){        
        printf("%2d",i+1);        
        for(j = 0; j < BOARD_SQUARE; j++ ){
            printf("%2d", tmp_board[i*BOARD_SQUARE + j].score);
        }
        puts("");
    }
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

int forbidden_hand_judgement(int bx,int by){
	//三三禁
	char xsigns[8]="++0-";	//置かれた手から八方への走査に必要な符号
	char ysigns[8]="0+++";

	int san_counter = 0;	//三三禁になりうる三を数えるもの
	int yon_counter = 0;	//四四禁になりうる四を数えるもの
	int choren_flag = 0; //長連があるかないか
					
	int ik = 0;
	for(ik = 0; ik < 4; ik ++){	//置かれた手から八方（４X２）を走査
		int xvector;	//置かれた手から進む向きをここに入れる
		int yvector;

		switch (xsigns[ik]){	//置かれた手から進む向きを決定
			case '+': xvector = 1; break;
			case '-': xvector = -1; break;
			case '0': xvector = 0; break;
		}

		switch (ysigns[ik]){
			case '+': yvector = 1; break;
			case '-': yvector = -1; break;
			case '0': yvector = 0; break;
		}

		int jm = 0;
		int xweight = 0;	//置かれた手から走査を進める向きとその大きさをここに入れる
		int yweight = 0;
		int xmax = bx-1;	//置かれた手を含む直線上にある、黒の手の最大の座標と最小の座標をここに入れる
		int ymax = by-1;
		int xmin = bx-1;
		int ymin = by-1;
		int black_counter = 0;	//置かれた手を含む直線上にある先攻の手を数えるもの
		int san_reach_flag = 0; //三が一つ見つかった時点でこのフラグを１にする
		int yon_reach_flag = 0;
		int white_in_plus_flag = 0; //直線走査中に白の手が見つかり次第その方向の走査をとめるためのフラグ
		int white_in_minus_flag = 0;

		for(int jm = 0; jm < 4; jm++){ //置かれた手から、禁じ手となる手が存在しうる範囲で、直線上を走査
			xweight += xvector;	//置かれた手が進む向きと大きさを決定
			yweight += yvector;

			if(board[by-1+yweight][bx-1+xweight] == 1 && white_in_plus_flag == 0){	//走査対象の座標が黒の手であれば-(*)
				black_counter++; //置かれた手を含む直線上の黒の手をカウント
				xmax = bx-1+xweight; //その手（座標）を最大座標として保存
				ymax = by-1+yweight;
			} else if(board[by-1+yweight][bx-1+xweight] == 2){ //走査中に白の手があれば
				white_in_plus_flag = 1;//フラグを立てる
			}

			if(black_counter == 2 && san_reach_flag == 0){	//置かれた手の他に一直線上に黒の手が２つ見つかれば
				san_counter++; //三々禁となりうる三をカウント
				san_reach_flag = 1;//三フラグを立てる
			}else if(black_counter == 3 && yon_reach_flag == 0){	//置かれた手の他に一直線上に黒の手が3つ見つかれば
				yon_counter++; //四四禁となりうる四をカウント
				yon_reach_flag = 1;//四フラグを立てる
			}
		
			if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
				if(black_counter == 4){ //黒の手が５つ並んでいたら
					
				}
			}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
				if(black_counter == 5){ //黒の手が６つ並んだら
					choren_flag = 1; //長連フラグを立てる
					break;
				}else{
					break; //禁じ手となり得ない範囲まで走査したら、今回の直線の走査は終了
				}
			} 

			if(board[by-1-yweight][bx-1-xweight] == 1&& white_in_minus_flag == 0){ //（*)とは反対向きに進んだ時の走査対象の座標が黒の手であれば
				black_counter++;	//置かれた手を含む直線上の黒の手をカウント
				xmin = bx-1-xweight;	//その手（座標）を最小座標として保存
				ymin = by-1-yweight;
			}else if(board[by-1+yweight][bx-1+xweight] == 2){ //走査中に白の手があれば
				white_in_minus_flag = 1;//フラグを立てる
			}

			if(black_counter == 2 && san_reach_flag == 0){	//置かれた手の他に一直線上に黒の手が２つ見つかれば
				san_counter++;	//三々禁となりうる三をカウント
				san_reach_flag = 1;//三フラグを立てる
			}else if(black_counter == 3 && yon_reach_flag == 0){	//置かれた手の他に一直線上に黒の手が3つ見つかれば
				yon_counter++; //四四禁となりうる四をカウント
				yon_reach_flag = 1;//四フラグを立てる
			}

			if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
				if(black_counter == 4){	//黒の手が５つ並んでいたら
					//長連の可能性
				}
			}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
				if(black_counter == 5){ //黒の手が６つ並んだら
					choren_flag = 1; //長連フラグを立てる
					break;
				}else{
					break; //禁じ手となり得ない範囲まで走査したら、今回の直線の走査は終了
				}
			} 
		}
						
	}

	if(yon_counter == 2) {	//四四禁があれば
		puts("It is forbidden hand!:yonyonkin");
		return 1;
	}else if(san_counter == 2 && yon_counter == 0) {	//三三禁があれば
		puts("It is forbidden hand!:sansankin");
		return 1;
	}else if(choren_flag == 1){ //長連があれあば
		puts("It is forbidden hand!:choren");
		return 1;
	}
	return 0;
}