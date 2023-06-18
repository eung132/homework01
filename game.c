#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

 
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
 
#define MAP_X 10
#define MAP_Y 5
 
 
#define BLACK             (0x0000)        // 검정색
#define DARK_BLUE         (0x0001)        // 파란색
#define GREEN             (0x0002)        // 녹색
#define BLUE_GREEN        (0x0003)        // 청녹색
#define BLOOD             (0x0004)        // 검붉은색
#define PURPLE            (0x0005)        // 보라색
#define GOLD              (0x0006)        // 금색
#define ORIGINAL          (0x0007)        // 밝은 회색 (ORIGINAL CONSOLE COLOR)
#define GRAY              (0x0008)        // 회색
#define BLUE              (0x0009)        // 파란색
#define HIGH_GREEN        (0x000a)        // 연두색
#define SKY_BLUE          (0x000b)        // 하늘색
#define RED               (0x000c)        // 빨간색
#define PLUM              (0x000d)        // 자주색
#define YELLOW            (0x000e)        // 노란색
#define WHITE             (0x000f)        // 흰색

#define COMBINED_NUM		0x1000 //0001 0000 0000 0000

void cursor_position(int x,int y) { //모니터상의 x,y 좌표로 이동시키는 함수 (사용자 정의 함수?)
    COORD cur={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur);
}
 
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // gcc 표준 함수
void  cursor_type(CURSOR_TYPE c){ //커서 표시를 바꿔주는 함수  
     CONSOLE_CURSOR_INFO cursorInfo;
 
     switch (c) {
     case NOCURSOR:
          // cursorInfo.dwSize=1 <-- 필요없졍
          cursorInfo.bVisible=FALSE; // 0이면 커서 안 보임! 0 제외 다른 수는 커서가 보인대영 FALSE, TRUE 해도 되긴 함..
          break;
     case NORMALCURSOR:
          cursorInfo.dwSize=20;
          cursorInfo.bVisible=TRUE;
          break;
     }
     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
}
 
int board[4][4]={ // 게임판 
{0,0,0,0},
{0,0,0,0},
{0,0,0,0},
{0,0,0,0}
};
 
int game_over_on=0; //게임 오버 스위치 
int score; //점수 저장함수 
 
void reset(void); //게임을 초기화 
void draw(void); // 게임판을 그림 
void new_num(void); // 새로운 숫자 2나 4를 게임판에 만듦 
int check_key(void); // 키입력받고 움직이는 함수 
void check_game_over(void); // 게임오버를 판단하는 함수 
 
int game_exit = 0;
int draw_count=0;
 

int game(void){
	
    reset(); //게임을 초기화 
 
    while(1){
        if(check_key()){ //키 입력후 움직임이 있는 경우에만 
            new_num();    //새로운 숫자를 만듦 
        } 
        check_game_over(); // 게임오버판단 
		if(game_exit) break;;
    }
	
	return 1;
}
 
void reset(void){
    int y, x; // for문을 돌리는 변수들 
    
    system("chcp 65001"); //UTF-8 인코딩(한글/특수문자 표현을 위해 변경)
    system("cls"); //화면을 지움 
    
    for(y=0;y<4;y++) for(x=0;x<4;x++) board[y][x]=0; //게임판을 초기화 
 
    //각종변수 초기화 
    score=0;
    game_over_on=0;
    
	game_exit = 0;
	draw_count=0;;
	
    //배경을 그림     
    cursor_position(MAP_X-2,MAP_Y-3);
    printf("♪ 2048 GAME ♪");
    cursor_position(MAP_X-2,MAP_Y-1);
    printf("===========================");
	for(y=0;y<12;y++){
		cursor_position(MAP_X-2,MAP_Y+y);
		printf("/                         /");
	}
    cursor_position(MAP_X-2,MAP_Y+y);
    printf("===========================");    
    cursor_position(MAP_X-2,MAP_Y+y+2);
    printf("◇ ESC : Quit");
    
    //새로운 숫자를 2개 만듦 
    new_num();
    new_num();
}
 
int check_move(int curr_y, int curr_x, int next_y, int next_x){
	int curr_sel, next_sel;
	
	curr_sel=board[curr_y][curr_x];
	next_sel=board[next_y][next_x];
	
	if(curr_sel==0||curr_sel>COMBINED_NUM) return 0; //자기 값이 0 혹은 COMBINED_NUM보다 큰 경우,
	if(next_sel!=0&&next_sel!=curr_sel) return 0;
	if(next_sel==0) next_sel=curr_sel; //자기 왼쪽값이 0인경우 자기값 복사 
	else if(curr_sel==next_sel){ //자기 왼쪽값과 동일한 경우에 
		next_sel*=2; //왼쪽값을 2배로(자기 값을 더하는것과 같음) 
		next_sel+=COMBINED_NUM; // 임시로 값증가 함수 마지막에 다시 값을 감소시킴 
		/* COMBINED_NUM 안더하면 만약 한줄에 값이 2,2,2,2 인 경우 왼쪽키를 누르면 한번에       */
		/* 8,0,0,0으로 바뀜.. COMBINED_NUM을 더하고, 값이 COMBINED_NUM보다 큰 경우 연산을 하지 않게 하여*/
		/* 2,2,2,2 에서 4,4,0,0으로 바뀜 */
		score+=2*curr_sel; //점수를 증가     
	}
	board[curr_y][curr_x]=0;
	board[next_y][next_x]=next_sel;
	
	draw(); // 숫자 이동(애니메이션 효과) 
	return 1;
}	

int check_key(void){ // 게임 내의 조작키값을 검사하는 함수
    int y, x;
    
    int key=0; //입력받은 키를 저장하는 변수 
    int act=0; //입력받은 키가 유효한 작용을 했을때 함수가 값을 return하는 것을 돕는 변수 
    int mov=-1; //입력받은 키가 유효한 작용을 했을때 반복문을 탈출할수 있게 하는 변수 
 
    key=getch(); //키입력받음 
    if(key==ESC){
	    game_exit=1; // ESC키를 누르면 게임종료
    }
    if(key==0xE0 || key==0){ // 방향키가 눌린 경우 1바이트 값을 추가로 읽음
        key=getch();
        switch(key){
            case LEFT:     //왼쪽방향키를 누른경우 
				while(mov!=0){     //함수 실행시 mov값이 -1이므로 처음엔 반드시 실행되고 실행후 0으로 변경, 
					mov=0;         //이후 게임판에 변화가 있는 경우에 수치 증가하여 게임판에 변화가 없을때까지 반복 
					for(x=1;x<=3;x++){  //가장 왼쪽열(0번째열)은 검사할 필요가 없으므로 1~3번열을 순서대로 검사 
						for(y=0;y<4;y++){ //모든 행을 검사  
							if(check_move(y,x,y,x-1)==0) continue;
							act++; // action이 있음을 알림(check_key 함수 종료시 판단됨) 
							mov++; // mov가 있음을 알림(while 문안에서만 사용되고 리셋됨)
						}   
					}       
				}                      
            break;
 
            case RIGHT: // 오른쪽 방향키를 누른경우. 작동원리는 동일 좌우만 바뀜 
				while(mov!=0){
					mov=0;
					for(x=2;x>=0;x--){ //2번열, 1번열, 0번열을 순서대로 검사. 
						for(y=0;y<4;y++){  // 모든행을 검사
							if(check_move(y,x,y,x+1)==0) continue;
							act++;
							mov++;
						}    
					}
				}
            break;
 
            case UP: // 위쪽 방향키를 누른경우. 작동원리는 동일 
				while(mov!=0){
					mov=0;
					for(y=1;y<=3;y++){ //1번행, 2번행, 3번행을 순서대로 검사 
						for(x=0;x<4;x++){ //모든 열을 검사 
							if(check_move(y,x,y-1,x)==0) continue;
							act++;
							mov++;
						}        
					}
				}
            break;
 
            case DOWN: // 아래쪽 방향키를 누른경우. 작동원리는 동일
				while(mov!=0){
					mov=0;
					for(y=2;y>=0;y--){ //2번행, 1번행, 0번행을 순서대로 검사  
						for(x=0;x<4;x++){ //모든 열을 검사  
							if(check_move(y,x,y+1,x)==0) continue;
							act++; 
							mov++; 
						}        
					}
				}
            break;
        }
    }
    
    for(y=0;y<4;y++){ //임시로 증가시켰던 COMBINED_NUM을 빼줌. 
        for(x=0;x<4;x++){
            if(board[y][x]>COMBINED_NUM)board[y][x]-=COMBINED_NUM;
        }
    }
	
    if(act==0) return 0; //만약 키입력후 변화가 없으면 0을 return 
    else return 1;  //키입력후 변화가 있으면 1을 return 
}
 
void draw(){
    int x,y,k; // for문을 돌리기 위한 변수 
    int num; // 게임판의 값을 화면에 칸을 표시하기 위한 색깔을 검사하게 되는데 

	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			if(board[y][x]==0){ 
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), GRAY);
                cursor_position(MAP_X+(x*6),MAP_Y+(y*3)+1);
                printf("  ＊  ");
            }
            else{
                num=((board[y][x]>COMBINED_NUM)?board[y][x]-COMBINED_NUM:board[y][x]); //COMBINED_NUM이 더해진 경우 보정 
                if(game_over_on==1) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), GRAY);
				else { //게임오버인 경우에는 회색으로 
					
					if(num == 2) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), GREEN); 
					else if(num == 4) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), SKY_BLUE); 
					else if(num == 8) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BLOOD); 
					else if(num == 16) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), YELLOW); 
					else if(num == 32) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), PLUM); 
					else if(num == 64) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), WHITE); 
					else if(num == 128) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BLUE); 
					else if(num == 256) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), DARK_BLUE); 
					else if(num == 512) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), PURPLE); 
					else if(num == 1024) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BLUE_GREEN); 
					else if(num == 2048) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), GOLD); 
					
						
					
				}
				cursor_position(MAP_X+(x*6),MAP_Y+(y*3)+1);
				if(num > 99) printf(" %d ", num);
				else printf("  %d  ", num);
				if(game_over_on==1) Sleep(200);  //게임오버인 경우 한칸 그리고 0.2초의 딜레이 
			}
		}
	}
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), ORIGINAL); //원래 색깔로 고침 
                    
    cursor_position(MAP_X+28,MAP_Y+11);
    printf("Score : %6d \n", score); //점수표시 
    Sleep(50);
	
    cursor_position(MAP_X-2,MAP_Y+20);

}
 
void new_num(){
    int y, x; //for문을 돌리는 변수
     
    int count; //게임판에 빈칸을 세어 저장하는 변수 
    int random; //난수표에 의해 생성된 값을 저장하는 변수 
    int n_num; // 새로운 숫자 2, 4를 저장하는 변수 
    int* p0[16]={0,}; //빈칸의 위치를 기억할 포인터 배열 
    
    for(y=0,count=0;y<4;y++){ //전체 게임판을 검사
        for(x=0;x<4;x++){
            if(board[y][x]==0){ //빈칸이 있으면 
                p0[count++]=&board[y][x]; //그 위치를 포인터 배열에 저장, count값 증가 
            }
        }
    }
 
    srand(time(0)); //난수 생성 
    random=rand()%(count); //포인터 배열count수보다 작은 값으로 저장. 
    n_num=(rand()%100<80)?2:4; //80%확률로 2을 만들고, 20%확률로 4를 만듦 
    
    *p0[random]=n_num; //빈칸에 수를 넣음 
    draw();            
    *p0[random]=0;        //애니메이션 효과 두번깜박임 
    draw();
    *p0[random]=n_num;
    draw();
}

void new_record_message(int num, int color, char * msg) {
	for(int k=0;k<3;k++){    
		cursor_position(MAP_X+28,MAP_Y);
		printf("                                        ");
		Sleep(100);
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color);
		cursor_position(MAP_X+28,MAP_Y);
		if(num < 2048) printf("You made %d : ",num);
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), ORIGINAL);
		printf("%s", msg);
		Sleep(100);
	}
}
	
	

 
void check_game_over(void){ //게임오버인지 체크 
    int y,x;

	int temp=0;
	
    for(y=0;y<4;y++) for(x=0;x<4;x++) if(board[y][x]==0) temp=1; //일단 빈칸이 있으면 게임오버가 아님 
    for(y=0;y<3;y++) for(x=0;x<3;x++) if(board[y][x]==board[y+1][x]||board[y][x]==board[y][x+1]) temp=1; //return;
	//0,0부터 2,2까지 오른쪽과 아래를 검사 
    for(y=0;y<3;y++) if(board[y][3]==board[y+1][3]) temp=1; //0~2번행은 아래만 검사(오른쪽값이 없으므로) 
    for(x=0;x<3;x++) if(board[3][x]==board[3][x+1]) temp=1; //0~2번열은 오른쪽만 검사(아래쪽값이 없으므로) 
    
    for(y=0;y<4;y++) for(x=0;x<4;x++) if(board[y][x]==2048) temp=2;
	if(temp==1) return;
	
    game_over_on=1;    //게임오버 flag를 on. draw함수에서 애니메이션 효과를 주기 위함 
    
    cursor_position(MAP_X+28,MAP_Y);
    if(temp==0) printf("Game Over..                             ");
    else if(temp==2) printf("\033[0;33m축하합니다 2048을 만드셨어요!! 나가주세요.                          ");
    cursor_position(MAP_X+28,MAP_Y+2);
    printf("                                        ");
    draw();
    cursor_position(MAP_X+28,MAP_Y+2);
    printf("Restart? (Y/N)                     "); // 게임을 다시할건지 물어봄 
    
    while(1){
        switch(getch()){ // 컴파일러에 포함되어 있는 라이브러리 함수 : 표준 함수
            case 'Y': 
            case 'y': 
                reset(); //게임을 초기화  
                break;
            case 'N': 
            case 'n': 
            case ESC:
				game_exit=1;
        }
        break;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
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
const char HELP_L5[] =  "          move key          ";
const char HELP_L6[] =  "         ESC : Quit         ";

const char EXIT_L1[] =  "         GAME 2048          ";
const char EXIT_L2[] =  "  do you exit game (y/n)?   ";
const char EXIT_L3[] =  "       y. game exit         ";
const char EXIT_L4[] =  "       n. game return       ";

char screen[31*15];	// 전체화면 크기 31*15의 문자를 저장할 공간 선언

// 전체 화면 중 일부 영역에 문자열을 채우기 위한 함수
int write_string_to_buffer(const char* string, int x, int y)
{
	while(*string != '\0') //채울 문자열의 끝일 때까지 반복
	{
		screen[y*31+x] = *string++; x++;
	}

	return 0;
}

// 화면 외곽 틀에 해당하는 문자를 screen 배열 변수에 채우는 함수
void write_frame_to_buffer(void)
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
void draw_title_screen(void)
{
	system("cls");
	write_frame_to_buffer();

	write_string_to_buffer(TITLE_L1,1,3);
	write_string_to_buffer(TITLE_L2,1,6);
	write_string_to_buffer(TITLE_L3,1,8);
	write_string_to_buffer(TITLE_L4,1,10);

	printf("%s",screen);
}

// 종료 화면 그리기 함수
void draw_exit_screen(void)
{
	system("cls");
	write_frame_to_buffer();

	write_string_to_buffer(EXIT_L1,1,3);
	write_string_to_buffer(EXIT_L2,1,6);
	write_string_to_buffer(EXIT_L3,1,8);
	write_string_to_buffer(EXIT_L4,1,10);

	printf("%s",screen);
}

// 게임 설명 화면 그리기 함수
void draw_help_screen(void)
{
	system("cls");
	write_frame_to_buffer();

	write_string_to_buffer(HELP_L1,1,2);
	write_string_to_buffer(HELP_L2,1,5);
	write_string_to_buffer(HELP_L3,1,6);
	write_string_to_buffer(HELP_L4,1,7);
	write_string_to_buffer(HELP_L5,1,9);
	write_string_to_buffer(HELP_L6,1,11);

	printf("%s",screen);
}

int main(void)
{
	char ckey;

    cursor_type(NOCURSOR); //마우스 커서를 지우기 

	draw_title_screen(); // 타이틀 화면 그리기

	while(1)
	{
		ckey = getch(); // 눌린 키값을 저장

		if(ckey == '1') // 타이틀 메뉴에서 '1' 이 눌렸을 때 처리
		{
			(void)game();
			draw_title_screen();
		}
		else if(ckey == '2') // 타이틀 메뉴에서 '2' 이 눌렸을 때 처리
		{
			draw_help_screen(); // 게임설명 화면 그리기
			while(1)
			{
				
				ckey = getch();
				if(ckey == ESC) //ESC가 눌렸을 때 처리
				{
					draw_title_screen(); // 타이틀 화면을 그리기
					break; // 반복문 탈출
				}
			}
		}
		else if(ckey == '3') // 타이틀 메뉴에서 '3' 이 눌렸을 때 처리
		{
			draw_exit_screen(); // 종료 화면 그리기
			while(1)
			{
				
				ckey = getch();
				if(ckey == 'y' || ckey == 'Y') //'y'가 눌렸을 때 처리
				{
					cursor_type(NORMALCURSOR); //마우스 커서를 보이기 
					return(1); // 프로그램 완전 종료
				}
				else if(ckey == 'n' || ckey == 'N') //'n'가 눌렸을 때 처리
				{
					draw_title_screen(); // 타이틀 화면을 그리기
					break; // 반복문 탈출
				}
			}
		}
	}

	cursor_type(NORMALCURSOR); //마우스 커서를 보이기 

	return 0; // 프로그램 종료
}
