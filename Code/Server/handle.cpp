#include"handle.h"

//要求线程互斥
HANDLE filelock;
HANDLE userlock;

map<string, int> all_files;

map<string, struct userinf> all_users;

FILE *all_files_fp;
FILE *all_users_fp;


//文件分为k份
int ramp_k;

//产生m块冗余码
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
	//临时的用户标示符
	char cookies[32];
	//用户的标号，-1表示没有用户登录
	char user[33];
	memset(user, 0, 33 * sizeof(char));
	//收到的字节数
	int readnum = 0;
	int flag = 1;

	//表明该个线程处理的用户是否登录了
	int state = 0;
	while (flag)
	{
		//printf("子线程%d\n", GetCurrentThreadId());
		//printf("等待");
		readnum = recv(clntSock, buffer, 1, NULL);
		if (readnum <= 0)
		{
			printf("用户已断开连接!\n");
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
					printf("一用户已登录！\n");
					state = 1;
				}
				break;
			case SIGN_UP:
				if (sign_up(clntSock))
				{
					printf("一用户已注册！\n");
				}
				break;
			case SIGN_OUT:
				if (sign_out(user, clntSock, cookies))
				{
					state = 0;
					printf("一用户已退出...\n");
				}
				break;
			case REQUIRE_FILES:
				if (!(state && require_files(user, clntSock, cookies)))
				{
					flag=0;
					printf("一用户请求文件列表失败！\n");
				}
				else
				{
					printf("一用户已取得文件列表！\n");
				}
				break;
			case REQUIRE_FRIENDS:
				if (!(state && require_friends(user, clntSock, cookies)))
				{
					flag = 0;
					printf("一用户取得好友列表失败！\n");
				}
				else
				{
					printf("一用户已取得好友列表！\n");
				}
				break;
			case ADD_FRIEND:
				if (!(state && add_friend(user, clntSock, cookies)))
				{
					flag = 0;
					printf("一用户添加一好友失败！\n");
				}
				else
				{
					printf("一用户已添加一好友！\n");
				}
				break;
			case CHECK_FILE:
				if (!(state && check_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("一用户上传文件失败！\n");
				}
				break;
			case DOWNLOAD_FILE:
				if (!(state && download_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("一用户被拒绝下载文件\n");
				}
				else
				{
					printf("一用户下载了文件\n");
				}
				break;
			case SHARE_FILE:
				if (!(state && share_file(user, clntSock, cookies)))
				{
					flag = 0;
					printf("一用户分享文件失败！\n");
				}
				else
				{
					printf("一用户分享文件成功！\n");
				}
				break;
			default:
				flag = 0;
				break;
			}
		}
	}
	printf("用户不合法的行为导致连接被主动关闭！\n");
	closesocket(clntSock);
	free(buffer);
}

//用户的登录
int sign_in(char *user,SOCKET clntSock, char *cookies)
{
	char userid[33];
	char userpassword[33];
	int readnum = 0;
	//读取用户名的哈希
	readnum = recv(clntSock, userid, 32, NULL);
	while (readnum != 32)
	{
		if (readnum <= 0)
			return 0;
		readnum += recv(clntSock, userid + readnum, 32 - readnum, NULL);
	}
	userid[32] = 0;

	//读取密码
	readnum = recv(clntSock, userpassword, 32, NULL);
	while (readnum != 32)
	{
		if (readnum <= 0)
			return 0;
		readnum += recv(clntSock, userpassword + readnum, 32 - readnum, NULL);
	}
	userpassword[32] = 0;
	//判断该用户存在且密码正确且该用户没有登录
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


//用户注册
int sign_up(SOCKET clntSock)
{
	char uname[33];
	
	int readnum;
	//接收用户的名字哈希
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

	//假如这个用户不存在的话，可以注册
	if (all_users.count(uname) == 0)
	{
		all_users[uname].state = 0;
		memcpy_s(all_users[uname].userid, 32, uname, 32);
		//接收用户的密码
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

		//接收用户的名字长度和名字
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

		//接收用户的公钥
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

//向文件中写入用户的信息
void writeuser(struct userinf user)
{
	//保证写入文件的同步,等待互斥锁
	WaitForSingleObject(userlock, INFINITE);
	printf("写入一名用户\n");
	fseek(all_users_fp, 0, SEEK_END);
	fwrite(user.userid, sizeof(char), 32, all_users_fp);
	fwrite(user.userpassword, sizeof(char), 32, all_users_fp);
	fwrite(user.userpkey, sizeof(char), 130, all_users_fp);
	fwrite(&(user.namelen), sizeof(char), 1, all_users_fp);
	fwrite(user.username, sizeof(char), (unsigned char)user.namelen, all_users_fp);
	fflush(all_users_fp);
	//释放互斥锁
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


//返回数据帧格式：标志头(1)|后面的信息长度(4)|文件名长度(1)|文件名(?)|文件大小(4)|文件哈希哈希值(32)|......
//用户请求文件列表
int require_files(char *user, SOCKET clntSock, char *thecookies)
{
	char *buffer;
	int readnum = 0;

	int sendnum = 0;
	if (check_cookies(clntSock, thecookies))
	{
		int n = 0;
		//等待互斥锁
		WaitForSingleObject(all_users[user].file_lock, INFINITE);
		sendnum++;
		//文件名和大小等总长度+200，防止缓冲区溢出
		fseek(all_users[user].user_files, 0, SEEK_END);
		int fsize = ftell(all_users[user].user_files);
		buffer = (char *)malloc((fsize+200)*sizeof(char));
		memset(buffer, 0, sizeof(char)*(fsize + 200));

		buffer[0] = (char)GIVE_FILES;
		fseek(all_users[user].user_files, 0, SEEK_SET);
		//保留四个字节作为文件的数目
		sendnum += 4;
		//文件名大小
		while (fread(buffer + sendnum, sizeof(char), 1, all_users[user].user_files) != 0)
		{
			n++;
			sendnum += 1;

			//文件名
			sendnum += fread(buffer + sendnum, sizeof(char), bytes_to_int((unsigned char *)buffer + sendnum - 1), all_users[user].user_files);
			//文件大小

			sendnum += fread(buffer + sendnum, sizeof(char), 4, all_users[user].user_files);
			//文件的唯一标示符，哈希的哈希

			sendnum += fread(buffer + sendnum, sizeof(char), 32, all_users[user].user_files);
			(all_users[user].have_files)[buffer + sendnum - 32] = 1;
			//经过用户私钥加密的文件的哈希

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
		//文件名和大小等总长度+200，防止缓冲区溢出
		fseek(all_users[user].user_friends, 0, SEEK_END);
		int fsize = ftell(all_users[user].user_friends);
		buffer = (char *)malloc((fsize + 200)*sizeof(char));
		memset(buffer, 0, sizeof(char)*(fsize + 200));

		buffer[0] = (char)GIVE_FILES;
		fseek(all_users[user].user_friends, 0, SEEK_SET);
		//保留四个字节作为好友的数目
		sendnum += 4;
		//好友名大小
		while (fread(buffer + sendnum, sizeof(char), 1, all_users[user].user_friends) != 0)
		{
			n++;
			sendnum++;
			//好友名
			sendnum += fread(buffer + sendnum, sizeof(char), bytes_to_int((unsigned char *)buffer + sendnum -1), all_users[user].user_friends);
			//好友名的哈希
			sendnum += fread(buffer + sendnum, sizeof(char), 32, all_users[user].user_friends);
			(all_users[user].have_friends)[buffer + sendnum - 32] = 1;
			//好友的公钥
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

//用户添加好友，只需要发送该好友的名字的哈希就可以了，会返回给用户好友的公钥
int add_friend(char *user, SOCKET clntSock, char *thecookies)
{
	char *buffer;
	if (check_cookies(clntSock, thecookies))
	{
		buffer = (char *)malloc(350 * sizeof(char));
		//读取得到用户请求加的好友的名字的哈希
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
			//将二者互为好友的信息写入到文件中去
			writefriends(user, buffer);
			(all_users[buffer].have_friends)[user] = 1;
			(all_users[user].have_friends)[buffer] = 1;
			//返回给用户好友的公钥
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

//将user1和user2互为好友的信息写入到文件中
//此处仍有漏洞，难以做到线程的互斥，假如这两个用户都登录了，两人同时添加好友，无论两者是不是互相添加，都可能会出现严重的问题
//------需要后续进一步解决线程的互斥的问题
void writefriends(char *user1, char *user2)
{
	char *buffer;
	char *c=(char *)malloc(1*sizeof(char));
	struct userinf *u1 = &(all_users[user1]);
	struct userinf *u2 = &(all_users[user2]);
	//u1为u2的好友
	WaitForSingleObject(all_users[user2].friend_lock, INFINITE);
	fseek(u2->user_friends, 0, SEEK_END);
	buffer = (char *)malloc(((unsigned char)u1->namelen + 300)*sizeof(char));
	buffer[0] = u1->namelen;
	memcpy_s(buffer	+ 1, 256, u1->username, (unsigned char)u1->namelen);
	memcpy_s(buffer + 1 + (unsigned char)u1->namelen, 32, u1->userid, 32);
	memcpy_s(buffer + 33 + (unsigned char)u1->namelen, 130, u1->userpkey, 130);
	//等待互斥锁
	fwrite(buffer, sizeof(char), (unsigned char)u1->namelen + 163, u2->user_friends);
	fflush(u2->user_friends);

	free(buffer);
	ReleaseMutex(all_users[user2].friend_lock);

	//u2为u1的好友
	//等待互斥锁
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

//校验文件
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
	//用户身份合法
	if (check_cookies(clntSock, thecookies))
	{
		//获取文件名大小
		recv(clntSock, c, 1, 0);
		myfile->filename = (char *)malloc((unsigned char)c[0]*sizeof(char));
		myfile->filenamesize = c[0];
		//获取文件名
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
		//获取文件大小
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
		//获取文件的哈希的哈希
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
		//获取经过加密的文件密钥
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
			//文件已经存在
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
			//文件不存在
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
		printf("校验文件时用户身份不合法\n");
		return 0;
	}
}

int upload_files(char *user, SOCKET clntSock, char *thecookies, user_file *myfile)
{
	int readnum = 0;
	if (check_cookies(clntSock, thecookies) && recvfile(clntSock, myfile->id, myfile->size))
	{
		//接收完文件后应该将该文件信息写入用户的数据和系统的数据中
		writedata(user, myfile);
		char c[1];
		c[0] = UPLOAD_FINISH;
		send(clntSock, c, 1, 0);
		return 1;
	}
	else
	{
		printf("用户上传文件时身份不合法！\n");
		return 0;
	}
}

int upload_partfile(char *user, SOCKET clntSock, char *thecookies, user_file *myfile)
{
	//k个分开后的文件的指针数组
	FILE **fd;

	//m个冗余块的文件的指针数组
	FILE **fs;


	//分为k块的文件的数据数组的指针
	char **data;

	//分为m块的冗余码的数组的指针
	char **coding;

	//缓冲区的指针，存放k块blocksize大小的文件数据内容，也就是data数据
	char *buffer;

	char *checkdata;

	//文件划分的块的总数
	int blocknum;

	//缺失的块的编号数组，最后一位为-1
	int erasures[100];

	//标示缺失的块的数组，某个数为1说明其下标对应的块缺失
	int erase[100];

	int e = 0;
	//临时文件名
	char fname[1000];

	int size = myfile->size * 32;
	//printf("验证开始：%ld\n", clock());
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
	//保证文件夹的存在
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

	//打开各分块和冗余码所在文件
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(myfile->id));
		fopen_s(fd + i, fname, "rb");
		//获得blocknum
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
	//获得解码后的文件的大小
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

	//假如存在块缺失的话，要先把缺失的块补齐
	if (erasures[0] != -1)
	{
		recover_file(fd,fs,erase,erasures,blocknum);
	}

	checkdata = (char *)malloc(sizeof(char)* 2 * blocksize*ramp_k);
	buffer = (char *)malloc(sizeof(char)* blocksize*ramp_k);
	int ct = xr - xl;
	for (int i = 0; i < ramp_k; i++)
	{
		//fseek移动不过去，所以采用关闭文件再打开的方式
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
	//发送请求文件某部分的文件帧
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
	
	//帧格式正确并且验证文件正确，就写入数据
	if ((unsigned char)c[0] == UPLOAD_FILE&&check_partfile(user, clntSock, thecookies, checkdata + xl+16, myfile->right - myfile->left + 1) == 1)
	{
		writedata(user, myfile);
		char c[1];
		c[0] = UPLOAD_FINISH;
		send(clntSock, c, 1, 0);
		ret = 1;
		//printf("验证结束：%ld\n", clock());
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

//恢复文件
void recover_file(FILE **fd, FILE **fs, int *erase, int *erasures,int blocknum)
{
	//分为k块的文件的数据数组的指针
	char **data;

	//分为m块的冗余码的数组的指针
	char **coding;

	//缓冲区的指针，存放k块blocksize大小的文件数据内容，也就是data数据
	char *buffer;
	//printf("恢复开始：%ld\n", clock());

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
		//读取数据到data和coding
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

		//假如有数据块或编码块丢失的话，恢复这些块
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
	//printf("恢复结束：%ld\n", clock());
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

//检查一部分的文件是否相同
int check_partfile(char *user, SOCKET clntSock, char *thecookies, char *checkdata,int ct)
{

	int readnum = 0;
	
	char *recvdata;
	recvdata = (char *)malloc(sizeof(char)*ct * 16);
	if (check_cookies(clntSock, thecookies))
	{
		//接收传来的部分文件
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
		//检查文件是否相同
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
		printf("用户身份不合法！\n");
		return 0;
	}
}

void writedata(char *user, user_file *myfile)
{
	if (all_users[user].have_files.count(myfile->id) != 0)
		return;
	//将文件写入到用户信息中
	char buffer[4];

	//保证用户文件信息操作的互斥，防止有用户分享给该用户时出现问题
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

	//保证写入文件的同步,等待互斥锁
	WaitForSingleObject(filelock, INFINITE);
	printf("一用户已上传或分享一个文件,");
	//假如系统没有的话，就写入到系统的文件信息中
	if (myfile->flag != 1)
	{
		fseek(all_files_fp, 0, SEEK_END);
		fwrite(myfile->id, sizeof(char), 32, all_files_fp);
		fwrite(buffer, sizeof(char), 4, all_files_fp);
		all_files[myfile->id] = myfile->size;
		fflush(all_files_fp);
		printf("且此文件系统没有\n");
	}
	else
		printf("且此文件系统已有\n");
	//释放互斥锁
	//printf("%d个文件\n", all_files.size());
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
		//读取文件的哈希的哈希
		while (readnum != 32)
		{
			if (readnum <= 0)
			{
				return 0;
			}
			readnum += recv(clntSock, downfile + readnum, 32 - readnum, NULL);
		}
		downfile[32] = 0;
		//如果用户拥有该文件，就给该用户传该文件
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
		
		//获取好友名的哈希
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
		
		//获取文件名大小
		recv(clntSock, c, 1, 0);
		myfile->filename = (char *)malloc((unsigned char)c[0] * sizeof(char));
		myfile->filenamesize = c[0];
		//获取文件名
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
		//获取文件大小
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
		//获取文件的哈希的哈希
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
		//获取经过加密的文件密钥
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
		//要是用户可以分享的话，要保证文件该用户有，且分享的是该用户的好友
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
	//接收cookies
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
	//k个分开后的文件的指针数组
	FILE **fd;

	//m个冗余块的文件的指针数组
	FILE **fs;


	//分为k块的文件的数据数组的指针
	char **data;

	//分为m块的冗余码的数组的指针
	char **coding;

	//缓冲区的指针，存放k块blocksize大小的文件数据内容，也就是data数据
	char *buffer;

	//文件划分的块的总数
	int blocknum;

	//缺失的块的编号数组，最后一位为-1
	int erasures[100];

	//标示缺失的块的数组，某个数为1说明其下标对应的块缺失
	int erase[100];

	int e = 0;
	//临时文件名
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

	//保证文件夹的存在
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

	//打开各分块和冗余码所在文件
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d/%s", i, charto16(filename));
		fopen_s(fd + i, fname, "rb");
		//获得blocknum
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
	

	//获得解码后的文件的大小
	size = blocknum*blocksize*ramp_k;
	erasures[e] = -1;

	//printf("下载开始：%ld\n", clock());

	while (blocknum--)
	{
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
		//读取数据到data和coding
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

		//如果没有快丢失，就不用解码
		if (erasures[0] != -1)
			jerasure_schedule_decode_lazy(ramp_k, ramp_m, ramp_w, bitmatrix, erasures, data, coding, packetsize*ramp_w, packetsize, 1);

		//一次向文件中写入的字节数
		int n = blocksize*ramp_k;
		//除去填充
		if (blocknum == 0)
		{
			while (buffer[--n] == 0);
			/*printf("%d\n", n);
			printf("%s\n", charto16(buffer + n - 16));*/
		}
		send(clntSock, buffer, n, 0);

		//假如有数据块或编码块丢失的话，仍要恢复这些块
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
	//printf("下载结束：%ld\n", clock());

	//释放所有申请的空间
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
	//k个分开后的文件的指针数组
	FILE **fd;

	//m个冗余块的文件的指针数组
	FILE **fs;


	//分为k块的文件的数据数组的指针
	char **data;

	//分为m块的冗余码的数组的指针
	char **coding;

	//缓冲区的指针，存放k块blocksize大小的文件数据内容，也就是data数据
	char *buffer;

	//文件划分的块的总数
	int blocknum;


	//临时文件名
	char fname[1000];

	size = size * 16;

	//printf("上传开始：%ld\n", clock());

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

	//保证文件夹的存在
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

	//打开分块的文件和编码的文件
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



	//读取文件，分块冗余备份
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
					//释放所有申请的空间
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
					//释放所有申请的空间
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
			//这句和memset一起将每一块的后面填充1000···
			buffer[size % (blocksize*ramp_k)] = 1;
		}

		//数据即为buffer中的数据
		for (int i = 0; i < ramp_k; i++)
		{
			data[i] = buffer + i*blocksize;
		}

		
		//编码文件
		jerasure_schedule_encode(ramp_k, ramp_m, ramp_w, schedule, data, coding, blocksize, packetsize);

		//写入文件
		for (int i = 0; i < ramp_k; i++)
		{
			fwrite(data[i], sizeof(char), blocksize, fd[i]);
		}
		for (int i = 0; i < ramp_m; i++)
		{
			fwrite(coding[i], sizeof(char), blocksize, fs[i]);
		}
		//清空缓冲区
		memset(buffer, 0, sizeof(char)*blocksize*ramp_k);
	}
	//printf("上传结束：%ld\n", clock());

	//释放所有申请的空间
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



