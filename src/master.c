#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
void exit_handler(int signo)
{
  if (signo == SIGQUIT)
  {
    printf("Signal received, exiting\n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
  }
}

int spawn(const char *program, char *arg_list[])
{

  pid_t child_pid = fork();

  if (child_pid < 0)
  {
    perror("Error while forking...");
    return 1;
  }

  else if (child_pid != 0)
  {
    return child_pid;
  }

  else
  {
    if (execvp(program, arg_list) == 0)
      ;
    perror("Exec failed");
    return 1;
  }
}

int main()
{
  if (signal(SIGQUIT, exit_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  char *arg_list_command[] = {"/usr/bin/konsole", "-e", "./bin/command", NULL};
  char *arg_list_motor_x[] = {"/usr/bin/konsole", "-e", "./src/motor_x", NULL};
  char *arg_list_motor_z[] = {"/usr/bin/konsole", "-e", "./src/motor_z", NULL};
  char *arg_list_world[] = {"/usr/bin/konsole", "-e", "./src/world", NULL};
  char *arg_list_inspection[] = {"/usr/bin/konsole", "-e", "./bin/inspection", NULL};
  char *arg_list_watchdog[] = {"/usr/bin/konsole", "-e", "./src/watchdog", NULL};

  pid_t pid_cmd = spawn("/usr/bin/konsole", arg_list_command);
  pid_t pid_mx = spawn("/usr/bin/konsole", arg_list_motor_x);
  pid_t pid_mz = spawn("/usr/bin/konsole", arg_list_motor_z);
  pid_t pid_world = spawn("/usr/bin/konsole", arg_list_world);
  pid_t pid_insp = spawn("/usr/bin/konsole", arg_list_inspection);
  pid_t pid_wtd = spawn("/usr/bin/konsole", arg_list_watchdog);
  // int p_id, p_pid;

  // p_id = getpid(); /*process id*/
  // p_pid = getpid(); /*parent process id*/

  // printf("Process ID: %d\n", p_id);
  // printf("Parent Process ID: %d\n", p_pid);
  // printf("command: %d\n", pid_cmd);
  // printf("mx: %d\n", pid_mx);
  // printf("mz: %d\n", pid_mz);

  int status;
  waitpid(pid_cmd, &status, 0);
  waitpid(pid_mx, &status, 0);
  waitpid(pid_mz, &status, 0);
  waitpid(pid_world, &status, 0);
  waitpid(pid_insp, &status, 0);
  waitpid(pid_wtd, &status, 0);

  printf("Main program exiting with status %d\n", status);
  return 0;
}
