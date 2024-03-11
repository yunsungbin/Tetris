#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

void title(void); //게임시작화면
//void reset(void); //게임판 초기화

void gotoxy(int x, int y) {
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main() {
	title();
}

void title(void) {
	//타이틀 화면이 표시되는 좌표
	int x = 5;
	int y = 4;
	//타이틀 프레임을 재는 변수
	int cnt;

	gotoxy(x, y + 0); printf("■■■■■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 1); printf("■□□□  □□□  □□□■"); Sleep(100);
	gotoxy(x, y + 2); printf("■□□□       □□□■"); Sleep(100);
	gotoxy(x, y + 3); printf("■■■■□□□□□□□■■■■"); Sleep(100);
	gotoxy(x, y + 4); printf("■■■■■■■■■■■■■■■"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("TETRIS"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start");
	gotoxy(x, y + 9); printf("  △   : Shift");
	gotoxy(x, y + 10); printf("◁  ▷ : Left / Right");
	gotoxy(x, y + 11); printf("  ▽   : Soft Drop");
	gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
	gotoxy(x, y + 13); printf("   P   : Pause");
	gotoxy(x, y + 14); printf("  ESC  : Quit");
}