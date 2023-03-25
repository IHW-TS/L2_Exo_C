#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
char ReturnString[255];

char * NumberToBase (int v,char baseNbr) {
    char b[64] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
    if(0 == v) {
        return "0";
    } else {
        strcpy(ReturnString,"");
        while(v > 0) {
            char x[255];
            sprintf(&x,"%c",b[v % baseNbr]);
            strcat(&x,&ReturnString);
            strcpy(&ReturnString,&x);
            v /= baseNbr;
        }
        return ReturnString;
    }
}
 
int main()
{
    char nbr;
    char base; 
    printf("Nombre a convertir :") ;
    scanf("%i",&nbr) ;
    printf("Base de conversion :") ;
    scanf("%i",&base) ;
    printf("%d = %s\n",nbr,NumberToBase(nbr,base));
    
}