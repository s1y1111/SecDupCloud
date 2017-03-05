#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include<ctime>
#include<iostream>
#include<string>
#include<map>
using namespace std;

extern "C"
{
	#include<io.h>
	#include<direct.h>
	#include<time.h>
	#include<winsock2.h>
	#include <process.h>  
	#include <windows.h> 
	#include "jerasure.h"
	#include "reed_sol.h"
	#include "galois.h"
	#include "cauchy.h"
	#include "liberation.h"

}


#define BUFFERSIZE (ramp_k*blocksize)

#define SIGN_IN 8
#define SIGN_UP 9
#define SIGN_OUT 1
#define REQUIRE_FILES 2
#define REQUIRE_FRIENDS 3
#define CHECK_FILE 4
#define UPLOAD_FILE 5
#define DOWNLOAD_FILE 6
#define SHARE_FILE 7
#define ADD_FRIEND	10


#define FAILED_ACTION 100
#define SUCCESS_LOGN 101
#define GIVE_FILES 102
#define GIVE_FRIENDS 103
#define DEMMAND_FILE 104
#define SEND_FILE 105
#define GIVE_FRIEND 106
#define UPLOAD_FINISH 107
#define SUCCESS_SHARE 108

typedef struct ufile{
	//文件的密钥
	char key[322];
	//文件的哈希的哈希（多两位以防万一）
	char id[34];
	//标志位，标志系统是否已经拥有，拥有为1
	char flag;
	//文件名
	char *filename;
	//要求的文件的部分的左右偏移
	char filenamesize;
	int left;
	int right;
	//文件的大小为16*sizez字节
	int size;
} user_file;

struct userinf{
	char state;
	char userid[33];
	char userpassword[33];
	char userpkey[130];
	char namelen;
	char *username;
	FILE *user_files;
	FILE *user_friends;
	HANDLE file_lock;
	HANDLE friend_lock;
	map<string, int> have_files;
	map<string, int> have_friends;
};

int bytes_to_int(unsigned char *b);

void int_to_bytes(int i, unsigned char *b);

char *charto16(char *s);

int sign_in(char *user, SOCKET clntSock,char *cookies);

int sign_up(SOCKET clntSock);

int sign_out(char *user,SOCKET clntSock, char *cookies);

//用户处理
void  handleClnt(SOCKET clntSock);

//用户请求文件列表
int require_files(char *user, SOCKET clntSock, char *thecookies);

//用户请求好友列表
int require_friends(char *user, SOCKET clntSock, char *thecookies);

int add_friend(char *user, SOCKET clntSock, char *thecookies);

//将user1和user2互为好友的信息写入到文件中
//此处仍有漏洞，难以做到线程的互斥，假如这两个用户都登录了，两人同时添加好友，无论两者是不是互相添加，都可能会出现严重的问题
//------需要后续进一步解决线程的互斥的问题
void writefriends(char *user1, char *user2);

//校验文件
int check_file(char *user, SOCKET clntSock, char *thecookies);

//用户上传部分文件来进行校验
int upload_partfile(char *user, SOCKET clntSock, char *thecookies, user_file *myfile);

int check_partfile(char *user, SOCKET clntSock, char *thecookies, char *checkdata,int ct);

//恢复文件
void recover_file(FILE **fd, FILE **fs, int *erase, int *erasures,int blocknum);

//用户正式上传文件
int upload_files(char *user, SOCKET clntSock, char *thecookies, user_file *myfile);

//用户下载文件
int download_file(char *user, SOCKET clntSock, char *thecookies);

//用户分享文件
int share_file(char *user, SOCKET clntSock, char *thecookies);

int check_cookies(SOCKET clntSock, char *getcookies);


//向用户发送文件
void send_file(SOCKET clntSock, char *filename,int size);

//接收用户上传的文件
int recvfile(SOCKET clntSock, char *filename, int size);

void writedata(char *user, user_file *myfile);

void writeuser(struct userinf user);