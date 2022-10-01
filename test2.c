#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
extern int errno;

pid_t isparent = 0;
int aleaNum = 0;
int sigtermCalls = 0;
int proamount = 10;
int sigcount = 0;

void handleSIGTERM (int sig)
{
  sigtermCalls++;
  if (sigtermCalls == aleaNum)
  {
    exit(0);
  }
}

int main (int argc, char* argv[])
{
  for (int i = 0; i < proamount; i++)
  {
    ////launching processes
    isparent = fork();
    if (isparent == -1)//error
    {
      perror("Fork failed ! : ");
      exit(EXIT_FAILURE);
    }
    else if (isparent)//parent
    {

    }
    else//child 
    {
      break;
    }
  }

  if (isparent) //PARENT
  {
    printf("PARENT -> PID : %i GPID : %i \n", getpid(), getpgid(getpid()));

    //sending signal until finding alea number
    while (proamount > 0)
    {
      //sending
      errno = 0;
      if (kill(0, SIGTERM) == -1)//error
      {
        perror("Kill failed ! : ");
        exit(EXIT_FAILURE);
      }
      sigcount++;

      sleep(0.5); //pause to let child proceed

      //waiting for any child
      errno = 0;
      int ws = 0;
      pid_t wsp = waitpid(0, &ws, WNOHANG);
      if (wsp == -1)//error
      {
        perror("waiting failed ! : ");
        exit(EXIT_FAILURE);
      }
      else if (wsp > 0) //child collected
      {
        //Print child PID and alea number :
        printf("PARENT child found -> PID : %i AleaNum : %i \n", wsp, sigcount);
        proamount-= 1;
      }
    }
  }
  else // CHILD
  {
    srand(getpid());
    aleaNum = rand() % 100;
    printf("PID : %i GPID : %i AleaNum : %i \n", getpid(), getpgid(getpid()), aleaNum);
    //signal handle
    struct sigaction act;
    act.sa_handler = handleSIGTERM;
    act.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &act, NULL);

    //Dodo indéfini
    while (1) {
      sleep(1); 
    }
  }
}