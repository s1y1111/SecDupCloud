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
	//�ļ�����Կ
	char key[322];
	//�ļ��Ĺ�ϣ�Ĺ�ϣ������λ�Է���һ��
	char id[34];
	//��־λ����־ϵͳ�Ƿ��Ѿ�ӵ�У�ӵ��Ϊ1
	char flag;
	//�ļ���
	char *filename;
	//Ҫ����ļ��Ĳ��ֵ�����ƫ��
	char filenamesize;
	int left;
	int right;
	//�ļ��Ĵ�СΪ16*sizez�ֽ�
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

//�û�����
void  handleClnt(SOCKET clntSock);

//�û������ļ��б�
int require_files(char *user, SOCKET clntSock, char *thecookies);

//�û���������б�
int require_friends(char *user, SOCKET clntSock, char *thecookies);

int add_friend(char *user, SOCKET clntSock, char *thecookies);

//��user1��user2��Ϊ���ѵ���Ϣд�뵽�ļ���
//�˴�����©�������������̵߳Ļ��⣬�����������û�����¼�ˣ�����ͬʱ��Ӻ��ѣ����������ǲ��ǻ�����ӣ������ܻ�������ص�����
//------��Ҫ������һ������̵߳Ļ��������
void writefriends(char *user1, char *user2);

//У���ļ�
int check_file(char *user, SOCKET clntSock, char *thecookies);

//�û��ϴ������ļ�������У��
int upload_partfile(char *user, SOCKET clntSock, char *thecookies, user_file *myfile);

int check_partfile(char *user, SOCKET clntSock, char *thecookies, char *checkdata,int ct);

//�ָ��ļ�
void recover_file(FILE **fd, FILE **fs, int *erase, int *erasures,int blocknum);

//�û���ʽ�ϴ��ļ�
int upload_files(char *user, SOCKET clntSock, char *thecookies, user_file *myfile);

//�û������ļ�
int download_file(char *user, SOCKET clntSock, char *thecookies);

//�û������ļ�
int share_file(char *user, SOCKET clntSock, char *thecookies);

int check_cookies(SOCKET clntSock, char *getcookies);


//���û������ļ�
void send_file(SOCKET clntSock, char *filename,int size);

//�����û��ϴ����ļ�
int recvfile(SOCKET clntSock, char *filename, int size);

void writedata(char *user, user_file *myfile);

void writeuser(struct userinf user);