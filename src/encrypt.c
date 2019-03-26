#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/user_account.h"
#include "../include/encrypt.h"

/*
* Clement Truillet 
* Derniere modification : 24/03/2019
*/

//Encrypt password
char * encryptPassword(char * string){
    FILE *fp;
    char path[1035];
    char * password = NULL; //for the encryp password
    char chaine[128] = "echo "; 
    char end_chaine[128] = " | openssl md5 | cut -d\" \" -f2"; 
    //Create the command
    strcat(chaine,string); 
    strcat(chaine,end_chaine); 
    //Execute
    fp = popen(chaine, "r");
    //Reclaim the encrypt password
    fgets(path, sizeof(path)-1, fp);
    pclose(fp);
    //To remove the caracter '\n'
    password = (char *)malloc((strlen(path)*sizeof(char)));
    strncpy(password,path,strlen(path)-1);

    //OK IT'S FINISH !
    return (password);
}


//Check if the password is good
int passwordIsGood(char * password, User_account ua){
    return (strcmp(encryptPassword(password),get_pwd(ua))==0 ? 1 : 0);
}



/*
int main(int argc, char *argv[]){

    printf("Avant pwd : -admin-\n");
    printf("Après pwd : -%s-\n",encryptPassword("admin"));

    printf("\nAvant pwd : -admin123-\n");
    printf("Après pwd : -%s-\n",encryptPassword("admin123"));

    printf("\nAvant pwd : -abc-\n");
    printf("Après pwd : -%s-\n",encryptPassword("abc"));

    printf("\nAvant pwd : -a-\n");
    printf("Après pwd : -%s-\n",encryptPassword("a"));

    printf("\nAvant pwd : -azerty-\n");
    printf("Après pwd : -%s-\n",encryptPassword("azerty"));
}*/