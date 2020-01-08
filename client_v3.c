//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define BOARD_SQUARE 15

//�v���g�^�C�v�錾
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);
void boardScoring();
int boardScore(int checkBoard[], int whichPlayer);
char convertInt(int array[]);
int forbidden_hand_judgement(int board[BOARD_SQUARE][BOARD_SQUARE])

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

		//�ւ��蔻��̃v���O����
		//int end_flag = 0;
		if(board[bx-1][by-1] == 1){ //�����s�ŁA���u������
			if(forbidden_hand_judgement(board) > 0){
				printf("It is forbidden_hand!");
				puts("I am Winner!!");
				while(1);
			} 
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

//�ւ��蔻��̊֐�
int forbidden_hand_judgement(int board[BOARD_SQUARE][BOARD_SQUARE]){
	//�O�O��
			char xsigns[8]="++0-";	//�Ώۍ��W���甪���ւ̑����ɕK�v�ȕ���
			char ysigns[8]="0+++";

			int ki = 0;
			int mj = 0;
			for(ki = 1; ki < BOARD_SQUARE + 1; ki++ ){

				for(mj = 1; mj < BOARD_SQUARE + 1; mj++ ){
					int san_counter = 0;	//�O�O�ւɂȂ肤��O�𐔂������
					int yon_counter = 0;	//�l�l�ւɂȂ肤��l�𐔂������
					int choren_flag = 0;

					if(board[mj-1][ki-1] == 1){	//�Ώۍ��W���A��s�̑��肪�u��������
						int ik = 0;
						for(ik = 0; ik < 4; ik ++){	//�Ώۍ��W���甪���i�SX�Q�j�𑖍�
							int xvector;	//�Ώۍ��W����i�ތ����������ɓ����
							int yvector;

							switch (xsigns[ik]){	//�Ώۍ��W����i�ތ���������
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
							int xweight = 0;	//�Ώۍ��W���i�ތ����Ƒ傫���������ɓ����
							int yweight = 0;
							int xmax = mj-1;	//�Ώۂ̍��W���܂ޒ�����ɂ���A����ɕK�v�ȍő�̍��W�ƍŏ��̍��W�������ɓ����
							int ymax = ki-1;
							int xmin = mj-1;
							int ymin = ki-1;
							int black_counter = 0;	//�Ώۂ̍��W���܂ޒ�����ɂ����U�̎�𐔂������
							int san_reach_flag = 0; //�O��������������_�ł��̃t���O���P�ɂ���
							int yon_reach_flag = 0;
							int white_in_plus_flag = 0;
							int white_in_minus_flag = 0;

							for(int jm = 0; jm < 4; jm++){ //�Ώۍ��W����O�X�ւƂȂ肤�钼����͈̔͂𑖍�
								xweight += xvector;	//�Ώۍ��W���i�ތ����Ƒ傫��������
								yweight += yvector;
	
								if(board[mj-1+xweight][ki-1+yweight] == 1 && white_in_plus_flag == 0){	//�i�񂾐�̍��W����U�i����j�̎�ł����-(*)
									black_counter++; //�Ώۂ̍��W���܂ޒ�����ɂ����U�̎���J�E���g
									xmax = mj-1+xweight; //���̎�i���W�j���ő���W�Ƃ��ĕۑ�
									ymax = ki-1+yweight;
								} else if(board[mj-1+xweight][ki-1+yweight] == 2){
									white_in_plus_flag = 1;
								}

								if(black_counter == 2 && san_reach_flag == 0){	//�Ώۍ��W�̑��ɒ�����ɂ����U�̎肪�Q�����
									san_counter++; //�O�X�ւƂȂ肤��O���J�E���g
									san_reach_flag = 1;//�O�t���O�𗧂Ă�
									//break;	//����̒����̑����͏I��
								}else if(black_counter == 3 && yon_reach_flag == 0){	//�Ώۍ��W�̑��ɒ�����ɂ����U�̎肪�Q�����
									yon_counter++; //�l�l�ւƂȂ肤��l���J�E���g
									yon_reach_flag = 1;//�l�t���O�𗧂Ă�
									//break;	//����̒����̑����͏I��
								}
							
								if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
									if(black_counter == 4){ //����̎肪�T����ł�����
										//���A�̉\������
									}
								}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
									if(black_counter == 5){ //����̎肪�U���񂾂�
										choren_flag = 1; //���A�t���O�𗧂Ă�
										break;
									}else{
										break; //�ւ���ƂȂ蓾�Ȃ��͈͂܂ő���������A����̒����̑����͏I��
									}
								} 

								if(board[mj-1-xweight][ki-1-yweight] == 1&& white_in_minus_flag == 0){ //�i*)�Ƃ͔��Ό����ɐi�񂾐�̍��W����U�i����j�̎�ł����
									black_counter++;	//�Ώۂ̍��W���܂ޒ�����ɂ����U�̎���J�E���g
									xmin = mj-1-xweight;	//���̎�i���W�j���ŏ����W�Ƃ��ĕۑ�
									ymin = ki-1-yweight;
								}else if(board[mj-1+xweight][ki-1+yweight] == 2){
									white_in_minus_flag = 1;
								}

								if(black_counter == 2 && san_reach_flag == 0){	//�Ώۍ��W�̑��ɒ�����ɂ����U�̎肪�Q�����
									san_counter++;	//�O�X�ւƂȂ肤��O���J�E���g
									san_reach_flag = 1;
									//break;	//����̒����̑����͏I��
								}else if(black_counter == 3 && yon_reach_flag == 0){	//�Ώۍ��W�̑��ɒ�����ɂ����U�̎肪�Q�����
									yon_counter++; //�l�l�ւƂȂ肤��l���J�E���g
									yon_reach_flag = 1;
									//break;	//����̒����̑����͏I��
								}

								if(abs(xmax - xmin) == 4 || abs(ymax - ymin) == 4){
									if(black_counter == 4){	//����̎肪�T����ł�����
										//���A�̉\��
									}
								}else if(abs(xmax - xmin) > 4 || abs(ymax - ymin) > 4){
									if(black_counter == 5){ //����̎肪�U���񂾂�
										choren_flag = 1; //���A�t���O�𗧂Ă�
										break;
									}else{
										break; //�O�ƂȂ蓾�Ȃ��͈͂܂ő���������A����̒����̑����͏I��
									}
								} 
							}
						
						}

						if(yon_counter == 2) {	//�l�l�ւ������
							puts("It is forbidden hand!:yonyonkin");
							//end_flag = 1;
							return 1;
							break;	//�ւ���̃v���O�������I��
						}else if(san_counter == 2 && yon_counter == 0) {	//�O�O�ւ������
							puts("It is forbidden hand!:sansankin");
							//end_flag = 1;
							return 1;
							break;	//�ւ���̃v���O�������I��
						}else if(choren_flag == 1){ //���A�����ꂠ��
							puts("It is forbidden hand!:choren");
							//end_flag = 1;
							return 1;
							break;	//�ւ���̃v���O�������I��
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