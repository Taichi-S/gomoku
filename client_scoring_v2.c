// #include "winsock2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SQUARE 15

//�v���g�^�C�v�錾

void boardScoring();
int matchingScore(int *checkBoard, int whichPlayer, int putPlayer);
void output_array(int *in, int len);
void printScoreBoard(int whichPlayer);
void printBoard(void);
//��s�̃v���C���[(���C1)�̃A���S���Y���̎�����ׂė��񂷂�(�����̋t���܂�)

// int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};
int board[BOARD_SQUARE][BOARD_SQUARE]={
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,2,2,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,2,2,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
//�œK�ȑł����T�����߂ɔՖʂ̃p�^�[���ɂ���čs���_���t�����L�^����ՖʁD�����ƓG��2�̃X�R�A�{�[�h��ێ�����D
//��s��0�ԖڂɁC��U��1�ԖڂɊi�[����
int scoreBoard[BOARD_SQUARE][BOARD_SQUARE][2] = {{0}};

//��������s����U�����L�^����(��s(��)��1, ��U(��)��2)
int player_number=1;
int enemy_number=2;

//�����t���b�O(��������������1)
int isWin=0;

int main(void) {
    printBoard();

    boardScoring();

    //�_���t�������Ֆʂ��o�͂���(����)


    printScoreBoard(1);
    printScoreBoard(2);
	return 0;

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
					//�X�R�A�����Z���ĉ��̔ՖʂɊi�[����
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

//�}�b�`���O�������ǂ���+�}�b�`���O�����p�^�[���ƑΏ۔z���0����v���Ă��邩+����ɂ��_�������_����
//�ꏊ�ˑ��₩��C�}�b�`���O�����̃��\�b�h�ɂ͂ł��Ȃ�
//1�̕���(�v�f��9�̈ꎟ�z��)�ɑ΂��āC35��ޑS�Ẵp�^�[����T������
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