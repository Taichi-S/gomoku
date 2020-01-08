//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BOARD_SQUARE 15

//プロトタイプ宣言
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);

//盤面の情報を記録しておく配列
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};

//禁じ手判定の関数
int forbidden_hand_judgement(int board[BOARD_SQUARE][BOARD_SQUARE]){
	//三三禁
			char xsigns[8]="++0-";	//対象座標から八方への走査に必要な符号
			char ysigns[8]="0+++";

			int ki = 0;
			int mj = 0;
			for(ki = 1; ki < BOARD_SQUARE + 1; ki++ ){

				for(mj = 1; mj < BOARD_SQUARE + 1; mj++ ){
					int san_counter = 0;	//三三禁になりうる三を数えるもの
					int yon_counter = 0;	//四四禁になりうる四を数えるもの
					int choren_flag = 0;

					if(board[mj-1][ki-1] == 1){	//対象座標が、先行の相手が置いたもの
						int ik = 0;
						for(ik = 0; ik < 4; ik ++){	//対象座標から八方（４X２）を走査
							int xvector;	//対象座標から進む向きをここに入れる
							int yvector;

							switch (xsigns[ik]){	//対象座標から進む向きを決定
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
							int xweight = 0;	//対象座標が進む向きと大きさをここに入れる
							int yweight = 0;
							int xmax = mj-1;	//対象の座標を含む直線上にあり、判定に必要な最大の座標と最小の座標をここに入れる
							int ymax = ki-1;
							int xmin = mj-1;
							int ymin = ki-1;
							int black_counter = 0;	//対象の座標を含む直線上にある先攻の手を数えるもの
							int san_reach_flag = 0; //三が一つ見つかった時点でこのフラグを１にする
							int yon_reach_flag = 0;

							for(int jm = 0; jm < 4; jm++){ //対象座標から三々禁となりうる直線上の範囲を走査
								xweight += xvector;	//対象座標が進む向きと大きさを決定
								yweight += yvector;
	
								if(board[mj-1+xweight][ki-1+yweight] == 1){	//進んだ先の座標が先攻（相手）の手であれば-(*)
									black_counter++; //対象の座標を含む直線上にある先攻の手をカウント
									xmax = mj-1+xweight; //その手（座標）を最大座標として保存
									ymax = ki-1+yweight;
								} 

								if(black_counter == 2 && san_reach_flag == 0){	//対象座標の他に直線上にある先攻の手が２つあれば
									san_counter++; //三々禁となりうる三をカウント
									san_reach_flag = 1;//三フラグを立てる
									//break;	//今回の直線の走査は終了
								}else if(black_counter == 3 && yon_reach_flag == 0){	//対象座標の他に直線上にある先攻の手が２つあれば
									yon_counter++; //四四禁となりうる四をカウント
									yon_reach_flag = 1;//四フラグを立てる
									//break;	//今回の直線の走査は終了
								}
							
								if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
									if(black_counter == 4){ //相手の手が５つ並んでいたら
										//長連の可能性あり
									}
								}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
									if(black_counter == 5){ //相手の手が６つ並んだら
										choren_flag = 1; //長連フラグを立てる
										break;
									}else{
										break; //禁じ手となり得ない範囲まで走査したら、今回の直線の走査は終了
									}
								} 

								if(board[mj-1-xweight][ki-1-yweight] == 1){ //（*)とは反対向きに進んだ先の座標が先攻（相手）の手であれば
									black_counter++;	//対象の座標を含む直線上にある先攻の手をカウント
									xmin = mj-1-xweight;	//その手（座標）を最小座標として保存
									ymin = ki-1-yweight;
								}

								if(black_counter == 2 && san_reach_flag == 0){	//対象座標の他に直線上にある先攻の手が２つあれば
									san_counter++;	//三々禁となりうる三をカウント
									san_reach_flag = 1;
									//break;	//今回の直線の走査は終了
								}else if(black_counter == 3 && yon_reach_flag == 0){	//対象座標の他に直線上にある先攻の手が２つあれば
									yon_counter++; //四四禁となりうる四をカウント
									yon_reach_flag = 1;
									//break;	//今回の直線の走査は終了
								}

								if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
									if(black_counter == 4){	//相手の手が５つ並んでいたら
										//長連の可能性
									}
								}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
									if(black_counter == 5){ //相手の手が６つ並んだら
										choren_flag = 1; //長連フラグを立てる
										break;
									}else{
										break; //三となり得ない範囲まで走査したら、今回の直線の走査は終了
									}
								} 
							}
						
						}

						if(yon_counter == 2) {	//四四禁があれば
							puts("It is forbidden hand!:yonyonkin");
							//end_flag = 1;
							return 1;
							break;	//禁じ手のプログラムを終了
						}else if(san_counter == 2 && yon_counter == 0) {	//三三禁があれば
							puts("It is forbidden hand!:sansankin");
							//end_flag = 1;
							return 1;
							break;	//禁じ手のプログラムを終了
						}else if(choren_flag == 1){ //長連があれあば
							puts("It is forbidden hand!:choren");
							//end_flag = 1;
							return 1;
							break;	//禁じ手のプログラムを終了
						}
					}else{
						
					}

					//if(end_flag) break;

				}
					//if(end_flag) break;
			}
			// if(end_flag){
			// 	puts("I am Winner!!");
			// 	while(1);
			// }
}

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
		if(port==12345){		//自分が先行の時、相手の手は2だから
			board[bx-1][by-1]=2;
		}else{
			board[bx-1][by-1]=1;
		}

		//禁じ手判定のプログラム
		int end_flag = 0;
		if(board[bx-1][by-1] == 1){ //相手先行で、手を置いた時
			if(forbidden_hand_judgement(board) > 0) printf("It is forbidden_hand!");
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
		if(port==12345){		//自分が先行の時、自分の手は1だから
			board[bx-1][by-1]=1;
		}else{
			board[bx-1][by-1]=2;
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