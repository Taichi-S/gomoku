//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define BOARD_SQUARE 15

//�v���g�^�C�v�錾
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);

//�Ֆʂ̏����L�^���Ă����z��
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
//�œK�ȑł����T�����߂ɔՖʂ̃p�^�[���ɂ���čs���_���t�����L�^����ՖʁD�����ƓG��2�̃X�R�A�{�[�h��ێ�����D
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};

//��������s����U�����L�^����(��s(��)��1, ��U(��)��2)
int player_number;
int enemy_number;

//�����t���b�O(��������������true)
bool isWin;

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

	//�|�[�g�ԍ��ɂ����mySrtike��enemy_number��������
	if(port==12345){
		player_number = 1;
		enemy_number = 2;
	}else{
		player_number = 2;
		enemy_number = 1;
	}

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
		if(player_number==1){		//��������s�̎��A����̎��2������
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


		//�_���t�������Ֆʂ��o�͂���(����)
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
		if(player_number==1){		//��������s�̎��A����̎��2������
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

		//�_���t�������Ֆʂ��o�͂���(����)
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

//���u���I�������_���������Ȃ��Ƃ����񂩂��H�i��O�̎���g���Ȃ炢����)
//�Ֆʂ�S�ʒT�����āC�܂��΂��u����Ă��Ȃ��ꏊ�̓_���t��������(�_���͉��Z���Ă���)�D���̒l�́C�Q�����z��scoreBoard�ɋL�^����D
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[9];
	int cdIni[4]={
		{0, -4},	//�c
		{-4, 0},	//��
		{-4, -4},	//�E������΂�
		{4, -4}		//�E�オ��΂�
	}
	int cdGap[4]={
		{0, 1},		//�c
		{1, 0},		//��
		{1, 1},		//�E������΂�
		{-1, 1}		//�E�オ��΂�
	}
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sx][sy]==0){		//�΂�������Ă��Ȃ��Ƃ��A�S����������
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
					for(cb=0;cb<9;cb++){
						checkBoard[cb]=board[tempX][tempY];		//checkBoard�Ɉ�����̔Ղ̏����i�[
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					//�X�R�A�����Z���ĉ��̔ՖʂɊi�[����
					scoreBoard[sx][sy][player_number]+=boardScore(checkBoard[], player_number);
					scoreBoard[sx][sy][enemy_number]+=boardScore(checkBoard[], enemy_number);
				}
			}
		}
	}
}

//checkBoard�������ɂ�����A���S���Y���̎�Ɣ�ׂāA�_���t�������Ă����̓_�������v�����l��Ԃ�
int boardScore(int checkBoard[], int whichPlayer){
	char checkStr[];
	int bs;
	int tempScore=0;
	//��s�̃v���C���[(���C1)�̃A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)
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
	//��U�̃v���C���[(���C2)�̃A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)
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
		if(whichPlayer==1){		//��s�̃v���C���[�̓_����m�肽���Ƃ�
			if(strstr(checkStr, firstPatternEx[bs]!){	//�p�^�[�������݂���
				tempScore+=patternScore[firstPatternEx[bs][0]];
			}
		}
		else{		//��U�̃v���C���[�̓_����m�肽���Ƃ�
			if(strstr(checkStr, secondPatternEx[bs]!){	//�p�^�[�������݂���
				tempScore+=patternScore[secondPatternEx[bs][0]];
			}
		}
	}
	return tempScore;
}

//int�̔z���char�ɕϊ�����
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





//�������牺�͎g��Ȃ�(����)

//���u���I�������_���������Ȃ��Ƃ����񂩂��H�i��O�̎���g���Ȃ炢����)
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[4];
	int cdGap[8]={
		{-1, 0},	//��
		{-1, -1},	//����
		{0, -1},	//��
		{1, -1},
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}		//����
	}
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sx][sy]!=0){		//�΂�������Ă���B�����𒆐S�ɂW���ʂɂS�R�}���v�Z����
				for(cd=0;cd<8;cd++){
					for(cb=0;cb<4;cb++){
						checkBoard[cb]=board[sx+cdGap[cd][0]][sy+cdGap[cd][1]];		//checkBoard�Ɉ�����̔Ղ̏����i�[
					}
					boardScore(checkBoard[]);
				}
			}
		}
	}
}

//checkBoard�������ɂ�����A���S���Y���̎�Ɣ�ׂāA�_���t�������Ă��̓_����Ղɖ��ߍ���
void boardScore(int checkBoard[]){
	//�A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)
	int pattenEx[]={
		{1,1,1,1,100},
		{1,1,1,0,80},
		{1,1,1,2,70},
		{0,1,1,1,70},
		{1,0,1,1,70},
		{1,1,0,0,60},//����
		{1,1,0,1,60},
		
}

//2�̔z��̗v�f�����ׂĂ̈�v���Ă��邩���ׂ�
bool array_equal(const int* array1, const int* array2){	
	for (size_t i = 0; i < size1; ++i) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

//�����͎g��Ȃ���(����)