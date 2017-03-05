#include"handle.h"

//Ҫ���̻߳���
HANDLE filelock;
HANDLE userlock;

map<string, int> all_files;

map<string, struct userinf> all_users;

FILE *all_files_fp;
FILE *all_users_fp;


//�ļ���Ϊk��
int ramp_k;

//����m��������
int ramp_m;

//word size
int ramp_w;

int blocksize;
int packetsize;


int *matrix;
int *bitmatrix;
int **schedule;


int bytes_to_int(unsigned char *b)
{
	int i = 0;
	for (int j = 3; j >= 0; j--)
	{
		i=i << 8;
		i += b[j];
	}
	return i;
}

void int_to_bytes(int i,unsigned char *b)
{
	memset(b, 0, sizeof(unsigned char)* 4);
	for (int j = 0; j < 4; j++)
	{
		b[j] = i % 256;
		i = i / 256;
	}
	return ;
}

void randomiv(char *iv)
{
	srand((unsigned int)time(0));
	for (int i = 0; i < 16; i++)
		iv[i] = (char)(rand() % 256);
	return;
}

char *charto16(char *s)
{
	char *c = (char *)malloc(66 * sizeof(char));
	for (int i = 0; i < 32; i++)
		sprintf_s(c + 2 * i, 66 - 2 * i, "%02x ", (unsigned char)s[i]);
	c[64] = 0;
	return c;
}

void  handleClnt(SOCKET clntSock)
{
	char *buffer=(char *)malloc(1*sizeof(char));
	//��ʱ���û���ʾ��
	char cookies[32];
	//�û��ı�ţ�-1��ʾû���û���¼
	char user[33];
	memset(user, 0, 33 * sizeof(char));
	//�յ����ֽ���
	int readnum = 0;
	int flag = 1;

	//�����ø��̴߳�����û��Ƿ��¼��
	int state = 0;
	while (flag)
	{
		//printf("���߳�%d\n", GetCurrentThreadId());
		//printf("�ȴ�");
		readnum = recv(clntSock, buffer, 1, NULL);
		if (readnum <= 0)
		{
			printf("�û��ѶϿ�����!\n");
			closesocket(clntSock);
			flag = 0;
			return;
		}
		else
		{
			switch (buffer[0]){
			case SIGN_IN:
				if (sign_in(user, clntSock, cookies))
				{
					printf("һ�û��ѵ�¼��\n");
					state = 1;
				}
				break;
			case SIGN_UP:
				if (sign_up(clntSock))
				{
					printf("һ�û���ע�ᣡ\n");
				}
				break;
			case SIGN_OUT:
				if (sign_out(user, clntSock, cookies))
				{
					state = 0;
					printf("һ�û����˳�...\n");
				}
				break;
			case REQUIRE_FILES:
				if (!(state && require_files(user, clntSock, cookies)))
				{
					flag=0;
					printf("һ�û������ļ��б�ʧ�ܣ�\n");
				}
				else
				{
					printf("һ�û���ȡ���ļ��б�\n");
				}
				break;
			case REQUIRE_FRIENDS:
				if (!(state && require_friends(user, clntSock, cookies)))
				{
					flag = 0;
					printf("һ�û�ȡ�ú����б�ʧ�ܣ�\n");
				}
				else
				{
					printf("һ�û���ȡ�ú����б�\n");
				}
				break;
			case ADD_FRIEND:
				if (!(state && add_friend(user, clntSock, cookies)))
				{
					flag = 0;
					printf("һ�û����һ����ʧ�ܣ�\n");
				}
				else
				{
					printf("һ�û������һ���ѣ�\n");
				}
				break;
			case CHECK_FILE:
				if (!(state && check_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("һ�û��ϴ��ļ�ʧ�ܣ�\n");
				}
				break;
			case DOWNLOAD_FILE:
				if (!(state && download_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("һ�û����ܾ������ļ�\n");
				}
				else
				{
					printf("һ�û��������ļ�\n");
				}
				break;
			case SHARE_FILE:
				if (!(state && share_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("һ�û������ļ�ʧ�ܣ�\n");
				}
				else
				{
					printf("һ�û������ļ��ɹ���\n");
				}
				break;
			default:
				flag = 0;
				break;
			}
		}
	}
	printf("�û����Ϸ�����Ϊ�������ӱ������رգ�\n");
	closesocket(clntSock);
	free(buffer);
}

//�û��ĵ�¼
int sign_in(char *user,SOCKET clntSock, char *cookies)
{
	char userid[33];
	char userpassword[33];
	int readnum = 0;
	//��ȡ�û����Ĺ�ϣ
	readnum = recv(clntSock, userid, 32, NULL);
	while (readnum != 32)
	{
		if (readnum <= 0)
			return 0;
		readnum += recv(clntSock, userid + readnum, 32 - readnum, NULL);
	}
	userid[32] = 0;

	//��ȡ����
	readnum = recv(clntSock, userpassword, 32, NULL);
	while (readnum != 32)
	{
		if (readnum <= 0)
			return 0;
		readnum += recv(clntSock, userpassword + readnum, 32 - readnum, NULL);
	}
	userpassword[32] = 0;
	//�жϸ��û�������������ȷ�Ҹ��û�û�е�¼
	if (all_users.count(userid) != 0 && strcmp(all_users[userid].userpassword, userpassword) == 0 && all_users[userid].state==0)
	{
		char f[1];
		f[0] = SUCCESS_LOGN;
		randomiv(cookies);
		randomiv(cookies+16);
		send(clntSock, f, 1, 0);
		send(clntSock, cookies, 32, 0);
		for (int i = 0; i < 32; i++)
			user[i] = userid[i];
		user[32] = 0;
		return 1;
	}
	else
	{
		char f[1];
		f[0] = FAILED_ACTION;
		send(clntSock, f, 1, 0);
		return 0;
	}
}


//�û�ע��
int sign_up(SOCKET clntSock)
{
	char uname[33];
	
	int readnum;
	//�����û������ֹ�ϣ
	readnum = recv(clntSock, uname, 32, NULL);
	while (readnum != 32)
	{
		if (readnum <= 0)
		{
			return 0;
		}
		readnum += recv(clntSock, all_users[uname].userid + readnum, 32 - readnum, NULL);
	}
	uname[32] = 0;

	//��������û������ڵĻ�������ע��
	if (all_users.count(uname) == 0)
	{
		all_users[uname].state = 0;
		memcpy_s(all_users[uname].userid, 32, uname, 32);
		//�����û�������
		readnum = recv(clntSock,all_users[uname].userpassword, 32, NULL);
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				return 0;
			}
			readnum += recv(clntSock, all_users[uname].userpassword + readnum, 32 - readnum, NULL);
		}
		all_users[uname].userpassword[32] = 0;

		//�����û������ֳ��Ⱥ�����
		recv(clntSock, &(all_users[uname].namelen), 1, 0);
		all_users[uname].username = (char *)malloc((unsigned char)all_users[uname].namelen*sizeof(char));
		readnum = recv(clntSock, all_users[uname].username, (unsigned char)all_users[uname].namelen, NULL);
		while (readnum != (unsigned char)all_users[uname].namelen)
		{
			if (readnum <= 0)
			{
				return 0;
			}
			readnum += recv(clntSock, all_users[uname].username + readnum, (unsigned char)all_users[uname].namelen - readnum, NULL);
		}

		//�����û��Ĺ�Կ
		readnum = recv(clntSock, all_users[uname].userpkey, 130, NULL);
		while (readnum != 130)
		{
			if (readnum <= 0)
			{
				return 0;
			}
			readnum += recv(clntSock, all_users[uname].userpkey + readnum, 130 - readnum, NULL);
		}
		char tempname[1000];
		sprintf_s(tempname, 1000, "user_file/%s", charto16(all_users[uname].userid));
		fopen_s(&(all_users[uname].user_files), tempname, "ab+");
		writeuser(all_users[uname]);
		return 1;
	}
	return 0;
}

//���ļ���д���û�����Ϣ
void writeuser(struct userinf user)
{
	//��֤д���ļ���ͬ��,�ȴ�������
	WaitForSingleObject(userlock, INFINITE);
	printf("д��һ���û�\n");
	fseek(all_users_fp, 0, SEEK_END);
	fwrite(user.userid, sizeof(char), 32, all_users_fp);
	fwrite(user.userpassword, sizeof(char), 32, all_users_fp);
	fwrite(user.userpkey, sizeof(char), 130, all_users_fp);
	fwrite(&(user.namelen), sizeof(char), 1, all_users_fp);
	fwrite(user.username, sizeof(char), (unsigned char)user.namelen, all_users_fp);
	fflush(all_users_fp);
	//�ͷŻ�����
	ReleaseMutex(userlock);
}

int sign_out(char *user,SOCKET clntSock, char *cookies)
{
	if (check_cookies(clntSock, cookies))
	{
		all_users[user].state = 0;
		return 1;
	}
	else
		return 0;
}


//��������֡��ʽ����־ͷ(1)|�������Ϣ����(4)|�ļ�������(1)|�ļ���(?)|�ļ���С(4)|�ļ���ϣ��ϣֵ(32)|......
//�û������ļ��б�
int require_files(char *user, SOCKET clntSock, char *thecookies)
{
	char *buffer;
	int readnum = 0;

	int sendnum = 0;
	if (check_cookies(clntSock, thecookies))
	{
		int n = 0;
		//�ȴ�������
		WaitForSingleObject(all_users[user].file_lock, INFINITE);
		sendnum++;
		//�ļ����ʹ�С���ܳ���+200����ֹ���������
		fseek(all_users[user].user_files, 0, SEEK_END);
		int fsize = ftell(all_users[user].user_files);
		buffer = (char *)malloc((fsize+200)*sizeof(char));
		memset(buffer, 0, sizeof(char)*(fsize + 200));

		buffer[0] = (char)GIVE_FILES;
		fseek(all_users[user].user_files, 0, SEEK_SET);
		//�����ĸ��ֽ���Ϊ�ļ�����Ŀ
		sendnum += 4;
		//�ļ�����С
		while (fread(buffer + sendnum, sizeof(char), 1, all_users[user].user_files) != 0)
		{
			n++;
			sendnum += 1;

			//�ļ���
			sendnum += fread(buffer + sendnum, sizeof(char), bytes_to_int((unsigned char *)buffer + sendnum - 1), all_users[user].user_files);
			//�ļ���С

			sendnum += fread(buffer + sendnum, sizeof(char), 4, all_users[user].user_files);
			//�ļ���Ψһ��ʾ������ϣ�Ĺ�ϣ

			sendnum += fread(buffer + sendnum, sizeof(char), 32, all_users[user].user_files);
			(all_users[user].have_files)[buffer + sendnum - 32] = 1;
			//�����û�˽Կ���ܵ��ļ��Ĺ�ϣ

			sendnum += fread(buffer + sendnum, sizeof(char), 322, all_users[user].user_files);

		}
		int_to_bytes(n,(unsigned char *)(buffer+1));
		send(clntSock, buffer, sendnum, 0);
		ReleaseMutex(all_users[user].file_lock);
		free(buffer);
		return 1;
	}
	return 0;
}

int require_friends(char *user, SOCKET clntSock, char *thecookies)
{
	char *buffer;
	int readnum = 0;

	int sendnum = 0;
	if (check_cookies(clntSock, thecookies))
	{
		int n = 0;
		sendnum++;
		WaitForSingleObject(all_users[user].friend_lock, INFINITE);
		//�ļ����ʹ�С���ܳ���+200����ֹ���������
		fseek(all_users[user].user_friends, 0, SEEK_END);
		int fsize = ftell(all_users[user].user_friends);
		buffer = (char *)malloc((fsize + 200)*sizeof(char));
		memset(buffer, 0, sizeof(char)*(fsize + 200));

		buffer[0] = (char)GIVE_FILES;
		fseek(all_users[user].user_friends, 0, SEEK_SET);
		//�����ĸ��ֽ���Ϊ���ѵ���Ŀ
		sendnum += 4;
		//��������С
		while (fread(buffer + sendnum, sizeof(char), 1, all_users[user].user_friends) != 0)
		{
			n++;
			sendnum++;
			//������
			sendnum += fread(buffer + sendnum, sizeof(char), bytes_to_int((unsigned char *)buffer + sendnum -1), all_users[user].user_friends);
			//�������Ĺ�ϣ
			sendnum += fread(buffer + sendnum, sizeof(char), 32, all_users[user].user_friends);
			(all_users[user].have_friends)[buffer + sendnum - 32] = 1;
			//���ѵĹ�Կ
			sendnum += fread(buffer + sendnum, sizeof(char), 130, all_users[user].user_friends);
		}
		int_to_bytes(n, (unsigned char *)(buffer + 1));
		send(clntSock, buffer, sendnum, 0);
		ReleaseMutex(all_users[user].friend_lock);
		free(buffer);
		return 1;
	}
	return 0;
}

//�û���Ӻ��ѣ�ֻ��Ҫ���͸ú��ѵ����ֵĹ�ϣ�Ϳ����ˣ��᷵�ظ��û����ѵĹ�Կ
int add_friend(char *user, SOCKET clntSock, char *thecookies)
{
	char *buffer;
	if (check_cookies(clntSock, thecookies))
	{
		buffer = (char *)malloc(350 * sizeof(char));
		//��ȡ�õ��û�����ӵĺ��ѵ����ֵĹ�ϣ
		int readnum = recv(clntSock, buffer, 32, 0);
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				free(buffer);
				return 0;
			}
			readnum += recv(clntSock, buffer + readnum, 32 - readnum, NULL);
		}
		buffer[32] = 0;
		if (all_users.count(buffer) != 0 && all_users[user].have_friends.count(buffer) == 0)
		{
			//�����߻�Ϊ���ѵ���Ϣд�뵽�ļ���ȥ
			writefriends(user, buffer);
			(all_users[buffer].have_friends)[user] = 1;
			(all_users[user].have_friends)[buffer] = 1;
			//���ظ��û����ѵĹ�Կ
			memcpy_s(buffer + 1,130, all_users[buffer].userpkey,130);
			buffer[0] = GIVE_FRIEND;
			send(clntSock, buffer, 131, 0);
		}
		else
		{
			buffer[0] = FAILED_ACTION;
			send(clntSock, buffer, 1, 0);
		}
		free(buffer);
		return 1;
	}
	else
	{
		return 0;
	}
}

//��user1��user2��Ϊ���ѵ���Ϣд�뵽�ļ���
//�˴�����©�������������̵߳Ļ��⣬�����������û�����¼�ˣ�����ͬʱ��Ӻ��ѣ����������ǲ��ǻ�����ӣ������ܻ�������ص�����
//------��Ҫ������һ������̵߳Ļ��������
void writefriends(char *user1, char *user2)
{
	char *buffer;
	char *c=(char *)malloc(1*sizeof(char));
	struct userinf *u1 = &(all_users[user1]);
	struct userinf *u2 = &(all_users[user2]);
	//u1Ϊu2�ĺ���
	WaitForSingleObject(all_users[user2].friend_lock, INFINITE);
	fseek(u2->user_friends, 0, SEEK_END);
	buffer = (char *)malloc(((unsigned char)u1->namelen + 300)*sizeof(char));
	buffer[0] = u1->namelen;
	memcpy_s(buffer	+ 1, 256, u1->username, (unsigned char)u1->namelen);
	memcpy_s(buffer + 1 + (unsigned char)u1->namelen, 32, u1->userid, 32);
	memcpy_s(buffer + 33 + (unsigned char)u1->namelen, 130, u1->userpkey, 130);
	//�ȴ�������
	fwrite(buffer, sizeof(char), (unsigned char)u1->namelen + 163, u2->user_friends);
	fflush(u2->user_friends);

	free(buffer);
	ReleaseMutex(all_users[user2].friend_lock);

	//u2Ϊu1�ĺ���
	//�ȴ�������
	WaitForSingleObject(all_users[user1].friend_lock, INFINITE);
	fseek(u1->user_friends, 0, SEEK_END);
	buffer = (char *)malloc(((unsigned char)u2->namelen + 300)*sizeof(char));
	buffer[0] = u2->namelen;
	memcpy_s(buffer + 1, 256, u2->username, (unsigned char)u2->namelen);
	memcpy_s(buffer + 1 + (unsigned char)u2->namelen, 32, u2->userid, 32);
	memcpy_s(buffer + 33 + (unsigned char)u2->namelen, 130, u2->userpkey, 130);
	fwrite(buffer, sizeof(char), (unsigned char)u2->namelen + 163, u1->user_friends);
	fflush(u2->user_friends);
	ReleaseMutex(all_users[user1].friend_lock);


	free(buffer);
	free(c);
	return;
}

//У���ļ�
int check_file(char *user, SOCKET clntSock, char *thecookies)
{
	char *c = (char *)malloc(sizeof(char)* 4);
	char *buffer;
	buffer = (char *)malloc(50*sizeof(char));
	memset(buffer, 0, sizeof(char)*50);
	user_file *myfile = (user_file *)malloc(sizeof(user_file));
	memset(myfile, 0, sizeof(user_file));
	int readnum = 0;
	

	int sendnum = 0;
	//�û���ݺϷ�
	if (check_cookies(clntSock, thecookies))
	{
		//��ȡ�ļ�����С
		recv(clntSock, c, 1, 0);
		myfile->filename = (char *)malloc((unsigned char)c[0]*sizeof(char));
		myfile->filenamesize = c[0];
		//��ȡ�ļ���
		readnum = recv(clntSock, myfile->filename, (unsigned char)c[0], 0);
		while (readnum != (unsigned char)c[0])
		{
			if (readnum <= 0)
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->filename + readnum, (unsigned char)c[0] - readnum, NULL);
		}
		//��ȡ�ļ���С
		readnum = recv(clntSock, c, 4, 0);
		while (readnum != 4)
		{
			if (readnum <= 0)
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, c + readnum, 4 - readnum, NULL);
		}
		myfile->size = bytes_to_int((unsigned char *)c);
		//��ȡ�ļ��Ĺ�ϣ�Ĺ�ϣ
		readnum = recv(clntSock, myfile->id, 32, 0);
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->id + readnum, 32 - readnum, NULL);
		}
		(myfile->id)[32] = 0;
		//��ȡ�������ܵ��ļ���Կ
		readnum = recv(clntSock, myfile->key, 322, 0);
		while (readnum != 322)
		{
			if (readnum <= 0)
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->key + readnum, 322 - readnum, NULL);
		}
		if (all_files.count(myfile->id) == 1)
		{
			//�ļ��Ѿ�����
			myfile->flag = 1;
			srand((unsigned int)time(0));
			myfile->left = ((((int)rand()) << 8) + rand()) % myfile->size;
			int temp = rand() % 32;
			myfile->right = (myfile->left + temp) >= myfile->size ? myfile->size-1 : myfile->left + temp;
			if (upload_partfile(user, clntSock, thecookies, myfile))
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 1;
			}
			else
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
		}
		else
		{
			//�ļ�������
			myfile->flag = 0;
			buffer[0] = DEMMAND_FILE;
			randomiv(buffer + 1);
			int_to_bytes(0, (unsigned char *)(buffer + 17));
			int_to_bytes(myfile->size, (unsigned char *)(buffer + 21));
			send(clntSock, buffer, 25, 0);
			recv(clntSock, c, 1, 0);
			if ((unsigned char)c[0] == UPLOAD_FILE&&upload_files(user, clntSock, thecookies, myfile)==1)
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 1;
			}
			else
			{
				free(c);
				free(buffer);
				free(myfile->filename);
				free(myfile);
				return 0;
			}
		}
	}
	else
	{
		free(c);
		free(buffer);
		free(myfile);
		printf("У���ļ�ʱ�û���ݲ��Ϸ�\n");
		return 0;
	}
}

int upload_files(char *user, SOCKET clntSock, char *thecookies, user_file *myfile)
{
	int readnum = 0;
	if (check_cookies(clntSock, thecookies) && recvfile(clntSock, myfile->id, myfile->size))
	{
		//�������ļ���Ӧ�ý����ļ���Ϣд���û������ݺ�ϵͳ��������
		writedata(user, myfile);
		char c[1];
		c[0] = UPLOAD_FINISH;
		send(clntSock, c, 1, 0);
		return 1;
	}
	else
	{
		printf("�û��ϴ��ļ�ʱ��ݲ��Ϸ���\n");
		return 0;
	}
}

int upload_partfile(char *user, SOCKET clntSock, char *thecookies, user_file *myfile)
{
	//k���ֿ�����ļ���ָ������
	FILE **fd;

	//m���������ļ���ָ������
	FILE **fs;


	//��Ϊk����ļ������������ָ��
	char **data;

	//��Ϊm���������������ָ��
	char **coding;

	//��������ָ�룬���k��blocksize��С���ļ��������ݣ�Ҳ����data����
	char *buffer;

	char *checkdata;

	//�ļ����ֵĿ������
	int blocknum;

	//ȱʧ�Ŀ�ı�����飬���һλΪ-1
	int erasures[100];

	//��ʾȱʧ�Ŀ�����飬ĳ����Ϊ1˵�����±��Ӧ�Ŀ�ȱʧ
	int erase[100];

	int e = 0;
	//��ʱ�ļ���
	char fname[1000];

	int size = myfile->size * 32;
	//printf("��֤��ʼ��%ld\n", clock());
	memset(erase, 0, sizeof(char)*100);
	memset(erasures, 0, sizeof(char)* 100);

	data = (char **)malloc(sizeof(char*)*ramp_k);
	coding = (char **)malloc(sizeof(char*)*ramp_m);

	fd = (FILE **)malloc(sizeof(FILE*)*ramp_k);
	fs = (FILE **)malloc(sizeof(FILE*)*ramp_m);

	for (int i = 0; i < ramp_m; i++)
	{
		coding[i] = (char *)malloc(sizeof(char)*blocksize);
		memset(coding[i], 0, sizeof(char)*ramp_w*packetsize);
	}
	//��֤�ļ��еĴ���
	char mname[256];
	if (_access("user_file", 0) == -1)
	{
		_mkdir("user_file");
	}
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(mname, 100, "file_k_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(mname, 100, "file_m_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}

	//�򿪸��ֿ�������������ļ�
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(myfile->id));
		fopen_s(fd + i, fname, "rb");
		//���blocknum
		if (*(fd + i) != NULL)
		{
			fseek(*(fd + i), 0, SEEK_END);
			blocknum = ftell(*(fd + i)) / blocksize;
			fseek(*(fd + i), 0, SEEK_SET);
		}
		else{
			erasures[e++] = i;
			erase[i] = 1;
			sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(myfile->id));
			fopen_s(fd + i, fname, "wb");
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(fname, 100, "file_m_%d/%s", i, charto16(myfile->id));
		fopen_s(fs + i, fname, "rb");
		if (*(fs + i) != NULL)
		{
			fseek(*(fs + i), 0, SEEK_END);
			blocknum = ftell(*(fs + i)) / blocksize;
			fseek(*(fs + i), 0, SEEK_SET);
		}
		else{
			erasures[e++] = i + ramp_k;
			erase[i + ramp_k] = 1;
			sprintf_s(fname, 100, "file_m_%d/%s", i, charto16(myfile->id));
			fopen_s(fs + i, fname, "wb");
		}
	}
	//��ý������ļ��Ĵ�С
	size = blocknum*blocksize*ramp_k;
	erasures[e] = -1;

	int xl = (myfile->left) * 16 / (blocksize*ramp_k);
	int xr;
	if ((myfile->left) * 16 % (blocksize*ramp_k) == 0)
		xl--;
	if (xl < 0)
		xl++;
	xr = xl + 1;

	if (e == ramp_m + ramp_k)
	{

		for (int i = 0; i < ramp_k; i++)
		{
			fflush(fd[i]);
			fclose(fd[i]);
		}
		free(fd);
		for (int i = 0; i < ramp_m; i++)
		{
			fflush(fs[i]);
			fclose(fs[i]);
		}
		free(fs);

		free(data);

		for (int i = 0; i < ramp_m; i++)
			free(coding[i]);
		free(coding);

		buffer = (char *)malloc(50 * sizeof(char));
		char *c = (char *)malloc(sizeof(char)* 4);
		memset(buffer, 0, sizeof(char)* 50);
		buffer[0] = DEMMAND_FILE;
		randomiv(buffer + 1);
		int_to_bytes(0, (unsigned char *)(buffer + 17));
		int_to_bytes(myfile->size, (unsigned char *)(buffer + 21));
		send(clntSock, buffer, 25, 0);
		recv(clntSock, c, 1, 0);
		if ((unsigned char)c[0] == UPLOAD_FILE&&upload_files(user, clntSock, thecookies, myfile) == 1)
		{
			free(c);
			free(buffer);
			free(myfile->filename);
			free(myfile);
			return 1;
		}
		else
		{
			free(c);
			free(buffer);
			free(myfile->filename);
			free(myfile);
			return 0;
		}
	}

	//������ڿ�ȱʧ�Ļ���Ҫ�Ȱ�ȱʧ�Ŀ鲹��
	if (erasures[0] != -1)
	{
		recover_file(fd,fs,erase,erasures,blocknum);
	}

	checkdata = (char *)malloc(sizeof(char)* 2 * blocksize*ramp_k);
	buffer = (char *)malloc(sizeof(char)* blocksize*ramp_k);
	int ct = xr - xl;
	for (int i = 0; i < ramp_k; i++)
	{
		//fseek�ƶ�����ȥ�����Բ��ùر��ļ��ٴ򿪵ķ�ʽ
		//fseek(fd[i], xl*blocksize, SEEK_SET);
		/*printf("%d\n", fseek(fd[i], xl*blocksize, SEEK_SET));*/
		fclose(fd[i]);
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(myfile->id));
		fopen_s(fd + i, fname, "rb");
		fseek(fd[i], xl*blocksize, SEEK_SET);
	}
	memset(buffer, 0xff, sizeof(char)*blocksize*ramp_k);
	while (ct--)
	{
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
		for (int i = 0; i < ramp_k; i++)
		{
			fread(buffer + i*blocksize, sizeof(char), blocksize, fd[i]);
			//printf("%s\n", charto16(buffer + i*blocksize));
		}
		int n = blocksize*ramp_k;
		memcpy_s(checkdata + ct*blocksize*ramp_k, blocksize*ramp_k, buffer, blocksize*ramp_k);
	}

	xl = ((myfile->left - 1) * 16 + blocksize*ramp_k) % (blocksize*ramp_k);
	char *sbuffer;
	//���������ļ�ĳ���ֵ��ļ�֡
	sbuffer = (char *)malloc(50 * sizeof(char));
	sbuffer[0] = DEMMAND_FILE;
	memcpy_s(sbuffer + 1, 16, checkdata + xl, 16);
	int_to_bytes(myfile->left, (unsigned char *)(sbuffer + 17));
	int_to_bytes(myfile->right, (unsigned char *)(sbuffer + 21));
	//printf("%s\n", charto16(checkdata + xl));
	send(clntSock, sbuffer, 25, 0);
	char c[1];
	recv(clntSock, c, 1, 0);
	int ret;
	
	//֡��ʽ��ȷ������֤�ļ���ȷ����д������
	if ((unsigned char)c[0] == UPLOAD_FILE&&check_partfile(user, clntSock, thecookies, checkdata + xl+16, myfile->right - myfile->left + 1) == 1)
	{
		writedata(user, myfile);
		char c[1];
		c[0] = UPLOAD_FINISH;
		send(clntSock, c, 1, 0);
		ret = 1;
		//printf("��֤������%ld\n", clock());
	}
	else
	{
		ret = 0;
	}
	free(buffer);
	free(sbuffer);

	free(checkdata);

	for (int i = 0; i < ramp_k; i++)
	{
		fflush(fd[i]);
		fclose(fd[i]);
	}
	free(fd);
	for (int i = 0; i < ramp_m; i++)
	{
		fflush(fs[i]);
		fclose(fs[i]);
	}
	free(fs);

	free(data);

	for (int i = 0; i < ramp_m; i++)
		free(coding[i]);
	free(coding);

	return ret;
}

//�ָ��ļ�
void recover_file(FILE **fd, FILE **fs, int *erase, int *erasures,int blocknum)
{
	//��Ϊk����ļ������������ָ��
	char **data;

	//��Ϊm���������������ָ��
	char **coding;

	//��������ָ�룬���k��blocksize��С���ļ��������ݣ�Ҳ����data����
	char *buffer;
	//printf("�ָ���ʼ��%ld\n", clock());

	data = (char **)malloc(sizeof(char*)*ramp_k);
	coding = (char **)malloc(sizeof(char*)*ramp_m);
	buffer = (char *)malloc(ramp_k*blocksize);

	for (int i = 0; i < ramp_m; i++)
	{
		coding[i] = (char *)malloc(sizeof(char)*blocksize);
		memset(coding[i], 0, sizeof(char)*ramp_w*packetsize);
	}
	

	while (blocknum--)
	{
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
		//��ȡ���ݵ�data��coding
		for (int i = 0; i < ramp_k; i++)
		{
			if (erase[i] == 0)
				fread(buffer + i*blocksize, sizeof(char), blocksize, fd[i]);
			data[i] = buffer + i*blocksize;
		}
		for (int i = 0; i < ramp_m; i++)
		{
			if (erase[i + ramp_k] == 0)
				fread(coding[i], sizeof(char), blocksize, fs[i]);
		}

		jerasure_schedule_decode_lazy(ramp_k, ramp_m, ramp_w, bitmatrix, erasures, data, coding, packetsize*ramp_w, packetsize, 1);

		//���������ݿ�����鶪ʧ�Ļ����ָ���Щ��
		for (int i = 0; i < ramp_k; i++)
		{
			if (erase[i] != 0)
				fwrite(data[i], sizeof(char), blocksize, fd[i]);
		}
		for (int i = 0; i < ramp_m; i++)
		{
			if (erase[i + ramp_k] != 0)
				fwrite(coding[i], sizeof(char), blocksize, fs[i]);
		}
	}
	//printf("�ָ�������%ld\n", clock());
	for (int i = 0; i < ramp_k; i++)
	{
		fflush(fd[i]);
	}
	for (int i = 0; i < ramp_m; i++)
	{
		fflush(fs[i]);
	}

	free(data);

	for (int i = 0; i < ramp_m; i++)
		free(coding[i]);
	free(coding);

	free(buffer);
}

//���һ���ֵ��ļ��Ƿ���ͬ
int check_partfile(char *user, SOCKET clntSock, char *thecookies, char *checkdata,int ct)
{

	int readnum = 0;
	
	char *recvdata;
	recvdata = (char *)malloc(sizeof(char)*ct * 16);
	if (check_cookies(clntSock, thecookies))
	{
		//���մ����Ĳ����ļ�
		readnum = recv(clntSock, recvdata, ct * 16, 0);
		while (readnum != ct * 16)
		{
			if (readnum <= 0)
			{
				free(recvdata);
				return 0;
			}
			readnum += recv(clntSock, recvdata + readnum, ct * 16 - readnum, NULL);
		}
		/*printf("%s\n", charto16(checkdata));
		printf("-%s\n", charto16(recvdata));*/
		//����ļ��Ƿ���ͬ
		for (int i = 0; i < ct * 16; i++)
		{
			if (checkdata[i] != recvdata[i])
			{
				return 0;
			}
		}
		free(recvdata);
		return 1;
	}
	else
	{
		free(recvdata);
		printf("�û���ݲ��Ϸ���\n");
		return 0;
	}
}

void writedata(char *user, user_file *myfile)
{
	if (all_users[user].have_files.count(myfile->id) != 0)
		return;
	//���ļ�д�뵽�û���Ϣ��
	char buffer[4];

	//��֤�û��ļ���Ϣ�����Ļ��⣬��ֹ���û���������û�ʱ��������
	WaitForSingleObject(all_users[user].file_lock, INFINITE);
	fseek(all_users[user].user_files, 0, SEEK_END);
	fwrite(&(myfile->filenamesize), sizeof(char), 1, all_users[user].user_files);
	fwrite(myfile->filename, sizeof(char), (unsigned char)myfile->filenamesize, all_users[user].user_files);
	int_to_bytes(myfile->size, (unsigned char *)buffer);
	fwrite(buffer, sizeof(char), 4, all_users[user].user_files);
	fwrite(myfile->id, sizeof(char), 32, all_users[user].user_files);
	fwrite(myfile->key, sizeof(char), 322, all_users[user].user_files);
	(all_users[user].have_files)[myfile->id] = 1;
	fflush(all_users[user].user_files);
	ReleaseMutex(all_users[user].file_lock);

	//��֤д���ļ���ͬ��,�ȴ�������
	WaitForSingleObject(filelock, INFINITE);
	printf("һ�û����ϴ������һ���ļ�,");
	//����ϵͳû�еĻ�����д�뵽ϵͳ���ļ���Ϣ��
	if (myfile->flag != 1)
	{
		fseek(all_files_fp, 0, SEEK_END);
		fwrite(myfile->id, sizeof(char), 32, all_files_fp);
		fwrite(buffer, sizeof(char), 4, all_files_fp);
		all_files[myfile->id] = myfile->size;
		fflush(all_files_fp);
		printf("�Ҵ��ļ�ϵͳû��\n");
	}
	else
		printf("�Ҵ��ļ�ϵͳ����\n");
	//�ͷŻ�����
	//printf("%d���ļ�\n", all_files.size());
	ReleaseMutex(filelock);
}

int download_file(char *user, SOCKET clntSock, char *thecookies)
{
	char downfile[33];
	
	memset(downfile, 0, 33 * sizeof(char));
	int readnum = 0;
	
	int sendnum = 0;
	if (check_cookies(clntSock, thecookies))
	{
		readnum = recv(clntSock, downfile, 32, NULL);
		//��ȡ�ļ��Ĺ�ϣ�Ĺ�ϣ
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				return 0;
			}
			readnum += recv(clntSock, downfile + readnum, 32 - readnum, NULL);
		}
		downfile[32] = 0;
		//����û�ӵ�и��ļ����͸����û������ļ�
		if (all_users[user].have_files.count(downfile) == 1)
		{
			char *firstchar = (char *)malloc(sizeof(char));
			firstchar[0] = SEND_FILE;
			send(clntSock, firstchar, 1, 0);
			send_file(clntSock, downfile, all_files[downfile]);
			//free(filename);
			free(firstchar);
			return 1;
		}
		//printf("%s---\n", charto16(downfile));
		return 0;
	}
	return 0;
}
int share_file(char *user, SOCKET clntSock, char *thecookies)
{
	int readnum = 0;
	if (check_cookies(clntSock, thecookies))
	{
		char myfriend[33];
		char c[4];
		char buffer[50];
		memset(buffer, 0, sizeof(char)* 50);
		user_file *myfile = (user_file *)malloc(sizeof(user_file));
		memset(myfile, 0, sizeof(user_file));
		
		//��ȡ�������Ĺ�ϣ
		readnum = recv(clntSock, myfriend, 32, 0);
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfriend + readnum, 32 - readnum, NULL);
		}
		myfriend[32] = 0;
		
		//��ȡ�ļ�����С
		recv(clntSock, c, 1, 0);
		myfile->filename = (char *)malloc((unsigned char)c[0] * sizeof(char));
		myfile->filenamesize = c[0];
		//��ȡ�ļ���
		readnum = recv(clntSock, myfile->filename, (unsigned char)c[0], 0);
		while (readnum != (unsigned char)c[0])
		{
			if (readnum <= 0)
			{
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->filename + readnum, (unsigned char)c[0] - readnum, NULL);
		}
		//��ȡ�ļ���С
		readnum = recv(clntSock, c, 4, 0);
		while (readnum != 4)
		{
			if (readnum <= 0)
			{
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, c + readnum, 4 - readnum, NULL);
		}
		myfile->size = bytes_to_int((unsigned char *)c);
		//��ȡ�ļ��Ĺ�ϣ�Ĺ�ϣ
		readnum = recv(clntSock, myfile->id, 32, 0);
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->id + readnum, 32 - readnum, NULL);
		}
		(myfile->id)[32] = 0;
		//��ȡ�������ܵ��ļ���Կ
		readnum = recv(clntSock, myfile->key, 322, 0);
		while (readnum != 322)
		{
			if (readnum <= 0)
			{
				free(myfile->filename);
				free(myfile);
				return 0;
			}
			readnum += recv(clntSock, myfile->key + readnum, 322 - readnum, NULL);
		}
		//Ҫ���û����Է���Ļ���Ҫ��֤�ļ����û��У��ҷ�����Ǹ��û��ĺ���
		if ((all_users[user].have_friends).count(myfriend) && (all_users[user].have_files).count(myfile->id) && (all_users[myfriend].have_files).count(myfile->id)==0)
		{
			myfile->flag = 1;
			writedata(myfriend, myfile);
			(all_users[myfriend].have_files)[myfile->id] = 1;
			free(myfile->filename);
			free(myfile);
			char c[1];
			c[0] = SUCCESS_SHARE;
			send(clntSock, c, 1, 0);
			return 1;
		}
		else
		{
			free(myfile->filename);
			free(myfile);
			return 0;
		}
	}
	return 0;
}

int check_cookies(SOCKET clntSock, char *getcookies)
{
	//����cookies
	char recvcookies[32];
	int readnum;
	readnum = recv(clntSock,recvcookies,32,0);
	//if (readnum == -1)
	//{
	//	printf("%d\n", errno);
	//}
	while (readnum != 32)
	{
		if (readnum <= 0)
		{
			return 0;
		}
		readnum += recv(clntSock, recvcookies + readnum, 32 - +readnum, 0);
	}
	int flag = 1;
	for (int i = 0; i < 32; i++)
	{
		if (recvcookies[i] != getcookies[i])
			flag = 0;
	}
	return flag;
}



void send_file(SOCKET clntSock, char *filename,int size)
{

	//printf("%s\n", charto16(filename));
	//k���ֿ�����ļ���ָ������
	FILE **fd;

	//m���������ļ���ָ������
	FILE **fs;


	//��Ϊk����ļ������������ָ��
	char **data;

	//��Ϊm���������������ָ��
	char **coding;

	//��������ָ�룬���k��blocksize��С���ļ��������ݣ�Ҳ����data����
	char *buffer;

	//�ļ����ֵĿ������
	int blocknum;

	//ȱʧ�Ŀ�ı�����飬���һλΪ-1
	int erasures[100];

	//��ʾȱʧ�Ŀ�����飬ĳ����Ϊ1˵�����±��Ӧ�Ŀ�ȱʧ
	int erase[100];

	int e = 0;
	//��ʱ�ļ���
	char fname[1000];

	size = size * 16;

	memset(erase, 0, sizeof(erase));

	data = (char **)malloc(sizeof(char*)*ramp_k);
	coding = (char **)malloc(sizeof(char*)*ramp_m);
	buffer = (char *)malloc(ramp_k*blocksize);
	fd = (FILE **)malloc(sizeof(FILE*)*ramp_k);
	fs = (FILE **)malloc(sizeof(FILE*)*ramp_m);

	for (int i = 0; i < ramp_m; i++)
	{
		coding[i] = (char *)malloc(sizeof(char)*blocksize);
		memset(coding[i], 0, sizeof(char)*ramp_w*packetsize);
	}

	//��֤�ļ��еĴ���
	char mname[256];
	if (_access("user_file", 0) == -1)
	{
		_mkdir("user_file");
	}
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(mname, 100, "file_k_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(mname, 100, "file_m_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}

	//�򿪸��ֿ�������������ļ�
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(filename));
		fopen_s(fd + i, fname, "rb");
		//���blocknum
		if (*(fd + i) != NULL)
		{
			fseek(*(fd + i), 0, SEEK_END);
			blocknum = ftell(*(fd + i)) / blocksize;
			fseek(*(fd + i), 0, SEEK_SET);
		}
		else{
			erasures[e++] = i;
			erase[i] = 1;
			sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(filename));
			fopen_s(fd + i, fname, "wb");
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(fname, 100, "file_m_%d/%s", i, charto16(filename));
		fopen_s(fs + i, fname, "rb");
		if (*(fs + i) != NULL)
		{
			fseek(*(fs + i), 0, SEEK_END);
			blocknum = ftell(*(fs + i)) / blocksize;
			fseek(*(fs + i), 0, SEEK_SET);
		}
		else{
			erasures[e++] = i + ramp_k;
			erase[i + ramp_k] = 1;
			sprintf_s(fname, 100, "file_m_%d/%s", i, charto16(filename));
			fopen_s(fs + i, fname, "wb");
		}
	}
	

	//��ý������ļ��Ĵ�С
	size = blocknum*blocksize*ramp_k;
	erasures[e] = -1;

	//printf("���ؿ�ʼ��%ld\n", clock());

	while (blocknum--)
	{
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
		//��ȡ���ݵ�data��coding
		for (int i = 0; i < ramp_k; i++)
		{
			if (erase[i] == 0)
				fread(buffer + i*blocksize, sizeof(char), blocksize, fd[i]);
			data[i] = buffer + i*blocksize;
		}
		for (int i = 0; i < ramp_m; i++)
		{
			if (erase[i + ramp_k] == 0)
				fread(coding[i], sizeof(char), blocksize, fs[i]);
		}

		//���û�п춪ʧ���Ͳ��ý���
		if (erasures[0] != -1)
			jerasure_schedule_decode_lazy(ramp_k, ramp_m, ramp_w, bitmatrix, erasures, data, coding, packetsize*ramp_w, packetsize, 1);

		//һ�����ļ���д����ֽ���
		int n = blocksize*ramp_k;
		//��ȥ���
		if (blocknum == 0)
		{
			while (buffer[--n] == 0);
			/*printf("%d\n", n);
			printf("%s\n", charto16(buffer + n - 16));*/
		}
		send(clntSock, buffer, n, 0);

		//���������ݿ�����鶪ʧ�Ļ�����Ҫ�ָ���Щ��
		for (int i = 0; i < ramp_k; i++)
		{
			if (erase[i] != 0)
				fwrite(data[i], sizeof(char), blocksize, fd[i]);
		}
		for (int i = 0; i < ramp_m; i++)
		{
			if (erase[i + ramp_k] != 0)
				fwrite(coding[i], sizeof(char), blocksize, fs[i]);
		}
	}
	//printf("���ؽ�����%ld\n", clock());

	//�ͷ���������Ŀռ�
	for (int i = 0; i < ramp_k; i++)
	{
		fflush(fd[i]);
		fclose(fd[i]);
	}
	free(fd);
	for (int i = 0; i < ramp_m; i++)
	{
		fflush(fs[i]);
		fclose(fs[i]);
	}
	free(fs);


	free(data);

	for (int i = 0; i < ramp_m; i++)
		free(coding[i]);
	free(coding);

	free(buffer);

	return;
}


int recvfile(SOCKET clntSock, char *filename, int size)
{
	//k���ֿ�����ļ���ָ������
	FILE **fd;

	//m���������ļ���ָ������
	FILE **fs;


	//��Ϊk����ļ������������ָ��
	char **data;

	//��Ϊm���������������ָ��
	char **coding;

	//��������ָ�룬���k��blocksize��С���ļ��������ݣ�Ҳ����data����
	char *buffer;

	//�ļ����ֵĿ������
	int blocknum;


	//��ʱ�ļ���
	char fname[1000];

	size = size * 16;

	//printf("�ϴ���ʼ��%ld\n", clock());

	data = (char **)malloc(sizeof(char*)*ramp_k);
	coding = (char **)malloc(sizeof(char*)*ramp_m);
	buffer = (char *)malloc(ramp_k*blocksize);
	memset(buffer, 0, ramp_k*blocksize*sizeof(char));
	fd = (FILE **)malloc(sizeof(FILE*)*ramp_k);
	fs = (FILE **)malloc(sizeof(FILE*)*ramp_m);
	for (int i = 0; i < ramp_m; i++)
	{
		coding[i] = (char *)malloc(sizeof(char)*blocksize);
		memset(coding[i], 0, sizeof(char)*ramp_w*packetsize);
	}

	//��֤�ļ��еĴ���
	char mname[256];
	if (_access("user_file", 0) == -1)
	{
		_mkdir("user_file");
	}
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(mname, 100, "file_k_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(mname, 100, "file_m_%d", i);
		if (_access(mname, 0) == -1)
		{
			_mkdir(mname);
		}
	}

	//�򿪷ֿ���ļ��ͱ�����ļ�
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(filename));
		fopen_s(fd + i, fname, "wb");
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(fname, 100, "file_m_%d/%s", i, charto16(filename));
		fopen_s(fs + i, fname, "wb");
	}

	blocknum = size / (blocksize*ramp_k) + 1;



	//��ȡ�ļ����ֿ����౸��
	while (blocknum--)
	{
		int haveread = 0;
		if (blocknum != 0)
		{
			haveread = recv(clntSock, buffer, blocksize*ramp_k, 0);
			while (haveread != blocksize*ramp_k)
			{
				if (haveread <= 0)
				{
					//�ͷ���������Ŀռ�
					for (int i = 0; i < ramp_k; i++)
					{
						fflush(fd[i]);
						fclose(fd[i]);
					}
					free(fd);
					for (int i = 0; i < ramp_m; i++)
					{
						fflush(fs[i]);
						fclose(fs[i]);
					}
					free(fs);
					free(data);

					for (int i = 0; i < ramp_m; i++)
						free(coding[i]);
					free(coding);

					free(buffer);

					return 0;
				}
				haveread += recv(clntSock, buffer + haveread, blocksize*ramp_k - haveread, 0);
			}
		}
		else
		{
			haveread = recv(clntSock, buffer, size%(blocksize*ramp_k), 0);
			while (haveread != size % (blocksize*ramp_k))
			{
				//printf("%d %d\n", blocknum, haveread);
				if (haveread <= 0)
				{
					//�ͷ���������Ŀռ�
					for (int i = 0; i < ramp_k; i++)
					{
						fflush(fd[i]);
						fclose(fd[i]);
					}
					free(fd);
					for (int i = 0; i < ramp_m; i++)
					{
						fflush(fs[i]);
						fclose(fs[i]);
					}
					free(fs);
					free(data);

					for (int i = 0; i < ramp_m; i++)
						free(coding[i]);
					free(coding);

					free(buffer);

					return 0;
				}
				haveread += recv(clntSock, buffer + haveread, size % (blocksize*ramp_k) - haveread, 0);
			}
			//����memsetһ��ÿһ��ĺ������1000������
			buffer[size % (blocksize*ramp_k)] = 1;
		}

		//���ݼ�Ϊbuffer�е�����
		for (int i = 0; i < ramp_k; i++)
		{
			data[i] = buffer + i*blocksize;
		}

		
		//�����ļ�
		jerasure_schedule_encode(ramp_k, ramp_m, ramp_w, schedule, data, coding, blocksize, packetsize);

		//д���ļ�
		for (int i = 0; i < ramp_k; i++)
		{
			fwrite(data[i], sizeof(char), blocksize, fd[i]);
		}
		for (int i = 0; i < ramp_m; i++)
		{
			fwrite(coding[i], sizeof(char), blocksize, fs[i]);
		}
		//��ջ�����
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
	}
	//printf("�ϴ�������%ld\n", clock());

	//�ͷ���������Ŀռ�
	for (int i = 0; i < ramp_k; i++)
	{
		fflush(fd[i]);
		fclose(fd[i]);
	}
	free(fd);
	for (int i = 0; i < ramp_m; i++)
	{
		fflush(fs[i]);
		fclose(fs[i]);
	}
	free(fs);


	free(data);

	for (int i = 0; i < ramp_m; i++)
		free(coding[i]);
	free(coding);

	free(buffer);

	return 1;
}



