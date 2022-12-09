#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
float posy = 0.0;

void exit_handler(int signo3)
{
    if (signo3 == SIGQUIT)
    {
        printf("Signal received, exiting");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}
void reset(int signo2)
{
    if (signo2 == SIGUSR1)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        posy = 0.0;
    }
}

int main()
{

    int fd;

    char *myfifo = "/tmp/myfifo2";
    mkfifo(myfifo, 0666);
    char *myfifo2 = "/tmp/myfifo4";
    mkfifo(myfifo2, 0666);
    char vel[80];
    char py[80];
    char format_string[80] = "%f";

    float posx, lim_y;
    float velx, vely, dt;

    velx = 0;
    vely = 0;
    posx = 0.0;
    dt = 0.5;
    lim_y = 9;
    float vx = 0;

    /*
    Movement process
    */
    if (signal(SIGUSR1, reset) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGQUIT, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    while (1)
    {
        // printf("vely: %f\n", vely);
        //

        fd = open(myfifo2, O_WRONLY);
        sprintf(py, "%f", posy);
        write(fd, py, strlen(py) + 1);
        close(fd);

        //
        fd = open(myfifo, O_RDONLY);
        read(fd, vel, 80);
        sscanf(vel, format_string, &vely);
        close(fd);

        posy = posy + dt * vely;
        if (posy > lim_y)
        {
            posy = lim_y;
        }
        // velx = velx - 0.25;
        if (posy < -lim_y)
        {
            posy = -lim_y;
        }
        usleep(500000);
        // velx = velx + 0.25;

        // posx = posx + dt * velx;
        // printf("posx: %f\n", posx);
        // fd = open(myfifo,O_RDONLY);
        // read(fd, vel, 80);
        // sscanf(vel, format_string, &velx,&vely);
        // printf("velx: %f,vely: %f\n", velx,vely);
        // close(fd);
    }
    // Terminate
    endwin();
    return 0;
}