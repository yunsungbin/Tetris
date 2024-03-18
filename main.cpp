#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define CEILLING -1
#define WALL 1

#define MAIN_X 11 //������ ����ũ�� 
#define MAIN_Y 23 //������ ����ũ�� 
#define MAIN_X_ADJ 3 //������ ��ġ���� 
#define MAIN_Y_ADJ 1 //������ ��ġ���� 

int main_org[MAIN_Y][MAIN_X]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� ����� 
int main_cpy[MAIN_Y][MAIN_X];

void title(void);
void reset(void); //������ �ʱ�ȭ

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
}

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