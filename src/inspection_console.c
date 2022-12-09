#include "./../include/inspection_utilities.h"
#include <signal.h>
#define LEN 10
float ee_x = 0.0;
float ee_y = 0.0;

void exit_handler(int signo3)
{
    if (signo3 == SIGQUIT)
    {
        printf("Signal received, exiting");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}

void stop(int signo1)
{
    if (signo1 == SIGUSR1)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        char line[LEN];
        FILE *cmd = popen("pidof -s command", "r");
        fgets(line, LEN, cmd);
        long pid = 0;
        pid = strtoul(line, NULL, 10);

        kill(pid, SIGUSR1);
    }
}

void reset(int signo2)
{
    if (signo2 == SIGUSR1)
    {
        // printf("Signal received, exiting");
        fflush(stdout);
        char line1[LEN];
        char line2[LEN];
        FILE *cmd1 = popen("pidof -s motor_x", "r");
        FILE *cmd2 = popen("pidof -s motor_z", "r");
        fgets(line1, LEN, cmd1);
        fgets(line2, LEN, cmd2);
        long pid1 = 0;
        long pid2 = 0;
        pid1 = strtoul(line1, NULL, 10);
        pid2 = strtoul(line2, NULL, 10);
        ee_x = 0.0;
        ee_y = 0.0;
        update_console_ui(&ee_x, &ee_y);
        kill(pid1, SIGUSR1);
        kill(pid2, SIGUSR1);
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
    // declaration of pipes
    int fd;
    char *myfifo = "/tmp/myfifo5";
    mkfifo(myfifo, 0666);
    char realpos[80];
    char format_string[80] = "%f,%f";
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // End-effector coordinates

    // Initialize User Interface
    init_console_ui();

    // SIGNAL
    if (signal(SIGQUIT, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGUSR2, k_process) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    // Infinite loop
    while (TRUE)
    {
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

                // STOP button pressed
                if (check_button_pressed(stp_button, &event))
                {
                    if (signal(SIGUSR1, stop) == SIG_ERR)
                        printf("\ncan't catch SIGINT\n");
                    // signal(SIGUSR1, Handle);
                    kill(getpid(), SIGUSR1);
                    kill(getpid(), SIGUSR2);

                    /*mvprintw(LINES - 1, 1, "STP button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }*/
                }

                // RESET button pressed
                else if (check_button_pressed(rst_button, &event))
                {
                    if (signal(SIGUSR1, reset) == SIG_ERR)
                        printf("\ncan't catch SIGINT\n");
                    kill(getpid(), SIGUSR1);
                    kill(getpid(), SIGUSR2);
                    /*mvprintw(LINES - 1, 1, "RST button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }*/
                }
            }
        }

        // To be commented in final version...
        fd = open(myfifo, O_RDWR);
        read(fd, realpos, 80);
        sscanf(realpos, format_string, &ee_x, &ee_y);
        close(fd);
        usleep(40000);
        /*switch (cmd)
        {
            case KEY_LEFT:
                ee_x--;
                break;
            case KEY_RIGHT:
                ee_x++;
                break;
            case KEY_UP:
                ee_y--;
                break;
            case KEY_DOWN:
                ee_y++;
                break;
            default:
                break;
        }*/

        // Update UI
        update_console_ui(&ee_x, &ee_y);
    }

    // Terminate
    endwin();
    return 0;
}
