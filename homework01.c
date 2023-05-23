#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// 화면 구성을 위한 스트링 문자 배열 선언
//                		012345678901234567890123456789
const char TITLE_L1[] = "         GAME 2048          ";
const char TITLE_L2[] = "       1. Start             ";
const char TITLE_L3[] = "       2. How to Play       ";
const char TITLE_L4[] = "       3. Exit              ";
//                		012345678901234567890123456789
const char HELP_L1[] =  "       [ How to play ]      ";
const char HELP_L2[] =  "              ^             ";
const char HELP_L3[] =  "           <     >          ";
const char HELP_L4[] =  "              v             ";
const char HELP_L5[] =  "         control key        ";
const char HELP_L6[] =  " Press 'q' button to return ";

const char EXIT_L1[] =  "         GAME 2048          ";
const char EXIT_L2[] =  "  do you exit game (y/n)?   ";
const char EXIT_L3[] =  "       y. game exit         ";
const char EXIT_L4[] =  "       n. game return       ";

char screen[31*15];	// 전체화면 크기 31*15의 문자를 저장할 공간 선언

// 전체 화면 중 일부 영역에 문자열을 채우기 위한 함수
int writeStringToBuffer(const char* string, int x, int y)
{
	while(*string != '\0') //채울 문자열의 끝일 때까지 반복
	{
		screen[y*31+x] = *string++; x++;
	}

	return 0;
}

// 화면 외곽 틀에 해당하는 문자를 screen 배열 변수에 채우는 함수
void writeFrameToBuffer(void)
{
	int a=0;

	for(int y=0;y<15;y++)
	{
		for(int x=0;x<31;x++)
		{
			screen[a] = ' ';
			if( (y==0)||(y==14) )
			{
				if(x==30) screen[a] = '\n';
				else screen[a] = '=';
			}
			else
			{
				if((x==0)||(x==29)) screen[a] = '/';
				else if(x==30) screen[a] = '\n';
			}
			a++;
		}
	}
   screen[a] = '\0';
}

// 타이틀 화면 그리기 함수
void DrawTitleScreen(void)
{
	system("cls");
	writeFrameToBuffer();

	writeStringToBuffer(TITLE_L1,1,3);
	writeStringToBuffer(TITLE_L2,1,6);
	writeStringToBuffer(TITLE_L3,1,8);
	writeStringToBuffer(TITLE_L4,1,10);

	printf("%s",screen);
}

// 종료 화면 그리기 함수
void DrawExitScreen(void)
{
	system("cls");
	writeFrameToBuffer();

	writeStringToBuffer(EXIT_L1,1,3);
	writeStringToBuffer(EXIT_L2,1,6);
	writeStringToBuffer(EXIT_L3,1,8);
	writeStringToBuffer(EXIT_L4,1,10);

	printf("%s",screen);
}

// 게임 설명 화면 그리기 함수
void DrawHelpScreen(void)
{
	system("cls");
	writeFrameToBuffer();

	writeStringToBuffer(HELP_L1,1,2);
	writeStringToBuffer(HELP_L2,1,5);
	writeStringToBuffer(HELP_L3,1,6);
	writeStringToBuffer(HELP_L4,1,7);
	writeStringToBuffer(HELP_L5,1,9);
	writeStringToBuffer(HELP_L6,1,11);

	printf("%s",screen);
}

int main(void)
{
	char ckey;

	DrawTitleScreen(); // 타이틀 화면 그리기

	while(1)
	{
		if(_kbhit()) // 키보드가 눌렸는지 체크
		{
			ckey = _getch(); // 눌린 키값을 저장

			if(ckey == '1') // 타이틀 메뉴에서 '1' 이 눌렸을 때 처리
			{
				DrawTitleScreen();
			}
			else if(ckey == '2') // 타이틀 메뉴에서 '2' 이 눌렸을 때 처리
			{
				DrawHelpScreen(); // 게임설명 화면 그리기
				while(1)
				{
					if(_kbhit())
					{
						ckey = _getch();
						if(ckey == 'q') //'q'가 눌렸을 때 처리
						{
							DrawTitleScreen(); // 타이틀 화면을 그리기
							break; // 반복문 탈출
						}
					}
				}
			}
			else if(ckey == '3') // 타이틀 메뉴에서 '3' 이 눌렸을 때 처리
			{
				DrawExitScreen(); // 종료 화면 그리기
				while(1)
				{
					if(_kbhit())
					{
						ckey = _getch();
						if(ckey == 'y') //'y'가 눌렸을 때 처리
						{
							return(1); // 프로그램 완전 종료
						}
						else if(ckey == 'n') //'n'가 눌렸을 때 처리
						{
							DrawTitleScreen(); // 타이틀 화면을 그리기
							break; // 반복문 탈출
						}
					}
				}
			}
		}
	}


	return 0; // 프로그램 종료
}
