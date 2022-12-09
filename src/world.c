#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
float posx=0.0; 
float posy=0.0;

void exit_handler(int signo3)
{
    if (signo3 == SIGQUIT)
    {
        printf("Signal received, exiting");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}

int generate_random(int l, int r)
{ // this will generate random number in range l and r
    int rand_num = (rand() % (r - l + 1)) + l;
    return rand_num;
}

int main()
{

    char *myfifo = "/tmp/myfifo3";
    mkfifo(myfifo, 0666);
    char *myfifo2 = "/tmp/myfifo4";
    mkfifo(myfifo2, 0666);
    char *myfifo3 = "/tmp/myfifo5";
    mkfifo(myfifo3, 0666);
    char vel[80];
    char px[80];
    char py[80];
    char realpos[80];
    char format_string[80] = "%f";
    char format_string_2[80] = "%f,%f";

    float lim_x, lim_y,prx,pry;
    float velx, vely, dt, vlimx, vlimy;
    double mean;
    velx = 0;
    vely = 0;
    posx = 0.0;
    posy = 0.0;
    dt = 0.5;
    lim_x = 1.5;
    lim_y = 0.9;
    vlimx = 2;
    vlimy = 2;
    float vx = 0;

    /*
    Movement process
    */
    if (signal(SIGQUIT, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    while (1)
    {
        fd_set fds;
        struct timeval tv;
        int maxfd;
        int res;
        int retval, m, n;
        char buf[256];
        int fd1 = open(myfifo, O_RDWR);
        int fd2 = open(myfifo2, O_RDWR);
        int fd3;
        FD_ZERO(&fds); // Clear FD set for select
        FD_SET(fd1, &fds);
        FD_SET(fd2, &fds);
        tv.tv_sec = 0.1;
        tv.tv_usec = 0;
        maxfd = fd1 > fd2 ? fd1 : fd2;

        retval = select(maxfd + 1, &fds, NULL, NULL, NULL);
        // printf("retval:%d\n",retval);
        switch (retval)
        {
        case -1:
            perror("select()");
            return -1;

        case 2:
            // printf("There are two pipes with info to be read. Choosing one randomnly...\n");
            fflush(stdout);
            int rand_number = generate_random(0, 1);
            if (rand_number == 0)
            {
                read(fd1, px, 80);
                sscanf(px, format_string, &posx);
                close(fd1);
                close(fd2);
            }
            else
            {
                read(fd2, py, 80);
                sscanf(py, format_string, &posy);
                close(fd1);
                close(fd2);
            }
            break;

        case 1:
            // printf("There is one pipe with info to be read.\n");
            fflush(stdout);
            if (FD_ISSET(fd1, &fds) != 0)
            {
                // printf("motorx pipe,\n");
                fflush(stdout);
                res = read(fd1, px, 80);
                sscanf(px, format_string, &posx);
                close(fd1);
                close(fd2);
            }
            else if (FD_ISSET(fd2, &fds) != 0)
            {
                // printf("motorz pipe\n");
                fflush(stdout);
                res = read(fd2, py, 80);
                sscanf(py, format_string, &posy);
                close(fd1);
                close(fd2);
            }
            break;
        }
        int rand_error = generate_random(0, 1);
        /*if (rand_error == 0)
        {
            posx=posx+0.005*posx;
            posy=posy+0.005*posy;
        }
        else
        {
            posx=posx-0.005*posy;
            posy=posy-0.005*posy;
        }*/

        printf("posx:%f,posy%f\n", posx, posy);
        fd3 = open(myfifo3, O_RDWR);
        sprintf(realpos, "%f,%f", posx, posy);
        write(fd3, realpos, strlen(realpos) + 1);
        close(fd3);
        usleep(40000);

        // posx = posx + dt * velx;
        // printf("posx: %f\n", posx);

        // read(fd1, px, 80);
        // sscanf(px, format_string, &posx,&posy);

        /*
        printf("velx:%f,posx:%f\n", velx, posx);
        printf("vely:%f,posy:%f\n", vely, posy);

        //a,s=left,right movement
        //w,s=up,down movement
        //e=stop x axis
        //r=stop y axis

        // Command console
        printf("write the command \n");
        fflush(stdout);

        fgets(input_string, 80, stdin);

        //X axis motor process
        if (input_string[0] == 'd')
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

        }
        if (input_string[0] == 'a')
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
        }
        if (input_string[0] == 'e')
        {
            velx = 0;
        }
        //Y axis motor process

        if (input_string[0] == 'w')
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
        }
        if (input_string[0] == 's')
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
        }
        if (input_string[0] == 'r')
        {
            vely = 0;
        }


        posy = posy + dt * vely;


        //Exit process
        if (input_string[0] == 'q')
        {
            exit(EXIT_SUCCESS);
        }*/
    }
    // Terminate
    endwin();
    return 0;
}