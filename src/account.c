/**
 * @file account.c
 * @author Clement Truillet (clement.truillet@univ-tlse3.fr)
 * @brief Ensemble des fonctions manipulant la structure Account
 * @version 0.1
 * @date 2019-04-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/account.h"
#include "../include/user_account.h"
#include "../include/display.h"

#ifndef COLOR
    #define color(param) printf("\033[%sm",param)
#endif

#ifndef CLEAR_STDIN
    #define CLEAR_STDIN { int c; while((c = getchar()) != '\n' && c != EOF); }
#endif

/**
 * @brief Structure account_s
 * 
 * Contient    
 *      - char * ID : Identifiant du compte   
 *      - char * type_account : Type du compte (PEL, Livret A, ...)   
 *      - Account next : Pointeur vers le compte suivant   
 * 
 */
typedef struct account_s{
	char ID[64];                //Identification
	char type_account[32];      //Type of account
	struct account_s* next;     //Next Account
} account ;

//Getters
char * get_id(Account acc){
	return acc->ID;
}
char * get_type_account(Account acc){
	return acc->type_account;
}

Account getNextAccount(Account ac){
	return ac->next;
}

//Setters
void set_ID(Account acc, char * ID){
	strcpy(acc->ID,ID);
}

void set_type_account(Account acc, char * type){
	strcpy(acc->type_account,type);
}

void InfoAccount(Account a){
	if(a!=NULL){
		printf("--------------------------\n| Compte %s\n| \tType : %s\n| \tSolde : ",get_id(a),get_type_account(a));
		if(getSolde(a)<=0){
			color("31");
		}else{
			color("32");
		}
		printf("%.2f�",getSolde(a));
		color("0");
		printf("\n--------------------------\n");
	}
}

Account verifSuppr(Account a, int *isSuppr){
    char path[64];
	FILE * fichier = NULL;
	Account aSuppr = a;
	if(aSuppr==NULL){
		return NULL;
	}
	while (aSuppr->next != NULL){
		sprintf(path,"data/account/%s.csv",get_id(aSuppr));
		fichier = fopen(path,"r");

		if(fichier==NULL){
			removeAccount(a,aSuppr);
			(*isSuppr)=1;
		}
		aSuppr=aSuppr->next;
	}
	return a;
}

Account setAccount(char * ID){
	Account ac = (Account) malloc(sizeof(struct account_s));
	char path[64];
	sprintf(path,"data/account/%s.csv",ID);

	set_ID(ac,ID);
	char temp[32]= "\0";

	strcpy(temp,ID);
	// Get the first token
	char *type = strtok(temp,"-"); // merdier token
	set_type_account(ac,type);
	ac->next=NULL;
	return (ac);
}

Account addNewAccount(Account a, Account aAdd){
	if(a==NULL){
		a=aAdd;
	}else{
		Account ac = a;
		while (ac->next != NULL){
			ac = getNextAccount(ac);
		}
		ac->next = aAdd;
	}
	return a;
}

int nbrAccount(Account a){
	if(a == NULL){
		return 0;
	}else{
		int size = 0;
		Account ac = a;             //to avoid modification on the pointeur
		while (ac != NULL){         //while we didn't reach the end of the list
			size++;
			ac = ac->next;
		}
		return size;
	}
}

char *List_accountToString(Account a){
	Account a2 = a;                                     //to avoid modification on the pointeur
	char * strR = (char *)malloc(sizeof(char)*2048);
	char str[2048] = "[";
	while(a2!=NULL){
		strcat(str,"\"");
		strcat(str,a2->ID);
		strcat(str,"\"");
		if(a2->next != NULL){
			strcat(str,",");
		}
		a2 = a2->next;
	}
	strcat(str,"]");
	strcpy(strR,str);
	return strR;
}

//file management
char * createAccountID(char * type){
	char *ID = (char*)malloc(32);
	char *timeS = (char*)malloc(32);
	time_t temps;
	struct tm date;

	// Recuperation de la date et l'heure actuelle.
	time(&temps);
	date=*localtime(&temps);

	// Remplissage de la chaine avec en date_heure
	strftime(timeS, 128, "%m%d%Y%H%M%S", &date);
	sprintf(ID,"%s-%s",type,timeS);
	
	return ID;
}

void createAccountCsv(char * ID){
	char path[64];
	sprintf(path,"data/account/%s.csv",ID);
	//r�cup�re la date et l'heure
    char *timeS = (char *)malloc(32);
    time_t temps;
    struct tm date;
    time(&temps);
    date = *localtime(&temps);
    strftime(timeS, 128, "[%d-%m-%Y|%H:%M]", &date);


	FILE * fileCSV = fopen(path, "a");
	if (fileCSV != NULL){
		fprintf(fileCSV, "date,operation,solde,comments,\n%s,0.00,0.00,, \n",timeS);
		fclose(fileCSV);
	}

	FILE * listAccount = fopen("data/account/listAccount.dat","a");
	if (listAccount != NULL){
		fprintf(listAccount, "%s \n",ID);
		fclose(listAccount);
	}
	
}

int newOperation(Account a){
	int choice, choiceVir;
	int i=0;
	float operation=0;
	char comment[512];
	char pathVir[64];
	char * idF; //Pour stocker l'ID de chaque compte
	FILE * compteVir = NULL;
	FILE * listAccount = NULL;
	Account accVir = NULL;
	char line[128];
	char lineVir[128];

	//r�cup�re la date et l'heure
    char *timeS = (char *)malloc(32);
    time_t temps;
    struct tm date;
    time(&temps);
    date = *localtime(&temps);
    strftime(timeS, 128, "[%d-%m-%Y|%H:%M]", &date);


	printf("1\\Faire un virement\t2\\Faire un depot/retrait\n");
	scanf("%d",&choice);
	CLEAR_STDIN

	switch(choice){
		case 1:	//Virement
			printf("Liste des comptes : \n");

			listAccount = fopen("data/account/listAccount.dat","r");
			if(listAccount != NULL){ //Lecture de listAccount.dat
				while (fgets(lineVir, 128, listAccount) != NULL){
					i++;
					printf("\t %d\\\t%s",i,lineVir);
				}
			}else{
				error("Impossible d'ouvrir le fichier listAccount.dat.");
			}
			printf("Choisissez un compte\n");
        	scanf("%d",&choiceVir);
			CLEAR_STDIN

			if(choiceVir>i || choiceVir<=0){
            	error("Vous avez selectionn� un compte inexistant.");
            	return 0;
        	}

			rewind(listAccount); //Revenir au d�but du fichier

			for(int k=1; k<=choiceVir; k++){ //Recuperer la bonne ligne
            	fgets(line, 128, listAccount);
        	}
        	idF = strtok(line," ");


			sprintf(pathVir,"data/account/%s.csv",idF);
			compteVir = fopen(pathVir,"r+");

			if(compteVir!=NULL){
				fclose(compteVir);
				compteVir = fopen(pathVir,"a");
				printf("Entrez le montant : ");
				scanf("%f",&operation);
				CLEAR_STDIN
				if(operation<=0){
					error("Vous ne pouvez pas faire de virement n�gatif ou nul !");
					return 0;
				}
				if(operation>getSolde(a)){
					error("Vous ne pouvez pas faire un virement superieur � votre solde !");
					return 0;
				}

				sprintf(comment,"Virement %s",get_id(a));
				accVir=setAccount(idF);
				fprintf(compteVir, "%s,%.2f,%.2f,%s,\n",timeS,operation,getSolde(accVir)+operation,comment);
				fclose(compteVir);
				operation*=-1;
				sprintf(comment,"Virement %s",idF);
			}else{
				error("Compte inexistant");
				return 0;
			}

			break;
		case 2:	//D�p�t
			printf("Entrez le montant : ");
			scanf("%f",&operation);
			CLEAR_STDIN
			printf("Entrez un commentaire : ");
			scanf("%s",comment);
			CLEAR_STDIN
			break;
		default:
			error("Choix inexistant");
			return 0;
			break;
	}
	char path[64];
	sprintf(path,"data/account/%s.csv",get_id(a));
	
	FILE * fileCSV = fopen(path, "a");
	if (fileCSV != NULL){
		fprintf(fileCSV, "%s,%.2f,%.2f,%s,\n",timeS,operation,getSolde(a)+operation,comment);
		fclose(fileCSV);
	}
	InfoAccount(a);
	return 0;
}

Account createAccount(char * ID, char * type_account){
	Account acc = (Account)malloc(sizeof(struct account_s));
	set_ID(acc,ID);
	set_type_account(acc,type_account);
	acc->next=NULL;

	return acc;
} 

float getSolde(Account a){
	FILE* fichier = NULL;
    char * soldeF;
    char chaine[256] = "";
    char line[256] = "";
	char path[256];
	sprintf(path,"data/account/%s.csv",get_id(a));
 
    fichier = fopen(path, "r");
 
    if (fichier != NULL){
        while (fgets(line, 256, fichier) != NULL) {
            strcpy(chaine,line);
            strtok(chaine,",");
            strtok(NULL,",");
            soldeF = strtok(NULL,",");
            
        }
		fclose(fichier);
		return (atof(soldeF));
    }else{
		return 0;
	}
}

Account removeAccount(Account a, Account aRmv){
	Account ac = a;
	int isRemoved = 0;
	char path[128];
	sprintf(path,"data/account/%s.csv",get_id(aRmv));
	if(get_id(a)==get_id(aRmv)){ //Si le compte aRmv supprimer est le premier
		remove(path);
		return (a->next);
	}else{
		while(isRemoved == 0 && ac->next!=NULL){
			if(get_id(ac->next)==get_id(aRmv)){
				ac->next = getNextAccount(aRmv);				
				isRemoved++;
			}
			ac = ac->next;
		}
		remove(path);
		return a;
	}
}

void removeAccountInList(Account a){
	char line[128];
	char * idF;

	FILE * fileIN = fopen("data/account/listAccount.dat","r");
    FILE * fileOUT = fopen("data/account/listAccountOUT.dat","w");

    if(fileIN != NULL){
        while (fgets(line, 128, fileIN) != NULL){
			idF = strtok(line," ");
			//printf(">>%s<< >< >>%s<<\n",idF,get_id(a));
            if(strcmp(idF,get_id(a))!=0){
				fprintf(fileOUT,"%s \n",idF);
			}
        }
    }else{
        error("Impossible d'ouvrir le fichier.");
    }
    fclose(fileIN);
    fclose(fileOUT);

    remove("data/account/listAccount.dat");
    rename("data/account/listAccountOUT.dat","data/account/listAccount.dat");
}