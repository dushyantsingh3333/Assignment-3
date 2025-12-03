#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define W 21
#define H 21
#define FRAME_MS 50   

int grid[W][H];
int px,py,ex,ey;

int rnd(){ return rand(); }

void hideCursor(){
    CONSOLE_CURSOR_INFO ci;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(h,&ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(h,&ci);
}

void rewindCursor(){ 
    COORD p = {0,0}; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p); 
}

void carve(int x,int y){
    int d[4][2]={{2,0},{-2,0},{0,2},{0,-2}};
    for(int i=0;i<4;i++){ int r=rnd()%4; int t0=d[i][0], t1=d[i][1]; d[i][0]=d[r][0]; d[i][1]=d[r][1]; d[r][0]=t0; d[r][1]=t1; }
    for(int i=0;i<4;i++){
        int nx = x + d[i][0], ny = y + d[i][1];
        if(nx>0 && ny>0 && nx<W-1 && ny<H-1 && grid[nx][ny]==0){
            grid[x + d[i][0]/2][y + d[i][1]/2] = 1;
            grid[nx][ny] = 1;
            carve(nx,ny);
        }
    }
}

void draw_frame(){
    rewindCursor();
    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            if(x==px && y==py) putchar('@');
            else if(x==ex && y==ey) putchar('E');
            else putchar(grid[x][y] ? ' ' : '#');
        }
        putchar('\n');
    }
    printf("WASD/Arrows to move  |  Q to quit  |  FPS target: %d ms\n", FRAME_MS);
}

int main(){
    srand((unsigned)time(NULL));
    hideCursor();
    for(int i=0;i<W;i++) for(int j=0;j<H;j++) grid[i][j]=0;
    grid[1][1]=1;
    carve(1,1);

    px=1; py=1; ex=W-2; ey=H-2;
    for(int i=0;i<H+3;i++) puts("");

    draw_frame();

    DWORD prevTick = GetTickCount();
    while(1){
        DWORD frameStart = GetTickCount();
        if(_kbhit()){
            int c = getch();
            int nx = px, ny = py;
            if(c==0 || c==224){
                int k = getch();
                if(k==72) ny--; else if(k==80) ny++;
                else if(k==75) nx--; else if(k==77) nx++;
            } else {
                if(c=='w'||c=='W') ny--;
                else if(c=='s'||c=='S') ny++;
                else if(c=='a'||c=='A') nx--;
                else if(c=='d'||c=='D') nx++;
                else if(c=='q'||c=='Q') break;
            }
            if(nx>=0 && nx<W && ny>=0 && ny<H && grid[nx][ny]){ px=nx; py=ny; }
        }
        draw_frame();
        if(px==ex && py==ey){
            printf("\nYou Win!\n");
            break;
        }
        DWORD frameEnd = GetTickCount();
        DWORD elapsed = frameEnd - frameStart;
        if(elapsed < FRAME_MS) Sleep(FRAME_MS - elapsed);
        prevTick = frameStart;
    }
    return 0;
}
