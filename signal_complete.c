#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> 
#include <unistd.h>
#include<ncurses.h>
#include<time.h>

void handle_signal1(int);
void handle_signal2(int);
void handle_signal3(int);
struct sigaction action_handle;
int main(int argc,char *argv)
{
//system("clear");

action_handle.sa_handler = &handle_signal1;
action_handle.sa_flags = SA_RESTART;
sigfillset(&action_handle.sa_mask);
if (sigaction(SIGUSR1, &action_handle, NULL) == -1) {
        perror("Error with SIGUSR1");
    }
if (sigaction(SIGINT, &action_handle, NULL) == -1) {
        perror("Error: cannot handle SIGINT");
    }

printf("\nProcess PID-->%d",getpid());

while(1)
{
//printf("\nProcess PID-->%d",getpid());
printf("\nWaiting for Input");
sleep(5);}
return(0);
}

void handle_signal1(int signal) {
    const char *signal_name;
    time_t time_data;
    struct tm * timeinfo;
     
  time ( &time_data );
  timeinfo = localtime ( &time_data );
  //printf ( "Current local time and date: %s", asctime (timeinfo) );
  FILE *ptr;
    // Find out which signal we're handling
    if (signal==SIGUSR1)
      {signal_name = "SIGUSR1";
      printf("\nSIGUSR1 Recieved at %s",asctime (timeinfo));
      }
   
   if (signal==SIGINT)
      {signal_name = "SIGINT";
      printf("\nCtrl+C Recieved at %s",asctime (timeinfo));
      
      ptr = fopen("temp.txt", "w");
    if (ptr == NULL) {

        fputs("File does not exist", stderr);
        exit(1);
    }
    
     fprintf(ptr,"iNTERRUPT");
     printf("\nPrinting Interrupt to File\n");
     fclose(ptr);
   	action_handle.sa_handler = &handle_signal2;
   	action_handle.sa_flags = SA_RESTART;
	sigfillset(&action_handle.sa_mask);
	if (sigaction(SIGUSR1, &action_handle, NULL) == -1) {
        perror("Error with SIGUSR1");
    }
if (sigaction(SIGINT, &action_handle, NULL) == -1) {
        perror("Error: cannot handle SIGINT");
    }

      }
    
   
    
}

void handle_signal2(int signal) {
    const char *signal_name;
    time_t time_data;
    struct tm * timeinfo;
     
  time ( &time_data );
  timeinfo = localtime ( &time_data );
  
    if (signal==SIGUSR1)
      {signal_name = "SIGUSR1";
      printf("\nSIGUSR1 Recieved at %s",asctime (timeinfo));
      }
   
   if (signal==SIGINT)
      {signal_name = "SIGINT";
      printf("\nCtrl+C Recieved at %s",asctime (timeinfo));
      printf("\n I am taking rest now\n");
      action_handle.sa_handler = &handle_signal3;
      action_handle.sa_flags = SA_RESTART;
sigfillset(&action_handle.sa_mask);
if (sigaction(SIGUSR1, &action_handle, NULL) == -1) {
        perror("Error with SIGUSR1");
    }
if (sigaction(SIGINT, &action_handle, NULL) == -1) {
        perror("Error: cannot handle SIGINT");
    }

  }
      
   
    
}

void handle_signal3(int signal) {
    const char *signal_name;
    time_t time_data;
    struct tm * timeinfo;
     
  time ( &time_data );
  timeinfo = localtime ( &time_data );
  
    if (signal==SIGUSR1)
      {signal_name = "SIGUSR1";
      printf("\nSIGUSR1 Recieved at %s",asctime (timeinfo));
      }
   
   if (signal==SIGINT)
      {signal_name = "SIGINT";
      printf("\nCtrl+C Recieved at %s",asctime (timeinfo));
      printf("\n Exiting now\n");
      exit(0);
  }
      
   
    
}



