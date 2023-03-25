#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX 256

struct msg_s{
    int msg_type;
    char msg_text[MAX];
};

int read_line(char text[], int n);
void writer(int msg_id);
void reader(int msg_id);
void rmv(int msg_id);


int main(){

    int msg_id;
    pid_t pid;
    msg_id = msgget(IPC_PRIVATE, 0600); // crée une nouvelle file d'attente 
    if(msg_id == -1){
        perror("msgget"); // gestion d'erreur -1
    }
    pid = fork();
    switch(pid){
        case -1:
            perror("fork");
            break;
        case 0:
            reader(msg_id);
            break;
        default:
            writer(msg_id);
            rmv(msg_id);
            break;
    }
    return 0;
}

int read_line(char text[], int n){
    char *ptext;
    int return_value;
    int length;
    ptext = fgets(text, n, stdin);
    if(ptext == NULL){
        return_value = EOF; // saisi plusieur ligne simultanement 
    }else{
        length = strlen(text);
        if(length>0 && text[length-1]=='\n'){
            text[length-1]='\0';
        }
        return_value=!EOF;
    }
    printf("nb ligne = %d\n", return_value);
    return return_value;
}

void writer(int msg_id){
    struct msg_s m;
    m.msg_type = 1;
    while(read_line(m.msg_text, MAX)!=EOF){
        int length;
        length= strlen(m.msg_text);
       //gestion d'erreur 
       if(msgsnd(msg_id, &m, length, 0)==-1){
            perror("msgsnd");
            exit(1);
        }
        if(msgsnd(msg_id, &m, 0, 0)==-1){
            perror("msgsnd");
            exit(1);
        }
    }
}

void reader(int msg_id){
    int length, n=0;
    struct msg_s m;
    while((length = msgrcv(msg_id, &m, MAX, 0, 0))>0){
        n+=length;
    }
    if(length==-1){
        perror("msgrcv");
        exit(1);
    }
    printf("Nombre d'octet = %d\n", n);
}

void rmv(int msg_id)
{
    msgctl(msg_id, IPC_RMID, NULL);
}