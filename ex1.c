#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Enum for encryption mode */
typedef enum {
    ENCRYPT,
    DECRYPT
} encrypt_mode;

//Caesar encryption algorithm
char caesar(unsigned char ch, encrypt_mode mode, int key)
{
    if (ch >= 'a' && ch <= 'z') {
        if (mode == ENCRYPT) {
            ch += key;
            if (ch > 'z') ch -= 26;
        } else {
            ch -= key;
            if (ch < 'a') ch += 26;
        }
        return ch;
    }

    if (ch >= 'A' && ch <= 'Z') {
        if (mode == ENCRYPT) {
            ch += key;
            if (ch > 'Z') ch -= 26;
        } else {
            ch -= key;
            if (ch < 'A') ch += 26;
        }
        return ch;
    }

    return ch;
}
//------------------------------------------

int main(int argc, char **argv){
  char ch;
  char message[100];
  char encryptedMSG[100];
  int key;
  FILE *fileReader;
  pid_t p1, p2;

  int i = 0;
  if(argc == 3){

    p1 = fork(); //create C1 process
    if(p1 > 0) wait(NULL); //father waiting C1 to terminate

    else if(p1 == 0){
      fileReader = fopen(argv[1], "r"); //C1 opens decrypted file
      if(fileReader != NULL){
        key = atoi(argv[2]); //C1 reads key number
        if(key >= 0 && key <= 25){
          while((ch = getc(fileReader)) != EOF){
            //message[i] = ch; //C1 reads message from decrypted file
            encryptedMSG[i] = caesar(ch, ENCRYPT, key); //C1 encrypts message using caesar algorithm
            i++;
          }
          //printf("%s", message);
          //printf("%s", encryptedMSG);
          fclose(fileReader); //C1 closes decrypted file
          fileReader = fopen("encrypted", "w"); //C1 opens encrypted file
          if(fileReader != NULL){
            for(i=0; i<100; i++){
              if(encryptedMSG[i] == '\0') break;
              putc(encryptedMSG[i], fileReader); //C1 writes the encrypted message into the encrypted file
            }
            fclose(fileReader); //C1 closes encrypted file
          }
          else{
            printf("Could not open encrypted file\n");
          }
          return 0;
        }
        else{
          printf("Invalid key number\n");
          return 0;
        }
      }
      else{
        printf("Could not open file\n");
        return 0;
      }
    } //C1 process terminated

    p2 = fork(); //father creates C2
    if(p2 > 0) wait(NULL);

    else if(p2 == 0){
      fileReader = fopen("encrypted", "r");
      if(fileReader != NULL){
        key = atoi(argv[2]); //C2 reads key number
        i = 0;
        if(key >= 0 && key <= 25){
          while((ch = getc(fileReader)) != EOF){
            encryptedMSG[i] = ch; //C2 reads encrypted message from encrypted file
            message[i] = caesar(ch, DECRYPT, key); //C2 decrypts message using caesar algorithm
            i++;
          }
          printf("%s", message);
        }
      else{
        printf("Could not open encrypted file\n");
      }
      return 0;
      }
    }//C2 terminated
  }
  else{
    printf("Error in arguments\n");
    return 0;
  }

}
