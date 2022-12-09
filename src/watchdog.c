#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define LEN 10
int count = 0;

void kmaster_1(FILE *s)
{
    char line[LEN];
    fgets(line, LEN, s);
    long pid = 0;
    pid = strtoul(line, NULL, 10);
    kill(pid, SIGQUIT);
}

void kmaster(int signo1)
{
    if (signo1 == SIGQUIT)
    {

        // printf("Signal received, exiting");
        fflush(stdout);
        
      
        FILE *cmd1 = popen("pidof -s command", "r");
        kmaster_1(cmd1);
        FILE *cmd2 = popen("pidof -s motor_x", "r");
        kmaster_1(cmd2);
        FILE *cmd3 = popen("pidof -s motor_z", "r");
        kmaster_1(cmd3);
         FILE *cmd4 = popen("pidof -s world", "r");
        kmaster_1(cmd4);
         FILE *cmd5 = popen("pidof -s inspection", "r");
        kmaster_1(cmd5);
        FILE *cmd6 = popen("pidof -s master", "r");
        kmaster_1(cmd6);

        exit(EXIT_SUCCESS);
    }
}

void k_process(int signo2)
{
    if (signo2 == SIGUSR2)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        count = 0;
    }
}

int main()
{

    /*
    Movement process
    */
    if (signal(SIGUSR2, k_process) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGQUIT, kmaster) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    while (1)
    {
        printf("counter: %d\n", count);
        count = count + 1;
        sleep(1);
        if (count == 30)
        {
            kill(getpid(), SIGQUIT);
        }
    }
    // Terminate
    endwin();
    return 0;
}