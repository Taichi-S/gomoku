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