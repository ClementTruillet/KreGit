#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"
#include "../include/affichage.h"

/*
* Clement Truillet 
* Derniere modification : 23/02/2019
*/

int main(int argc, char *argv[]){
    //Initialisation
    char LOG[128]; 
    crea_log(LOG);

    //Execution
    title();
    w_log(LOG,"La fonction main s'est bien executée.");


    return 0;
}
