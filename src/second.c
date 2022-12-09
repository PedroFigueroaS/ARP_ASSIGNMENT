
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>

int main() 
{ 
    int fd1; 
  
    char * myfifo = "/tmp/myfifo"; 
    mkfifo(myfifo, 0666); 
    char * myfifo2 = "/tmp/myfifo2"; 
    mkfifo(myfifo2, 0666); 
    char str1[80], str2[80]; 
    char format_string[80]="%d,%d";
    int n1, n2;
    double mean;
    //char output[80]=d;
    while (1) 
    { 
        fd1 = open(myfifo,O_RDONLY); 
        read(fd1, str1, 80); 

        /* if the first input char is q, exit  */
        if (str1[0] == 'q') exit(EXIT_SUCCESS) ;
        /* read numbers from input line */
        sscanf(str1, format_string, &n1, &n2);
        mean = (n1 + n2) / 2.0; 
        printf("mean value is: %f, sum is: %d\n", mean, n1 + n2);
        close(fd1);

        // TO WRITE
        fd1=open(myfifo2,O_WRONLY);
        char *str2="Data calculated";
        write(fd1,str2,strlen(str2)+1); 
        close(fd1);
    } 
    return 0; 
} 
