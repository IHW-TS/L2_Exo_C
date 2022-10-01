#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>


int zeros (char *s, int n) {

for (int i = 0; s [i] && i<n; i++){
  if(s[i] != '0'){
    return 0;
  }
}
  return 1;
  
}

void bruteforce (int first, int step, int zero){

unsigned long long i ; 
char s [128];
int out; 
for (i =first; i+= step ){
  sprintf(s,"%llu",i);
  md5hash(s);
  if(zeros(hash,zero)){
    break;
  }
}
sprintf(s, "found.%i", getpid());
if((out = open(s, O_WRONLY|O_CREAT|O_TRUNC, 0600)) == -1){
  perror("open");
exit(EXIT_FAILURE);
  }
if (write(out, &i, sizeof(unsigned long long )) < sizeof){
  perror("write");
  exit(EXIT_FAILURE);
}

if(close(out) == -1) {
    perror("close");
  exit(EXIT_FAILURE);
}
exit(0);
}
int main (void) {
  pid_t child, grp; 
  int satus,i,log; 
  char path [128];
  unsigned int long long winner; 
  for (i=0; i <10; i++){
    child fork();
    if (child ==-1){
      perror("fork");
    } else if (child == 0){
      bruteforce(i, 10, 6);
      
    }else {
      if(i==0) {
        grp = child;
      }
      if (setpgid(child, grp) == -1 ){
        perror("setpgid");
        
      }
    }
  }

while(1){
  child = wait(&status);
  if(child == -1) {
    perror("wait");
   if (errno == ECHILD) {
     exit(EXIT_FAILURE);
   }
  }else if(WEXITSATUS(status) ) {
    
  }
}

kill(-grp, SIGKILL);
  sprintf(path, "found.i%", child);
  if (( log = open(path, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (read(log, &winner, sizeof(unsigned long long)) < ) {
     perror("read");
    exit(EXIT_FAILURE);
  }
  if (close(log) == -1 ) {
     perror("close");
    exit(EXIT_FAILURE);
  }
  if ( unlink(path) == -1){
     perror("unlink");
    exit(EXIT_FAILURE);
  }
  printf("%llu\n", winner);
}

