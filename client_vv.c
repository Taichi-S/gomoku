// #include "winsock2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SQUARE 15

//�v���g�^�C�v�錾
int isDelimiter(char p, char delim);
int split(char *dst[], char *src, char delim);
void boardScoring();
int matchingScore(int *checkBoard, int whichPlayer);

//��s�̃v���C���[(���C1)�̃A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)
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
//��U�̃v���C���[(���C2)�̃A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)
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

//�Ֆʂ̏����L�^���Ă����z��
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
//�œK�ȑł����T�����߂ɔՖʂ̃p�^�[���ɂ���čs���_���t�����L�^����ՖʁD�����ƓG��2�̃X�R�A�{�[�h��ێ�����D
//��s��0�ԖڂɁC��U��1�ԖڂɊi�[����
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};

//��������s����U�����L�^����(��s(��)��1, ��U(��)��2)
int player_number=1;
int enemy_number=2;

//�����t���b�O(��������������1)
int isWin=0;

int main(void) {
    int i,j;
    //�Ֆʂ�0,1,2�Ń����_���ɖ��߂�
    for(i = 0; i < BOARD_SQUARE; i++ ){
        for(j=0;j<BOARD_SQUARE; j++){
            board[i][j]=rand() % 3;     //0,1,2 �̗����𔭐�
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

    //�_���t�������Ֆʂ��o�͂���(����)
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

    //�_���t�������Ֆʂ��o�͂���(����)
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

//���u���I�������_���������Ȃ��Ƃ����񂩂��H�i��O�̎���g���Ȃ炢����)
//�Ֆʂ�S�ʒT�����āC�܂��΂��u����Ă��Ȃ��ꏊ�̓_���t��������(�_���͉��Z���Ă���)�D���̒l�́C3�����z��scoreBoard�ɋL�^����D
void boardScoring(){
	int sx,sy;
	int cd, cb;
	int checkBoard[9];
	int cdIni[4][2]={
		{0, -4},	//�c
		{-4, 0},	//��
		{-4, -4},	//�E������΂�
		{4, -4}		//�E�オ��΂�
	};
	int cdGap[4][2]={
		{0, 1},		//�c
		{1, 0},		//��
		{1, 1},		//�E������΂�
		{-1, 1}		//�E�オ��΂�
	};
	for(sy=0;sy<BOARD_SQUARE;sy++){
		for(sx=0;sx<BOARD_SQUARE;sx++){
			if(board[sy][sx]==0){		//�΂�������Ă��Ȃ��Ƃ��A�S����������
				for(cd=0;cd<4;cd++){
					int tempX=sx+cdIni[cd][0];
					int tempY=sy+cdIni[cd][1];
					for(cb=0;cb<9;cb++){
						checkBoard[cb]=board[tempY][tempX];		//checkBoard�Ɉ�����̔Ղ̏����i�[
						tempX+=cdGap[cd][0];
						tempY+=cdGap[cd][1];
					}
					//�X�R�A�����Z���ĉ��̔ՖʂɊi�[����
					scoreBoard[sy][sx][player_number-1]+=matchingScore(checkBoard, player_number);
					scoreBoard[sy][sx][enemy_number-1]+=matchingScore(checkBoard, enemy_number);
				}
			}
		}
	}
}

//�}�b�`���O�������ǂ���+�}�b�`���O�����p�^�[���ƑΏ۔z���0����v���Ă��邩+����ɂ��_�������_����
//�ꏊ�ˑ��₩��C�}�b�`���O�����̃��\�b�h�ɂ͂ł��Ȃ�
//1�̕���(�v�f��9�̈ꎟ�z��)�ɑ΂��āC35��ޑS�Ẵp�^�[����T������
int matchingScore(int *checkBoard, int whichPlayer){
	int mi, mj;
	int ms;
	int sumScore=0;
	if(whichPlayer==1){
		for(ms=0;ms<35;ms++){
            int lenPEX = sizeof firstPatternEx[ms] / sizeof firstPatternEx[ms][0];
            int pNum = lenPEX-1;
            int mNum=10-pNum;
			for(mi=0;mi<mNum;mi++){
				//���Ă�p�^�[���̐^�񒆂�0�ł͂Ȃ������Z���Ȃ������΂�
				if(firstPatternEx[ms][5-mi]!=0){
					continue;
				}
				for(mj=0;mj<pNum;mj++){
					if(checkBoard[mi+mj]!=firstPatternEx[ms][mj+1]){
						continue;
					}
				}//�Ō�܂Ń~�X�Ȃ�
				sumScore+=patternScore[firstPatternEx[ms][0]];
			}
		}
	}
	else{
		for(ms=0;ms<35;ms++){
            int lenPEX = sizeof secondPatternEx[ms] / sizeof secondPatternEx[ms][0];
            int pNum = lenPEX-1;
            int mNum=10-pNum;
			for(mi=0;mi<mNum;mi++){
				//���Ă�p�^�[���̐^�񒆂�0�ł͂Ȃ������Z���Ȃ������΂�
				if(secondPatternEx[ms][5-mi]!=0){
					continue;
				}
				for(mj=0;mj<pNum;mj++){
					if(checkBoard[mi+mj]!=secondPatternEx[ms][mj+1]){
						continue;
					}
				}//�Ō�܂Ń~�X�Ȃ�
				sumScore+=patternScore[secondPatternEx[ms][0]];
			}
		}
	}
	return sumScore;
}