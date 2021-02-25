#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
//#include <arpa/inet.h>
#include <netdb.h>
//#include <netinet/in.h>
#include <ctype.h>
#include <time.h>

#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define GRAY "\033[38;1m"


int main(int argc, char **argv){
  char checkStr[30];
  char checkArgStr[30];
  int i;
  char host[50];
  char port[50];
  _Bool debug;
  _Bool portArg;
  _Bool hostArg;

  debug = 0;
  strcpy(port, "8080");
  strcpy(host, "tcp.akolaitis.os.grnetcloud.net");
  //strcpy(host, "localhost.net");
  if(argc >= 1 && argc <= 6){ //check the input arguments
    if(argc == 6){
      //check 1st argument
      strcpy(checkStr, "--host");
      strcpy(checkArgStr, argv[1]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      strcpy(host, argv[2]);
      //printf("%s\n", host);

      //check 2nd argument
      strcpy(checkStr, "--port");
      strcpy(checkArgStr, argv[3]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      strcpy(port, argv[4]);
      //printf("%d\n", port);

      //check 3rd argument
      strcpy(checkStr, "--debug");
      strcpy(checkArgStr, argv[5]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      debug = 1;
    }
    else if(argc == 5){
      //check 1st argument
      strcpy(checkStr, "--host");
      strcpy(checkArgStr, argv[1]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      strcpy(host, argv[2]);

      //check 2nd argument
      strcpy(checkStr, "--port");
      strcpy(checkArgStr, argv[3]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      strcpy(port, argv[4]);
    }
    else if(argc == 4){
      //check 1st argument
      hostArg = 1;
      strcpy(checkStr, "--host");
      strcpy(checkArgStr, argv[1]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          hostArg = 0;
          break;
        }
      }
      if(hostArg){
        strcpy(host, argv[2]);
        printf("%s\n", host);
      }

      //check 1st argument again
      if(!hostArg){
        portArg = 1;
        strcpy(checkStr, "--port");
        strcpy(checkArgStr, argv[1]);
        for(i=0; i<strlen(checkStr); i++){
          if(checkStr[i] != checkArgStr[i]){
            printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
            return 0;
          }
        }
        strcpy(port, argv[2]);
      }

      //check debug argument
      strcpy(checkStr, "--debug");
      strcpy(checkArgStr, argv[3]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      debug = 1;
    }
    else if(argc == 3){
      //check 1st argument
      hostArg = 1;
      strcpy(checkStr, "--host");
      strcpy(checkArgStr, argv[1]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          hostArg = 0;
          break;
        }
      }
      if(hostArg){
        strcpy(host, argv[2]);
      }

      //check 1st argument again
      if(!hostArg){
        portArg = 1;
        strcpy(checkStr, "--port");
        strcpy(checkArgStr, argv[1]);
        for(i=0; i<strlen(checkStr); i++){
          if(checkStr[i] != checkArgStr[i]){
            printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
            return 0;
          }
        }
        strcpy(port, argv[2]);
      }
    }
    else if(argc == 2){
      //check debug argument
      strcpy(checkStr, "--debug");
      strcpy(checkArgStr, argv[1]);
      for(i=0; i<strlen(checkStr); i++){
        if(checkStr[i] != checkArgStr[i]){
          printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
          return 0;
        }
      }
      debug = 1;
    }
    else if(argc == 1){
      debug = 0;
    }
  }
  else{
    printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n", "%s");
    return 0;
  }
  //printf("%s\n", port);
  //printf("%s\n", host);

  /*//creating client
  int domain = AF_INET;
  int type = SOCK_STREAM;

  int sd = socket(domain, type, 0);
  if(sd < 0){
    perror("socket");
    return -1;
  }

  struct sockaddr_in sin;
  bzero(&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  inet_pton(AF_INET, host, &sin.sin_addr);


  bind(sd, (struct sockaddr*)&sin, sizeof(sin));

  printf(BLUE"Connecting!"WHITE"\n");
  if(connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
    perror("connect");
    exit(0);
  }
  else printf(GREEN"Connected to %s:%d!"WHITE"\n", host, port);*/

  struct addrinfo *res;

  int adr = getaddrinfo(host, port, NULL, &res); //contains the Internet address that can be specified in a call and sets the structs hints and res
  if (adr != 0) {
      perror("getaddrinfo");
      exit(EXIT_FAILURE);
  }

  int sd = socket(AF_INET, SOCK_STREAM, 0); //creates an endpoint for communication
  if (sd < 0) {
      perror("socket");
      exit(EXIT_FAILURE);
  }

  printf(BLUE"Connecting!"WHITE"\n");

  int con = connect(sd, res->ai_addr, res->ai_addrlen); //connects with the server's socket
  if (con != 0) {
      perror("connection");
      exit(EXIT_FAILURE);
  }

  printf(GREEN"Connected to %s:%s!"WHITE"\n", host, port);

  //creating read
  char buffer[100];
  char recv_buffer[100];
  fd_set inset;
  int maxfd;
  _Bool boolGet, boolResponse, boolMediator;

  boolGet = 0;
  boolResponse = 0;
  boolMediator = 0;
  while(1){
    FD_ZERO(&inset);
    FD_SET(STDIN_FILENO, &inset);
    FD_SET(sd, &inset);

    maxfd = STDIN_FILENO;
    if(sd > maxfd) maxfd = sd;
    maxfd += 1;
    select(maxfd, &inset, NULL, NULL, NULL);

    if(FD_ISSET(STDIN_FILENO, &inset)){
      memset(buffer, 0, 100);
      int n_read = read(STDIN_FILENO, buffer, sizeof(buffer));
      buffer[n_read] = '\0';
      // New-line is also read from the stream, discard it.
      if (n_read > 0 && buffer[n_read-1] == '\n') {
          buffer[n_read-1] = '\0';
      }
      if(!strcmp(buffer, "help")){
        printf(YELLOW"Available commands:"WHITE"\n");
        printf(YELLOW"* 'help'                   : Print this help message"WHITE"\n");
        printf(YELLOW"* 'exit'                   : Exit"WHITE"\n");
        printf(YELLOW"* 'get'                    : Retrieve sensor data"WHITE"\n");
        printf(YELLOW"* 'N name surname reason'  : Ask permission to go out"WHITE"\n");
        continue;
      }
      else if(!strcmp(buffer, "exit")){
        close(sd);
        exit(0);
      }
      else if(!strcmp(buffer, "get")){
        write(sd, &buffer, sizeof(buffer));
        if(debug){
          printf(YELLOW"[DEBUG] sent 'get'"WHITE"\n");
        }
        boolGet = 1;
        continue;
      }
      else if(n_read > 1){
        write(sd, &buffer, sizeof(buffer));
        if(debug) printf(YELLOW"[DEBUG] sent '%s'"WHITE"\n", buffer);
        continue;
      }
    }
    else if(FD_ISSET(sd, &inset)){
      memset(recv_buffer, 0, 100);
      int n_read = read(sd, recv_buffer, sizeof(recv_buffer));
      recv_buffer[n_read] = '\0';
      // New-line is also read from the stream, discard it.
      if (n_read > 0 && recv_buffer[n_read-1] == '\n') {
          recv_buffer[n_read-1] = '\0';
      }
      if(!strcmp(recv_buffer, "try again")){
        printf(MAGENTA"%s"WHITE"\n", recv_buffer);
        continue;
      }
      if(boolGet){
        if(debug){
          printf(YELLOW"[DEBUG] read '%s'"WHITE"\n", recv_buffer);
        }
        //printf("%s\n", recv_buffer);
        char N, brightness[4], temperature[5], time[11];
        N = recv_buffer[0];
        brightness[0] = recv_buffer[2];
        brightness[1] = recv_buffer[3];
        brightness[2] = recv_buffer[4];
        temperature[0] = recv_buffer[6];
        temperature[1] = recv_buffer[7];
        temperature[2] = recv_buffer[8];
        temperature[3] = recv_buffer[9];
        for(int i=0; i<11; i++){
          time[i] = recv_buffer[i+11];
        }
        //printf("%c\n", N);
        //printf("%s\n", brightness);
        //printf("%s\n", temperature);
        //printf("%s\n", time);
        int intN, intBr, intTime;
        double dbTemp;
        intBr = atoi(brightness);
        dbTemp = atoi(temperature);
        dbTemp /= 100;
        intTime = atoi(time);
        printf(RED"----------------------"WHITE"\n");
        switch (N){
          case '0':
            printf(RED"Latest event: boot (0)"WHITE"\n");
            break;
          case '1':
            printf(RED"Latest event: setup (1)"WHITE"\n");
            break;
          case '2':
            printf(RED"Latest event: interval (2)"WHITE"\n");
            break;
          case '3':
            printf(RED"Latest event: button (3)"WHITE"\n");
            break;
          case '4':
            printf(RED"Latest event: motion (4)"WHITE"\n");
            break;
        }
        printf(RED"Temperature is: %.2f"WHITE"\n", dbTemp);
        printf(RED"Light level is: %d"WHITE"\n", intBr);

        time_t rawtime = intTime;
        struct tm *info;
        info = localtime(&rawtime);
        printf(RED"Timestamp is: %s"WHITE"\n", asctime(info));

        boolGet = 0;
        continue;
      }
      if(boolResponse){
        if(debug){
          printf(YELLOW"[DEBUG] read '%s'"WHITE"\n", recv_buffer);
        }
        printf(CYAN"Response: %s"WHITE"\n", recv_buffer);
        boolResponse = 0;
        boolMediator = 1;
        continue;
      }
      if(boolMediator){
        boolMediator = 0;
        continue;
      }
      if(debug){
        printf(YELLOW"[DEBUG] read '%s'"WHITE"\n", recv_buffer);
      }
      printf(CYAN"Send verification code: '%s'"WHITE"\n", recv_buffer);
      boolResponse = 1;
    }
  }



  close(sd);
  exit(0);

}
