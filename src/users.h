/*
 * users.h
 *
 *  Created on: 2016Äê6ÔÂ13ÈÕ
 *      Author: yuan5
 */

#ifndef USERS_H_
#define USERS_H_

void User_init();
unsigned char User_getUsersNum(void);
unsigned char User_addUser(unsigned char userID[]);
unsigned char User_getUserOnIndex(unsigned char userID[], unsigned char userIndex);
unsigned char User_getUserIndex(unsigned char userID[]);
unsigned char User_deleteUserOnIndex(unsigned char userIndex);
void User_deleteAllUsers();

#endif /* USERS_H_ */
