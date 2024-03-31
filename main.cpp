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
#define p 112 //일시정지
#define P 80 //일시정지
#define ESC 27

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 //저장될 블록의 상태
#define CEILLING -1 //블록이 이동할 수 있는 공간
#define EMPTY 0 //블록이 이동할 수 없는 공간
#define WALL 1
#define COMPLETE_BLOCK 2 //이동 완료된 블록

#define MAIN_X 11 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 1 //게임판 위치조정 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 

int STATUS_Y_GOAL;
int STATUS_Y_LEVEL;
int STATUS_Y_SCORE;

int blocks[7][4][4][4] = {
	{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}, {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
	{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}, {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
	{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0}, {0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
	{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0}, {0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
	{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0}, {0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
	{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0}, {0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
	{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0}, {0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0}, {0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
	{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0}, {0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0}, {0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
	{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0}, {0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0}, {0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
};

int main_org[MAIN_Y][MAIN_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨 
int main_cpy[MAIN_Y][MAIN_X];

void title(void);
void reset(void); //게임판 초기화
void reset_main(void);
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void);
void new_block(void);
void Check_Key(void);
int check_crush(int bx, int by, int b_rotation);
void drop_block(void);
void move_block(int dir);
void check_line(void);
void check_level_up(void);
void check_game_over(void);
void pause(void);

int b_type; //블록의 종류
int b_rotation; //블록 회전
int b_type_next; //다음 블록

int bx, by; //이동중익 블록 좌표 저장

int key; //키보드로 입력받은 키값을 저장

int speed;
int level;
int level_goal;
int cnt; //현재 레벨에서 제거한 줄 수를 저장 
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

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c) { //커서숨기기 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
	int i;

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);

	title();
	reset();

	while (1) {
		for (i = 0; i < 5; i++) {
			Check_Key();
			draw_main();
			Sleep(speed);
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);

			if (space_key_on == 1) {
				space_key_on = 0;
				break;
			}
		}

		drop_block();
		check_level_up();
		check_game_over();
		if (new_block_on == 1) new_block();
	}
}

void title(void) {
	//타이틀 화면이 표시되는 좌표
	int x = 5;
	int y = 4;
	//타이틀 프레임을 재는 변수
	int cnt;

	gotoxy(x, y + 0); printf("■■■■■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 1); printf("■□□□    □□□    □□□■"); Sleep(100);
	gotoxy(x, y + 2); printf("■□□□              □□□■"); Sleep(100);
	gotoxy(x, y + 3); printf("■■■■□□□□□□□■■■■"); Sleep(100);
	gotoxy(x, y + 4); printf("■■■■■■■■■■■■■■■"); Sleep(100);
	gotoxy(x + 6, y + 2); printf("TETRIS"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start");
	gotoxy(x, y + 9); printf("  △   : Shift");
	gotoxy(x, y + 10); printf("◁  ▷ : Left / Right");
	gotoxy(x, y + 11); printf("  ▽   : Soft Drop");
	gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
	gotoxy(x, y + 13); printf("   P   : Pause");
	gotoxy(x, y + 14); printf("  ESC  : Quit");
	gotoxy(x, y + 16); printf("BONUS FOR HARD DROPS / COMBOS");

	for (cnt = 0;; cnt++) {
		if (_kbhit()) break;
		if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("☆"); }
		if (cnt % 200 - 100 == 0) { gotoxy(x + 4, y + 1); printf(" "); }
		if (cnt % 350 == 0) { gotoxy(x + 10, y + 1); printf("☆"); }
		if (cnt % 350 - 100 == 0) { gotoxy(x + 13, y + 2); printf(" "); }
		Sleep(100);
	}

	while(_kbhit()) _getch();
}

void reset(void) {
	FILE* file = fopen("score.dat", "rt"); //score.dat파일을 연결

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
	draw_main();

	b_type_next = rand() % 7;
	new_block();
}

//게임판 초기화
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
	for (i = 1; i < MAIN_X + 11; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) {
		main_org[MAIN_Y - 1][j] = WALL;
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
	gotoxy(STATUS_X_ADJ, y + 15); printf("  △  : Shift        SPACE : Hard Drop");
	gotoxy(STATUS_X_ADJ, y + 16); printf("◁  ▷ : Left / Right   P   : Pause");
	gotoxy(STATUS_X_ADJ, y + 17); printf("  ▽  : Sort Drop     ESC  : Quit");
}

void draw_main(void) {
	int i, j;

	for (j = 1; j < MAIN_X-1; j++) {
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
					printf("▩");
				case COMPLETE_BLOCK:
					printf("□");
					break;
				case ACTIVE_BLOCK:
					printf("■");
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

void new_block(void) {
	int i, j;

	bx = (MAIN_X / 2) - 1; //게임판 가운데
	by = 0; //블록 생성 위치
	b_type = b_type_next;
	b_type_next = rand() % 7;
	b_rotation = 0;

	new_block_on = 0;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j];
		}
	}

	for (i = 1; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf(" ");
			}
		}
	}
}

void Check_Key(void) {
	key = 0;

	if (_kbhit()) {
		key = _getch();
		if (key == 224) {
			do { key = _getch(); } while (key == 224);
			switch (key) {
			case LEFT:
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case RIGHT:
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN:
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP:
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
				break;
			}
		}
		else {
			switch (key)
			{
			case SPACE:
				space_key_on = 1;
				while (crush_on == 0) {
					drop_block();
					score += level;
					gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
				}
				break;
			case p:
			case P:
				pause();
				break;
			case ESC:
				system("cls");
				exit(0); //게임종료
			}
		}
	}

	while (_kbhit()) _getch();
}

int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
		}
	}
	return true;
}

void drop_block(void) {
	int i, j;

	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i < MAIN_Y; i++) {
			for (j = 0; j < MAIN_X; j++) {
				if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = COMPLETE_BLOCK;
			}
		}

		crush_on = 0;
		check_line();
		new_block_on = 1;
		return;
	}

	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}

void move_block(int dir) {
	int i, j;

	switch (dir)
	{
	case LEFT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--;
		break;
	case RIGHT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;
	case DOWN:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;
	case UP:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}

		b_rotation = (b_rotation + 1) % 4;

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;
	case 100:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}

void check_line(void) {
	int i, j, k, l;

	int block_amount; //블록갯수 저장
	int combo = 0;

	for (i = MAIN_Y - 2; i > 3;) {
		block_amount = 0;
		for (j = 1; j < MAIN_X - 1; j++) {
			if (main_org[i][j] > 0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) {
			if (level_up_on == 0) {
				score += 100 * level;
				cnt++;
				combo++;
			}
			for (k = i; k > 1; k--) {
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
				}
			}
		}
		else i--;
	}

	if (combo) {
		if (combo > 1) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO", combo);

			Sleep(500);
			score += (combo * level * 100);
			reset_main_cpy();
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("     %6d", score);
	}
}

void check_level_up(void) {
	int i, j;

	if(cnt >= 10) {
		draw_main();
		level_up_on = 1;
		level += 1;
		cnt = 0;

		for (i = 0; i < 4; i++) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("             ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("             ");
			Sleep(200);

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("★LEVEL UP!★");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("★SPEED UP!★");
			Sleep(200);
		}
		reset_main_cpy();

		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) {
			for (j = 1; j < MAIN_X - 1; j++) {
				main_org[i][j] = COMPLETE_BLOCK;
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				printf("★");
				Sleep(20);
			}
		}
		Sleep(100);
		check_line();

		switch (level) {
		case 2:
			speed = 50;
			break;
		case 3:
			speed = 25;
			break;
		case 4:
			speed = 10;
			break;
		case 5:
			speed = 5;
			break;
		case 6:
			speed = 4;
			break;
		case 7:
			speed = 3;
			break;
		case 8:
			speed = 2;
			break;
		case 9:
			speed = 1;
			break;
		case 10:
			speed = 0;
			break;
		}
		level_up_on = 0;

		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level);
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL : %5d", 10 - cnt);
	}
}

void check_game_over(void) {
	int i;

	int x = 30;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) {
		if (main_org[3][i] > 0) {
			gotoxy(x, y + 0); printf("■■■■■■■■■■■■■■■■■");
			gotoxy(x, y + 1); printf("■                              ■");
			gotoxy(x, y + 2); printf("■   +-----------------------+  ■");
			gotoxy(x, y + 3); printf("■       | G A M E O V E R |    ■");
			gotoxy(x, y + 4); printf("■   +-----------------------+  ■");
			gotoxy(x, y + 5); printf("■       YOUR SCORE %6d      ■", score);
			gotoxy(x, y + 6); printf("■                              ■");
			gotoxy(x, y + 7); printf("■   Press any key to restart   ■");
			gotoxy(x, y + 8); printf("■                              ■");
			gotoxy(x, y + 9); printf("■■■■■■■■■■■■■■■■■");
			last_score = score;

			if (score > best_score) {
				FILE* file = fopen("score.dat", "wt");

				gotoxy(x, y + 6); printf("★ ★ ★ BEST SCORE ★ ★ ★");

				if (file == 0) {
					gotoxy(0, 0);
					printf("FILLE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
				}
				else {
					fprintf(file, "%d", score);
					fclose(file);
				}
			}
			Sleep(100);
			while (_kbhit()) _getch();
			key = _getch();
			reset();
		}
	}
}

void pause(void) {
	int i, j;

	int x = 30;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) {
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤                              ▤");
		gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
		gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
		gotoxy(x, y + 6); printf("▤                              ▤");
		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}
	_getch();

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	for (i = 1; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}