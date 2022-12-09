#include "./../include/command_utilities.h"
#include <signal.h>
#define LEN 10
float velx, vely, vlimx, vlimy;
void exit_handler(int signo3)
{
    if (signo3 == SIGQUIT)
    {
        printf("Signal received, exiting");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}
void stop(int signo)
{
    if (signo == SIGUSR1)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        velx = 0.0;
        vely = 0.0;
    }
}

void k_process(int signo2)
{
    if (signo2 == SIGUSR2)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        char line[LEN];
        FILE *cmd = popen("pidof -s watchdog", "r");
        fgets(line, LEN, cmd);
        long pid = 0;
        pid = strtoul(line, NULL, 10);

        kill(pid, SIGUSR2);
    }
}

int main(int argc, char const *argv[])
{

    int fd1, fd2;
    // pipe for motorx
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    // pipe for motorx
    char *myfifo2 = "/tmp/myfifo2";
    mkfifo(myfifo2, 0666);
    velx = 0;
    vely = 0;
    vlimx = 2;
    vlimy = 2;
    char vel[20];

    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize User Interface
    init_console_ui();
    if (signal(SIGUSR1, stop) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGUSR2, k_process) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGQUIT, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    // Infinite loop
    while (TRUE)
    {
        fd1 = open(myfifo, O_WRONLY);
        fflush(stdout);
        sprintf(vel, "%f", velx);
        write(fd1, vel, strlen(vel) + 1);
        close(fd1);
        fd2 = open(myfifo2, O_WRONLY);
        fflush(stdout);
        sprintf(vel, "%f", vely);
        write(fd2, vel, strlen(vel) + 1);
        close(fd2);
        usleep(10000);

        // Get mouse/resize commands in non-blocking mode...
        int cmd = getch();

        // If user resizes screen, re-draw UI
        if (cmd == KEY_RESIZE)
        {
            if (first_resize)
            {
                first_resize = FALSE;
            }
            else
            {
                reset_console_ui();
            }
        }
        // Else if mouse has been pressed
        else if (cmd == KEY_MOUSE)
        {

            // Check which button has been pressed...
            if (getmouse(&event) == OK)
            {

                // Vx++ button pressed
                if (check_button_pressed(vx_incr_btn, &event))
                {

                    if (velx < 0)
                    {
                        velx = 0;
                    }

                    else
                    {
                        velx = velx + 0.25;

                        if (velx > vlimx)
                        {
                            velx = vlimx;
                        }
                    }

                    // signal(SIGUSR1, Handle);
                    kill(getpid(), SIGUSR2);

                    /*fd1 = open(myfifo, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", velx);
                    write(fd1, vel, strlen(vel) + 1);
                    close(fd1);*/

                    /*mvprintw(LINES - 1, 1, "Horizontal Speed Increased");
                     refresh();
                     sleep(1);
                     for(int j = 0; j < COLS; j++) {
                         mvaddch(LINES - 1, j, ' ');
                     }*/
                }

                // Vx-- button pressed
                if (check_button_pressed(vx_decr_btn, &event))
                {

                    if (velx > 0)
                    {
                        velx = 0;
                    }
                    else
                    {
                        velx = velx - 0.25;
                        if (velx < -vlimx)
                        {
                            velx = -vlimx;
                        }
                    }
                    kill(getpid(), SIGUSR2);
                    /*fd1 = open(myfifo, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", velx);
                    write(fd1, vel, strlen(vel) + 1);
                    close(fd1);*/
                    /*
                    mvprintw(LINES - 1, 1, "Horizontal Speed Decreased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }*/
                }

                // Vx stop button pressed
                if (check_button_pressed(vx_stp_button, &event))
                {

                    velx = 0;
                    kill(getpid(), SIGUSR2);
                    /*fd1 = open(myfifo, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", velx);
                    write(fd1, vel, strlen(vel) + 1);
                    close(fd1);*/

                    /*    mvprintw(LINES - 1, 1, "Horizontal Motor Stopped");
                        refresh();
                        sleep(1);
                        for(int j = 0; j < COLS; j++) {
                            mvaddch(LINES - 1, j, ' ');

                        }*/
                }
                // Vz++ button pressed
                if (check_button_pressed(vz_incr_btn, &event))
                {
                    if (vely < 0)
                    {
                        vely = 0;
                    }

                    else
                    {
                        vely = vely + 0.25;

                        if (vely > vlimy)
                        {
                            vely = vlimy;
                        }
                    }
                    kill(getpid(), SIGUSR2);
                    /*fd2 = open(myfifo2, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", vely);
                    write(fd2, vel, strlen(vel) + 1);
                    close(fd2);*/
                }

                // Vz-- button pressed
                if (check_button_pressed(vz_decr_btn, &event))
                {
                    if (vely > 0)
                    {
                        vely = 0;
                    }
                    else
                    {
                        vely = vely - 0.25;
                        if (vely < -vlimy)
                        {
                            vely = -vlimy;
                        }
                    }
                    kill(getpid(), SIGUSR2);
                    /*fd2 = open(myfifo2, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", vely);
                    write(fd2, vel, strlen(vel) + 1);
                    close(fd2);*/
                    /*mvprintw(LINES - 1, 1, "Vertical Speed Decreased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }*/
                }

                // Vz stop button pressed
                if (check_button_pressed(vz_stp_button, &event))
                {

                    vely = 0;
                    kill(getpid(), SIGUSR2);
                    /*fd2 = open(myfifo2, O_WRONLY);
                    fflush(stdout);
                    sprintf(vel, "%f", vely);
                    write(fd2, vel, strlen(vel) + 1);
                    close(fd2);*/

                    /*mvprintw(LINES - 1, 1, "Vertical Motor Stopped");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }*/
                }
            }
        }

        refresh();
    }

    // Terminate
    endwin();
    return 0;
}
