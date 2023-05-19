#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

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
const char HELP_L6[] =  " Press space bar to return! ";

const char EXIT_L1[] =  "         GAME 2048          ";
const char EXIT_L2[] =  "  do you exit game (y/n)?   ";
const char EXIT_L3[] =  "       y. game exit         ";
const char EXIT_L4[] =  "       n. game return       ";

// const char *TITLE[] = "          GAME 2048         ";

char screen[465];

int writeStringToBuffer(const char* string, int x, int y)
{
#if 1
	while(*string != '\0')
	{
		screen[y*31+x] = *string++; x++;
	}
#endif

	return 0;
}

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

	DrawTitleScreen();

	while(1)
	{
		if(_kbhit())
		{
			ckey = _getch();
			
			if(ckey == '1')
			{
				DrawTitleScreen();
			}
			else if(ckey == '2')
			{
				DrawHelpScreen();
				while(1)
				{
					if(_kbhit())
					{
						ckey = _getch();
						if(ckey == ' ')
						{
							DrawTitleScreen();
							break;
						}
					}
				}
			}
			else if(ckey == '3')
			{
				DrawExitScreen();
				while(1)
				{
					if(_kbhit())
					{
						ckey = _getch();
						if(ckey == 'y')
						{
							return(1);
						}
						else if(ckey == 'n')
						{
							DrawTitleScreen();
							break;
						}
					}
				}
			}
		}
	}
	
	
	return 0;
}
