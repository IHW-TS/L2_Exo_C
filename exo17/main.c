#include <sys/types.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    pid_t fils[10];
    int nb = 10;

    // On lance les fils
    for (int i = 0; i < 10; ++i)
    {
        if ((fils[i] = fork()) < 0)
        {
            perror("Problème avec les fils");
        }
        else if (fils[i] == 0)
        {
            if (execl("./child", "./child", NULL) == -1)
            {
                perror("In the exec");
            }
        }
    }

    int status;
    pid_t pid;

    while (nb > 0)
    {
        // Le père attend les fils
        pid = wait(&status);
        --nb;
    }
    return 0;
}