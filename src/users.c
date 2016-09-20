/*
 * users.c
 *
 *  Created on: 2016年6月13日
 *      Author: yuan5
 */
#include "users.h"
#include "fram.h"
#include "lcd.h"
#include "misc.h"

#define USER_DATA_START FRAM_DATA_START
#define USER_DATA_OFFSET 4

void User_init() {
	unsigned char usersNum[1];
	usersNum[0] = User_getUsersNum();
	FRAM_writeBytes(usersNum, (unsigned char *)USER_DATA_START, 1);
}

unsigned char User_getUsersNum(void) {
	unsigned char usersNum[1];
	FRAM_readBytes(usersNum, (unsigned char *)USER_DATA_START, 1);
	if(usersNum[0] == 255)
		usersNum[0] = 0;
	return usersNum[0];
}

unsigned char User_addUser(unsigned char userID[]) {
	unsigned char usersNum[1];
	FRAM_readBytes(usersNum, (unsigned char *)USER_DATA_START, 1);
	//数据检测防止重复录入
	if(User_getUserIndex(userID) != 0)
		return usersNum[0];
	unsigned int address;
	address = USER_DATA_START + usersNum[0] * USER_DATA_OFFSET + 1;
	FRAM_writeBytes(userID, (unsigned char *)address, 4);
	usersNum[0] += 1;
	FRAM_writeBytes(usersNum, (unsigned char *)USER_DATA_START, 1);

	return usersNum[0];
}

unsigned char User_getUserOnIndex(unsigned char userID[], unsigned char userIndex) {
	if(userIndex <= User_getUsersNum())
		FRAM_readBytes(
				userID,
				(unsigned char *)(USER_DATA_START + (userIndex - 1) * USER_DATA_OFFSET + 1),
				4
				);
	else
		return 0;
	return 1;
}

unsigned char User_deleteUserOnIndex(unsigned char userIndex) {
	unsigned char usersNum[1], i;
	usersNum[0] = User_getUsersNum();
	if((userIndex == 0) || (userIndex > usersNum[0]))
		return usersNum[0];
	//（伪）删除用户数据  （用后面的数据依次覆盖前一个用户数据）
	for (i = userIndex; i <= usersNum[0]; ++i) {
		FRAM_writeBytes(
						(unsigned char *)(USER_DATA_START + i * USER_DATA_OFFSET + 1),
						(unsigned char *)(USER_DATA_START + (i - 1) * USER_DATA_OFFSET + 1),
						4
						);
	}
	usersNum[0] -= 1;
	FRAM_writeBytes(usersNum, (unsigned char *)USER_DATA_START, 1);
	return usersNum[0];
}
//删除所有用户数据（伪）      令用户数量为0
void User_deleteAllUsers() {
	unsigned char usersNum[1] = {0};
	FRAM_writeBytes(usersNum, (unsigned char *)USER_DATA_START, 1);
}

unsigned char User_getUserIndex(unsigned char userID[]) {
	unsigned char i, userNum, tmp[4];
	userNum = User_getUsersNum();
	for (i = 1; i <= userNum; ++i) {
		if(User_getUserOnIndex(tmp, i))
			if(Misc_dataMatch(tmp, userID, 4))
				return i;
	}
	return 0;
}
