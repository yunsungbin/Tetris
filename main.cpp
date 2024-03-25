#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80 //sort drop
#define SPACE 32 //hard drop
#define p 112 //�Ͻ�����
#define P 80 //�Ͻ�����
#define ESC 27

#define ACTIVE_BLOCK -2 //����� ����� ����
#define CEILLING -1 //����� �̵��� �� �ִ� ����
#define EMPTY 0 //����� �̵��� �� ���� ����
#define WALL 1
#define COMPLETE_BLOCK 2 //�̵� �Ϸ�� ���

#define MAIN_X 11 //������ ����ũ�� 
#define MAIN_Y 23 //������ ����ũ�� 
#define MAIN_X_ADJ 3 //������ ��ġ���� 
#define MAIN_Y_ADJ 1 //������ ��ġ���� 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //��������ǥ�� ��ġ���� 

int STATUS_Y_GOAL;
int STATUS_Y_LEVEL;
int STATUS_Y_SCORE;

int main_org[MAIN_Y][MAIN_X]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� ����� 
int main_cpy[MAIN_Y][MAIN_X];

void title(void);
void reset(void); //������ �ʱ�ȭ
void reset_main(void);
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void);

int key; //Ű����� �Է¹��� Ű���� ����

int speed;
int level;
int level_goal;
int cnt; //���� �������� ������ �� ���� ���� 
int score;
int last_score = 0;
int best_score = 0;

int new_block_on = 0;
int crush_on = 0;
int level_up_on = 0;
int space_key_on = 0;

void gotoxy(int x, int y) {
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main() {
	title();
	reset();

	/*while (1) {
		for (int i = 0; i < 5; i++) {
			
		}
	}*/
}

void title(void) {
	//Ÿ��Ʋ ȭ���� ǥ�õǴ� ��ǥ
	int x = 5;
	int y = 4;
	//Ÿ��Ʋ �������� ��� ����
	int cnt;

	gotoxy(x, y + 0); printf("����������������"); Sleep(100);
	gotoxy(x, y + 1); printf("�����  ����  �����"); Sleep(100);
	gotoxy(x, y + 2); printf("�����       �����"); Sleep(100);
	gotoxy(x, y + 3); printf("����������������"); Sleep(100);
	gotoxy(x, y + 4); printf("����������������"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("TETRIS"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start");
	gotoxy(x, y + 9); printf("  ��   : Shift");
	gotoxy(x, y + 10); printf("��  �� : Left / Right");
	gotoxy(x, y + 11); printf("  ��   : Soft Drop");
	gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
	gotoxy(x, y + 13); printf("   P   : Pause");
	gotoxy(x, y + 14); printf("  ESC  : Quit");
	gotoxy(x, y + 16); printf("BONUS FOR HARD DROPS / COMBOS");

	for (cnt = 0;; cnt++) {
		if (_kbhit()) break;
		if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("��"); }
		if (cnt % 200 - 100 == 0) { gotoxy(x + 4, y + 1); printf(" "); }
		if (cnt % 350 == 0) { gotoxy(x + 13, y + 2); printf("��"); }
		if (cnt % 350 - 100 == 0) { gotoxy(x + 13, y + 2); printf(" "); }
		Sleep(100);
	}

	while(_kbhit()) _getch();
}

void reset(void) {
	FILE* file = fopen("score.dat", "rt"); //score.dat������ ����

	if (file == 0) { best_score = 0; }
	else {
		fscanf(file, "%d", &best_score);
		fclose(file);
	}

	level = 1;
	score = 0;
	level_goal = 1000;
	key = 0;
	crush_on = 0;
	cnt = 0;
	speed = 100;

	system("cls");
	reset_main();
	draw_map();
}

//������ �ʱ�ȭ
void reset_main(void) {
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (j = 1; j < MAIN_X; j++) {
		main_org[3][j] = CEILLING;
	}
	for (i = 1; i < MAIN_X; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) {
		main_org[MAIN_Y][j] = WALL;
	}
}

void reset_main_cpy(void) {
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = 100;
		}
	}
}

void draw_map(void) {
	int y = 3;

	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);
	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL : %5d", 10 - cnt);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+- N E X T -+");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|           |");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|           |");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|           |");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|           |");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- - - - --+");
	gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
	gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
	gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
	gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
	gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
	gotoxy(STATUS_X_ADJ, y + 15); printf("  ��  : Shift        SPACE : Hard Drop");
	gotoxy(STATUS_X_ADJ, y + 16); printf("��  �� : Left / Right   P   : Pause");
	gotoxy(STATUS_X_ADJ, y + 17); printf("  ��  : Sort Drop     ESC  : Quit");
}

void draw_main(void) {
	int i, j;

	for (j = 1; j < MAIN_X; j++) {
		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
	}

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) {
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);

				switch (main_org[i][j])
				{
				case EMPTY:
					printf("  ");
					break;
				case CEILLING:
					printf(". ");
					break;
				case WALL:
					printf("��");
				case COMPLETE_BLOCK:
					printf("��");
					break;
				case ACTIVE_BLOCK:
					printf("��");
					break;
				}
			}
		}
	}

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}