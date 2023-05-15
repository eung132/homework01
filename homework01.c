#include <stdio.h>

//              012345678901234567890123456789  
char line1[] = "          GAME 2048         ";
char line2[] = "            V.0.1           ";
char line3[] = "       1. Start             ";
char line4[] = "       2. How to Play       ";
char line5[] = "       3. Exit              ";

void Draw_Title(void)
{
   char screen[465];
   int a = 0;
	int x = 0;
	int y = 0;

	for(y=0;y<15;y++)
	{
		for(x=0;x<30;x++)
		{
			screen[a] = ' ';
			if( (y==0)||(y==14) )
			{
				screen[a]='=';
			}
			else
			{
				if( ( (a%31) == 0) || ( (a%31) == 29) ) screen[a]='/';
				else if(x < 25)
				{
					if(y==2) screen[a] = line1[x];
					else if(y==4) screen[a] = line2[x];
					else if(y==7) screen[a] = line3[x];
					else if(y==9) screen[a] = line4[x];
					else if(y==11) screen[a] = line5[x];
				}
			}
			a++;
		}
        screen[a++]='\n';
	}
	
   
   screen[464]='\0';
   printf("%s",screen);
}

int main()
{
	Draw_Title();
    
	return 0;
}