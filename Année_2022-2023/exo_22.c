#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

#define MAX_MSG_LEN 256

// Structure pour les messages
typedef struct
{
    long type;              // Type de message, correspondant à l'uid du destinataire
    char data[MAX_MSG_LEN]; // Contenu du message
    time_t timestamp;       // Date d'envoi
    int sender_uid;         // Uid de l'expéditeur
} message_t;

// Fonction pour afficher une erreur et quitter le programme
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Fonction pour envoyer un message
void send_message(int msgid, int sender_uid, int recipient_uid)
{
    // Lecture du message au clavier
    printf("Entrez votre message (ligne vide pour terminer):\n");
    char message_text[MAX_MSG_LEN];
    fgets(message_text, MAX_MSG_LEN, stdin);

    // Suppression du caractère de fin de ligne
    message_text[strcspn(message_text, "\n")] = '\0';

    // Création du message
    message_t message;
    message.type = recipient_uid;
    strncpy(message.data, message_text, MAX_MSG_LEN);
    message.timestamp = time(NULL);
    message.sender_uid = sender_uid;

    // Envoi du message
    if (msgsnd(msgid, &message, sizeof(message_t) - sizeof(long), 0) == -1)
    {
        error("Erreur lors de l'envoi du message");
    }

    printf("Message envoyé.\n");
}

// Fonction pour recevoir et afficher le premier message destiné à l'utilisateur
void receive_message(int msgid, int uid)
{
    message_t message;

    // Lecture du premier message destiné à l'utilisateur
    if (msgrcv(msgid, &message, sizeof(message_t) - sizeof(long), uid, IPC_NOWAIT) == -1)
    {
        error("Erreur lors de la réception du message");
    }

    // Affichage du message
    printf("Message reçu :\n");
    printf("De : %d\n", message.sender_uid);
    printf("Date : %s", ctime(&message.timestamp));
    printf("Contenu : %s\n", message.data);
}

// Fonction pour afficher le nombre et la taille des messages en attente
void show_status(int msgid)
{
    struct msqid_ds buf;

    // Lecture des informations sur la file de messages
    if (msgctl(msgid, IPC_STAT, &buf) == -1)
    {
        error("Erreur lors de la lecture des informations sur la file de messages");
    }

    // Affichage des informations
    printf("Nombre de messages en attente : %ld\n", buf.msg_qnum);
    printf("Taille totale des messages en attente : %ld octets\n", buf.msg_qbytes);
}

// Fonction pour supprimer la file de messages
void remove_queue(int msgid)
{
    // Suppression de la file de messages
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        error("Erreur lors de la suppression de la file de messages");
    }

    printf("File de messages supprimée.\n");
}

int main(int argc, char *argv[])
{
if (argc != 2)
{
fprintf(stderr, "Usage: %s <commande>\n", argv[0]);
    exit(EXIT_FAILURE);
}

    // Récupération de l'uid de l'utilisateur
    int uid = getuid();

    // Génération de la clé pour la file de messages
    key_t key = ftok(".", 'M');
    if (key == -1)
    {
        error("Erreur lors de la génération de la clé pour la file de messages");
    }

    // Création de la file de messages si elle n'existe pas déjà
    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (msgid == -1)
    {
        if (errno == EEXIST)
        {
            // La file de messages existe déjà, on la récupère
            msgid = msgget(key, 0666);
            if (msgid == -1)
            {
                error("Erreur lors de la récupération de la file de messages");
            }
        }
        else
        {
            error("Erreur lors de la création de la file de messages");
        }
    }
https://www.onlinegdb.com/#tab-stdin
    // Traitement de la commande
    if (strcmp(argv[1], "snd") == 0)
    {
        send_message(msgid, uid, uid);
    }
    else if (strcmp(argv[1], "rcv") == 0)
    {
        receive_message(msgid, uid);
    }
    else if (strcmp(argv[1], "stat") == 0)
    {
        show_status(msgid);
    }
    else if (strcmp(argv[1], "rm") == 0)
    {
        remove_queue(msgid);
    }
    else
    {
        fprintf(stderr, "Commande non reconnue : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}