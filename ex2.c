#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int glDelay;
int glMyNumber;
int glCnt;
int glArgc;
int glFatherPID;
int gjDelay;



void SIGTIhandlerFather(){
  int i;
  pid_t tempPid;

  for(i=1; i<=glArgc-1; i++){
    tempPid = getpid() + i;
    kill(tempPid, SIGTERM);
  }
}

void SIG1handlerFather(){
  int i;
  pid_t tempPid;
  printf("\n[Father process: %d] Will ask current values (SIGUSR1) from all active children processes.\n", getpid());

  for(i=1; i<=glArgc-1; i++){
    tempPid = getpid() + i;
    kill(tempPid, SIGUSR1);
  }
}

void SIGThandler(){
  printf("\n[Father process: %d] Will terminate (SIGTERM) child process %d: %d\n", glFatherPID, glMyNumber, getpid());
  exit(0);
}

void SIG1handler(){
  printf("[Child process %d: %d] Value %d\n", glMyNumber, getpid(), glCnt);
}

void SIG2handler(){
  printf("[Child process %d] Echo!\n", getpid());
}

void CONThandler(){
  alarm(100);
  printf("[Child process %d: %d] Is starting!\n", glMyNumber, getpid());
  usleep(1000);

  int cnt = 0;
  glCnt = cnt;
  usleep(1000);
  while(1){
    cnt++;
    glCnt = cnt;
    //printf("[Child process %d: %d] Value %d\n", glMyNumber, getpid(), glCnt);
    sleep(glDelay);
  }
}

void STOPhandler(){
  printf("[Child process %d] Was created and will pause!\n", getpid());
}

void ALMhandler(){
  printf("[Child process %d: %d] Time expired! Final value %d\n", glMyNumber, getpid(), glCnt);
  exit(0);
}


int main(int argc, char **argv){

  int i, j;
  _Bool all;
  pid_t forks[argc-1];
  pid_t childrenPIDS[argc-1];//array that has the pids of the children only for the father
  int fatherPID;
  int cnt;
  int delay;
  int myNumber;
  int childrenWorking;
  int *st;

  glArgc = argc;
  printf("Maximum execution time of children is set to 100 secs\n\n");


  childrenWorking = argc - 1;
  fatherPID = getpid();
  glFatherPID = fatherPID;


  if(getpid() == fatherPID){
    signal(SIGUSR1, SIG1handlerFather);
    signal(SIGUSR2, SIG2handler);
    signal(SIGTERM, SIGTIhandlerFather);
    signal(SIGINT, SIGTIhandlerFather);
    printf("[Father process: %d] Was created and will create %d children!\n", getpid(), argc-1);

    i = 0;
    while((getpid() == fatherPID) && (i < argc-1)){
        forks[i] = fork();
        if(forks[i] == 0){
          myNumber = i + 1;
          glMyNumber = myNumber;
          delay = atoi(argv[myNumber]);
          glDelay = delay;
          printf("[Child process %d: %d] Was created and will pause!\n", i+1, getpid());
          usleep(1000);
          signal(SIGALRM, ALMhandler);
          signal(SIGCONT, CONThandler);
          signal(SIGUSR1, SIG1handler);
          signal(SIGUSR2, SIG2handler);
          signal(SIGTERM, SIGThandler);
          raise(SIGSTOP);
        }
        else if(forks[i] > 0) childrenPIDS[i] = forks[i];
        i++;
    }
    usleep(1000);
    waitpid(-1, st, CLD_STOPPED);

    if(WSTOPPED){
      printf("\n[Father process: %d] Waiting for %d children that are still working!\n", getpid(), childrenWorking);
      usleep(1000);
      kill(-1, SIGCONT);
    }
    for(i=0; i<argc-1; i++){
      wait(NULL);
    }
  }

  return 0;
}
