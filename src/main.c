#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"
#include "../include/display.h"

#define CLEAR_STDIN { int c; while((c = getchar()) != '\n' && c != EOF); }

//FSM
enum{
    TITLE,              //0 - Welcome
    ADMIN_ACCOUNT,      //1 - ADMIN - Show, create, edit and remove accounts
    ADMIN_CUSTOMER,     //2 - ADMIN - Add, edit and show customers
    ADMIN,              //3 - ADMIN - Change password
    CUSTOMER_ACCOUNT,   //4 - CUSTOMER - Consult account
    CUSTOMER,           //5 - CUSTOMER - Change password or add/remove account
    END                 //6 - Goodbye !
}FSM;

/*
* Clement Truillet 
* Derniere modification : 28/02/2019
*/

int main(int argc, char *argv[]){
    //Init
    char LOG[128]; 
    crea_log(LOG);
    int FSM = 0;

    title();


    //Execution
    while(FSM != 6){
        scanf("%d",&FSM);
        CLEAR_STDIN                                 //Clear buffer
        switch (FSM){
            case TITLE:                             //Welcome
                w_log(LOG,"Welcome page - FSM = 0");
                title();
                break;

            case ADMIN_ACCOUNT:                     //1 - ADMIN - Show, create, edit and remove accounts
                w_log(LOG,"ADMIN - FSM = 1");
                break;

            case ADMIN_CUSTOMER:                    //2 - ADMIN - Add, edit and show customers
                w_log(LOG,"ADMIN - FSM = 2");
                break;

            case ADMIN:                             //3 - ADMIN - Change password
                w_log(LOG,"ADMIN - FSM = 3");
                break;

            case CUSTOMER_ACCOUNT:                  //4 - CUSTOMER - Consult account
                w_log(LOG,"CUSTOMER - FSM = 4");
                break;

            case CUSTOMER:                          //5 - CUSTOMER - Change password or add/remove account
                w_log(LOG,"CUSTOMER - FSM = 5");
                break;
        
            case END:                               //6 - Goodbye !
                w_log(LOG,"This is the end - FSM = 6");
                end();
                break;
                    
            default:
                break;
        }
    }


    w_log(LOG,"La fonction main s'est bien executée.");


    return 0;
}