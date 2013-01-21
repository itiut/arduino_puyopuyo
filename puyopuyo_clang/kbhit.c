#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>

//-----------------------------------------------------------------
void init_keyboard();
void close_keyboard();
int kbhit();
int readkey();
//-----------------------------------------------------------------
static struct termios init_tio;

// int main()
// {
//     int ch,count=0;
//     init_keyboard();
//     while ( ch != 'q' ) {
//         if ( kbhit() ) {
//             ch = readkey();
//             printf("you hit %d : %c\n",ch,ch);
//         }
//         count++;
//     }
//     close_keyboard();
//     printf("final count = %d\n",count);
//     return 0;
// }
//-----------------------------------------------------------------
void init_keyboard()  { tcgetattr(0,&init_tio); }
void close_keyboard() { tcsetattr(0,TCSANOW,&init_tio); }
 
// kbhit
int kbhit()
{
    struct termios tio;
    struct timeval tv;
    fd_set rfds;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON);
    tcsetattr(0,TCSANOW,&tio);
    // do not wait
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    tv.tv_usec = 0;
    tv.tv_sec  = 0;
    select(1,&rfds,NULL,NULL,&tv);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return (FD_ISSET(0,&rfds)?1:0);
}
 
// キー入力
int readkey()
{
    int ch;
    struct termios tio;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&tio);
    // input key
    read(0,&ch,1);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return ch;
}
