#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

// ldd: -lcrypto
#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>

  char s[128];

// = {0}  =>  all the array is reset to zero (only works for zero!)
char hash[1 + 2*MD5_DIGEST_LENGTH] = {0};

char* md5hash (char *str) {
  unsigned char md5[MD5_DIGEST_LENGTH] = {0};
  MD5(str, strlen(str), md5);
  for (int i=0; i<MD5_DIGEST_LENGTH; i++) {
    sprintf(hash + 2*i, "%02x", md5[i]);
  }
  return hash;
}

int zeros (char *s, int n) {
  for (int i=0; i<n; i++) {
    if (s[i] != '0') {
      return 0;
    }
  }
  return 1;
}

void bruteforce (int first, int step, int zero) {
  unsigned long long i;
  char s[128];
  int out;
  for (i=first; ; i+=step) {
    sprintf(s, "%llu", i);
    md5hash(s);
    if (zeros(hash, zero)) {
      break;
    }
  }
  sprintf(s, "found.%i", getpid());
  if ((out = open(s, O_WRONLY|O_CREAT|O_TRUNC, 0600)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (write(out, &i, sizeof(unsigned long long)) < sizeof(unsigned long long)) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  if (close(out) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  exit(0);
}

void job(fdpipe* pipes) {
	int tid = getpid();
  
	// timer pour attendre maximum 5 secondes
	int i = 5;
	while (i > 0) {
		// lecture dans le tube
		if (read(fil[0], s, 128) > 0) {
			printf("Message du processus [%i] : %s\n", tid, s);
			// Ecriture du message dans le tableau
			sprintf(s, "je suis [%i] et j'ai bien reçu ton message !\n", tid);
			write(fil[1], s, 128);
			break;
		}
		sleep(1);
	}
}

int main(void) {
  pid_t child;
  pid_t grp = -1;
  int status, i, log;
 
  int fil[2], noct;
  
  
  unsigned long long winner;
  // fork 10 children
  for (i=0; i<10; i++) {
    
    child = fork();
    if (child == -1) {
      perror("fork");
      // don't exit since we have other children to manage
    } else if (child == 0) {
      // in child
      close(fil[0]);
      write(fil[1], bruteforce(i, 10, 6), strlen()+1));
      exit(0);

     
      
    } else {
      if (grp == -1) {
        grp = child;
      }
      if (setpgid(child, grp) == -1) {
        perror("setpgid");
      
      }
			// Ecriture du message dans le tube
			write(fil[1], s, 128);
			while (i > 0) {
				// lecture dans le tube
				if (read(fil[0], s, 128) > 0) {
					printf("Réponse du fils : %s\n", s);
					break;
				}
				sleep(1);
			}
			
    }
    waitForAll();
	return EXIT_SUCCESS;
  }


  
  // wait a winner
  while (1) {
    child = wait(&status);
    if (child == -1) {
      perror("wait");
      if (errno == ECHILD) {
        // exit if no more children is alive
        exit(EXIT_FAILURE);
      }
    } else if (WEXITSTATUS(status)) {
      // child failed
      printf("child %i exited with %i\n", child, status);
    } else {
      // child succeded
      break;
    }
  }


  
  // kill all processes in the group
  kill(-grp, SIGKILL);
  // read child file
  sprintf(path, "found.%i", child);
  if ((log = open(path, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (read(log, &winner, sizeof(unsigned long long)) < sizeof(unsigned long long)) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  if (close(log) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  if (unlink(path) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  printf("%llu\n", winner);
  return 0;
}


// https://stackoverflow.com/questions/38058319/creating-pipe-between-father-and-child-process