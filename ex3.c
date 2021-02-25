#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>


#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define GRAY "\033[38;1m"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int glIndex;

void SIGThandler(){
  exit(0);
}

int main(int argc, char *argv[]){
  //Variables declaration vol.1
  int n;
  int i;
  int checkInt;
  char checkStr[101];
  char round_robin[15] = "--round-robin";
  char random[15] = "--random";
  _Bool rb = 0;
  _Bool rdm = 0;
  pid_t fatherPID;
  //-----------------------------------------------------------------

  fatherPID = getpid();

  //Check if program inputs are in the right form
  if(argc < 2 || argc > 3){
    printf("Usage: %s <nChildren> [--random] [--round-robin]\n", "%s");
    return 0;
  }
  else if(argc == 2){
    strcpy(checkStr, argv[1]);
    for(i=0; i<strlen(checkStr); i++){
      if(checkStr[i] < '0' || checkStr[i] > '9'){
        printf("Usage: %s <nChildren> [--random] [--round-robin]\n", "%s");
        return 0;
      }
    }
    n = atoi(argv[1]);
    rb = 1;
    //printf("%d\n", n);
  }
  else{
    strcpy(checkStr, argv[1]);
    for(i=0; i<strlen(checkStr); i++){
      if(checkStr[i] < '0' || checkStr[i] > '9'){
        printf("Usage: %s <nChildren> [--random] [--round-robin]\n", "%s");
        return 0;
      }
    }
    n = atoi(argv[1]);
    //printf("%d\n", n);

    if(!strcmp(random, argv[2])) rdm = 1;
    if(!strcmp(round_robin, argv[2])) rb = 1;

    if(!(rdm || rb)){
      printf("Usage: %s <nChildren> [--random] [--round-robin]\n", "%s");
      return 0;
    }
  }
  //----------------------------------------------------------------

  //Variables declaration vol.2
  pid_t forks[n];
  int fd_father[2*n];
  int fd_child[2*n];
  _Bool locked[n];
  _Bool allLocked;
  _Bool isNumber;
  pid_t childrenPIDS[n];
  int val;
  //-----------------------------------------------------------------

  //Initialization of locked array
  for(i=0; i<n; i++) locked[i] = 0;
  //-----------------------------------------------------------------

  //Creating pipes
  for(i=0; i<n; i++){
    if(pipe(&fd_father[2*i]) != 0){
      printf("Error in creating pipe\n");
      return 0;
    }
    if(pipe(&fd_child[2*i]) != 0){
      printf("Error in creating pipe\n");
      return 0;
    }
  }
  //-----------------------------------------------------------------

  //Creating child processes
  i = 0;
  while((getpid() == fatherPID) && (i < n)){
      if(forks[i] = fork() < 0){
        printf("Error in creating child\n");
        return 0;
      }
      else if(forks[i] == 0){
        signal(SIGTERM, SIGThandler);
        glIndex = i;
        usleep(100);
      }
      i++;
  }
  //------------------------------------------------------------------


  while(1){
    if(getpid() == fatherPID){
      fd_set inset;
      //fd_set pipset;
      int maxfd;

      FD_ZERO(&inset);
      FD_SET(STDIN_FILENO, &inset);
      //FD_ZERO(&inset);
      for(i=0; i<n; i++){
        FD_SET(fd_child[2*i], &inset);
      }

      maxfd = STDIN_FILENO;
      for(i=0; i<n; i++){
        if(fd_child[2*i] > maxfd) maxfd = fd_child[2*i];
      }

      maxfd += 1;
      int ready_fds;
      ready_fds = select(maxfd, &inset, NULL, NULL, NULL);
      //if (ready_fds <= 0) {
        //    perror("select");
          //  continue;                                       // just try again
      //}
      if(FD_ISSET(STDIN_FILENO, &inset)){
        char buffer[101];
          int n_read = read(STDIN_FILENO, buffer, sizeof(buffer));   // error checking!
          buffer[n_read] = '\0';                          // why?

          // New-line is also read from the stream, discard it.
          if (n_read > 0 && buffer[n_read-1] == '\n') {
              buffer[n_read-1] = '\0';
          }


          if(!strcmp(buffer, "help")){
            printf(BLUE"Type a number to send job to a child!"WHITE"\n");
            continue;
          }
          else if(!strcmp(buffer, "exit")){
            int childrenRemaining = n;
            for(i=0; i<n; i++){
              printf(BLUE"Waiting for %d children"WHITE"\n", childrenRemaining);
              childrenRemaining--;
            }
            printf(GREEN"All children terminated"WHITE"\n");
            kill(0, SIGTERM);
          }
          else{
            strcpy(checkStr, buffer);
            isNumber = 1;
            for(i=0; i<strlen(checkStr); i++){
              if(i == 0 && checkStr[i] == '-') continue;
              if(checkStr[i] < '0' || checkStr[i] > '9'){
                printf(BLUE"Type a number to send job to a child!"WHITE"\n");
                isNumber = 0;
                break;
              }
            }
            if(isNumber){
              val = atoi(buffer);
              if(rb){
                allLocked = 1;
                for(i=0; i<n; i++){
                  if(!locked[i]){
                    allLocked = 0;
                    break;
                  }
                }
                if(!allLocked){
                  close(fd_father[2*i]);
                  write(fd_father[2*i+1], &val, sizeof(val));
                  printf(MAGENTA"[Parent] Assigned %d to child %d"WHITE"\n", val, i);
                  locked[i] = 1;
                }
                else{
                  printf(MAGENTA"All children are locked\nWait for one to unlock"WHITE"\n");
                }
              }
              if(rdm){
                allLocked = 1;
                for(i=0; i<n; i++){
                  if(!locked[i]){
                    allLocked = 0;
                    break;
                  }
                }
                if(!allLocked){
                  while(1){
                    i = rand() % n;
                    if(locked[i]) continue;
                    else{
                      close(fd_father[2*i]);
                      write(fd_father[2*i+1], &val, sizeof(val));
                      printf(MAGENTA"[Parent] Assigned %d to child %d"WHITE"\n", val, i);
                      locked[i] = 1;
                      break;
                    }
                  }
                }

                else{
                  printf(MAGENTA"All children are locked\nWait for one to unlock"WHITE"\n");
                }

              }
            }

          }


      }
      //printf("hi\n");

      for(i=0; i<n; i++){
        //ready_fds = select(maxfd, &inset, NULL, NULL, NULL);
        /*if(ready_fds <= 0) {
              continue;                                       // just try again
        }*/
        if(FD_ISSET(fd_child[2*i], &inset)){
            close(fd_child[2*i+1]);
            read(fd_child[2*i], &val, sizeof(val));   // error checking!
            printf(YELLOW"[Parent] Received %d from child %d"WHITE"\n", val, i);
            locked[i] = 0;

        }
      }

    }

    else{
      close(fd_father[2*glIndex + 1]);
      int n_read = read(fd_father[2*glIndex], &val, sizeof(int));
      if(n_read > 0){
        printf(BLUE"[Child %d] [%d] Child received %d!"WHITE"\n", glIndex, getpid(), val);
        sleep(5);
        close(fd_child[2*glIndex]);
        val++;
        printf(GREEN"[Child %d] [%d] Finished hard work, writing back %d"WHITE"\n", glIndex, getpid(), val);
        write(fd_child[2*glIndex + 1], &val, sizeof(val));
      }
    }

  }



}
