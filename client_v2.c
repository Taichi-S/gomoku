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

							for(int jm = 0; jm < 4; jm++){ //�Ώۍ��W����O�X�ւƂȂ肤�钼����͈̔͂𑖍�
								xweight += xvector;	//�Ώۍ��W���i�ތ����Ƒ傫��������
								yweight += yvector;
	
								if(board[mj-1+xweight][ki-1+yweight] == 1){	//�i�񂾐�̍��W����U�i����j�̎�ł����-(*)
									black_counter++; //�Ώۂ̍��W���܂ޒ�����ɂ����U�̎���J�E���g
									xmax = mj-1+xweight; //���̎�i���W�j���ő���W�Ƃ��ĕۑ�
									ymax = ki-1+yweight;
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

								if(board[mj-1-xweight][ki-1-yweight] == 1){ //�i*)�Ƃ͔��Ό����ɐi�񂾐�̍��W����U�i����j�̎�ł����
									black_counter++;	//�Ώۂ̍��W���܂ޒ�����ɂ����U�̎���J�E���g
									xmin = mj-1-xweight;	//���̎�i���W�j���ŏ����W�Ƃ��ĕۑ�
									ymin = ki-1-yweight;
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

		//�ւ��蔻��̃v���O����
		int end_flag = 0;
		if(board[bx-1][by-1] == 1){ //�����s�ŁA���u������
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