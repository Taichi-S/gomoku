//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BOARD_SQUARE 15

//�v���g�^�C�v�錾
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);

//�Ֆʂ̏����L�^���Ă����z��
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};

int main(void) {
	
	char *dst[10];		//buffer�𕪊�������������i�[����
	int count;			//�����������������i�[����
	int bx, by;		//�{�[�h�Ɋi�[������W���ꎞ�I�ɋL�^����
  
	
	//�ڑ�����T�[�o�̏��̍\���̂�p��

	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(dest));

	//�T�[�o�̏������

	printf("�A�h���X����͂��Ă�������\n");


	char destination[32];  //�A�h���X

	scanf("%s", destination);

	
	printf("�|�[�g�ԍ�����͂��Ă�������\n");

	int port;  //�A�h���X

	scanf("%d", &port);


	dest.sin_port = htons(port);  //�|�[�g�ԍ�

	dest.sin_family = AF_INET;

	dest.sin_addr.s_addr = inet_addr(destination);



	//�\�P�b�g�ʐM�̏����E����

	WSADATA data;

	WSAStartup(MAKEWORD(2, 0), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



	//�T�[�o�ւ̐ڑ�

	if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%s�ɐڑ��ł��܂���ł���\n", destination);

		return -1;

	}

	printf("%s�ɐڑ����܂���\n", destination);

  while(1){
	char buffer[1024];
	int i,j;

	//�T�[�o����f�[�^����M

	recv(s, buffer, 1024, 0);

	printf("�� %s\n\n", buffer);

	//�T�[�o�[����̎�M���(buffer)��board�z��ɑ}������
	count = split(dst, buffer, ',');
	
	bx=atoi(dst[0]);
	by=atoi(dst[1]);
	
	if(bx==0||by==0){
		
	}else{
		if(port==12345){		//��������s�̎��A����̎��2������
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
    
        //�T�[�o�Ƀf�[�^�𑗐M

	printf("���b�Z�[�W����͂��Ă�������\n");

	char message[32]; 

	scanf("%s", message);

	send(s, message, strlen(message), 0);
  	
  	//�W�����͂���̎�M���(message)��board�z��ɑ}������
	count = split(dst, message, ',');
	
	bx=atoi(dst[0]);
	by=atoi(dst[1]);
	
	if(bx==0||by==0){
		
	}else{
		if(port==12345){		//��������s�̎��A�����̎��1������
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


	// Windows �ł̃\�P�b�g�̏I��

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