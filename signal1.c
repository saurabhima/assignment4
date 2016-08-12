#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> 
#include <unistd.h>
#include<ncurses.h>
#include<time.h>

void handle_signal(int);

int main(int argc,char *argv)
{
//system("clear");
struct sigaction action_handle;
action_handle.sa_handler = &handle_signal;
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

void handle_signal(int signal) {
    const char *signal_name;
    time_t time_data;
    struct tm * timeinfo;
     
  time ( &time_data );
  timeinfo = localtime ( &time_data );
  //printf ( "Current local time and date: %s", asctime (timeinfo) );

    // Find out which signal we're handling
    if (signal==SIGUSR1)
      {signal_name = "SIGUSR1";
      printf("\nSIGUSR1 Recieved at %s",asctime (timeinfo));
      }
   
   if (signal==SIGINT)
      {signal_name = "SIGINT";
      printf("\nCtrl+C Recieved at %s",asctime (timeinfo));
      }
    
   
    
}
