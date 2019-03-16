/*
 * librairie encrypt.h
 */
#define __ENCRYPT_H__

#ifndef __USER_ACCOUNT_H__
	#include "../include/user_account.h"
#endif

/*
* Clement Truillet 
* Derniere modification : 28/02/2019
*/

//Check if the password is good
//	Return 1 if password is good
//	Return 0 if ... well.. is false ?
int passwordIsGood(char * password, user_account ua);

//Encrypt password
char * encryptPassword(char * string);