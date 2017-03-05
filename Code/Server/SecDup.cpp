#include"handle.h"


#pragma comment(lib, "ws2_32.lib")


extern map<string, int> all_files;

extern map<string, struct userinf> all_users;

extern FILE *all_files_fp;
extern FILE *all_users_fp;


//文件分为k份
extern int ramp_k;

//产生m块冗余码
extern int ramp_m;

//word size
extern int ramp_w;

extern int blocksize;
extern int packetsize;
extern HANDLE filelock;
extern HANDLE userlock;

extern int *matrix;
extern int *bitmatrix;
extern int **schedule;

//系统初始化
void initialize();

int cnt;
//线程函数
unsigned int __stdcall  thread_run(void *sock);

int main()
{
	char address[16] = "10.170.66.160";
	int port=5678;
	ramp_k = 4;
	ramp_m = 4;
	printf("输入ip地址：");
	scanf_s("%s", address, 16);
	printf("输入端口号：");
	scanf_s("%d", &port);
	printf("输入数据分块数k，冗余块数m：");
	scanf_s("%d%d", &ramp_k, &ramp_m);
	printf("系统初始化中···\n");

	ramp_w = 8;
	packetsize = 1024;
	blocksize = packetsize*ramp_w;

	matrix = cauchy_good_general_coding_matrix(ramp_k, ramp_m, ramp_w);
	bitmatrix = jerasure_matrix_to_bitmatrix(ramp_k, ramp_m, ramp_w, matrix);
	schedule = jerasure_smart_bitmatrix_to_schedule(ramp_k, ramp_m, ramp_w, bitmatrix);
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock < 0)
	{
		printf("socket failed!");
		exit(0);
	}
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr(address);  //具体的IP地址
	sockAddr.sin_port = htons(port);  //端口
	if (bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) < 0)
	{
		printf("bind error!\n");
		system("pause");
		exit(0);
	}
	//进入监听状态
	listen(servSock, 1000);

	initialize();

	filelock = CreateMutex(NULL, FALSE, NULL);
	userlock = CreateMutex(NULL, FALSE, NULL);


	printf("初始化已完成！\n");
	printf("正在监听用户···\n");

	cnt = 0;
	SOCKADDR clntAddr;
	int temp = sizeof(clntAddr);
	while (cnt!=1000)
	{
		//printf("%d\n",GetCurrentThreadId());
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &temp);
		printf("一用户已连接\n");
		cnt++;
		if (clntSock <= 0)
		{
			printf("error accept!");
			break;
		}
		_beginthreadex(NULL,0,thread_run,&clntSock,0,NULL);
		//向客户端发送数据
	}
	if (cnt == 1000)
		printf("连接用户已满！\n");
	closesocket(servSock);

	CloseHandle(filelock);
	CloseHandle(userlock);
	free(matrix);
	free(bitmatrix);

	for (int i = 0; i < ramp_k*ramp_m*ramp_w*ramp_w + 1; i++)
		free(schedule[i]);
	free(schedule);
	//终止 DLL 的使用
	WSACleanup();
	system("pause");
	return 0;
}

unsigned int __stdcall  thread_run(void *sock)
{
	SOCKET clntSock = *((SOCKET *)sock);
	int sendTimeout = 10000;
	setsockopt(clntSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&sendTimeout, sizeof(int));
	int recvTimeout = 3600000;
	setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(int));
	handleClnt(clntSock);
	printf("一名用户已离线!\n");
	cnt--;
	ExitThread(0);
	return 0;
}

void initialize()
{
	//保证所有的文件夹都建立了
	if (_access("user_file", 0) == -1)
	{
		_mkdir("user_file");
	}
	char fname[256];
	for (int i = 0; i < ramp_k; i++)
	{
		sprintf_s(fname, 100, "file_k_%d", i);
		if (_access(fname, 0) == -1)
		{
			_mkdir(fname);
		}
	}
	for (int i = 0; i < ramp_m; i++)
	{
		sprintf_s(fname, 100, "file_m_%d", i);
		if (_access(fname, 0) == -1)
		{
			_mkdir(fname);
		}
	}
	if (_access("user_friends", 0) == -1)
	{
		_mkdir("user_friends");
	}

	int cnt = 0;
	//初始化所有的文件信息
	fopen_s(&all_files_fp, "all_file.data", "ab+");
	fseek(all_files_fp, 0, SEEK_SET);
	char tempname[33];
	char n[4];
	int readnum;
	while ((readnum = fread(tempname, sizeof(char), 32, all_files_fp)) != 0)
	{
		tempname[32] = 0;
		fread(n, sizeof(char), 4, all_files_fp);
		all_files[tempname] = bytes_to_int((unsigned char *)n);
		cnt++;
	}
	printf("系统共有%d个文件,", cnt);
	cnt = 0;
	//初始化所有的用户
	fopen_s(&all_users_fp, "all_users.data", "ab+");
	fseek(all_users_fp, 0, SEEK_SET);
	while ((readnum = fread(tempname, sizeof(char), 32, all_users_fp)) != 0)
	{
		tempname[32] = 0;
		all_users[tempname].state = 0;
		all_users[tempname].file_lock = CreateMutex(NULL, FALSE, NULL);
		all_users[tempname].friend_lock = CreateMutex(NULL, FALSE, NULL);
		memcpy_s(all_users[tempname].userid, 32, tempname,32);
		all_users[tempname].userid[32] = 0;
		fread(all_users[tempname].userpassword, sizeof(char), 32, all_users_fp);
		all_users[tempname].userpassword[32] = 0;
		fread(all_users[tempname].userpkey, sizeof(char), 130, all_users_fp);

		fread(&(all_users[tempname].namelen), sizeof(char), 1, all_users_fp);
		all_users[tempname].username = (char *)malloc((unsigned char)all_users[tempname].namelen*sizeof(char));

		fread(all_users[tempname].username, sizeof(char), (unsigned char)all_users[tempname].namelen, all_users_fp);

		//初始化用户拥有的文件
		char fname[1000];
		sprintf_s(fname, 1000, "user_file/%s", charto16(all_users[tempname].userid));
		fopen_s(&(all_users[tempname].user_files), fname, "ab+");
		fseek(all_users[tempname].user_files, 0, SEEK_SET);
		while ((readnum = fread(fname, sizeof(char), 1, all_users[tempname].user_files)) != 0)
		{
			fread(fname+1, sizeof(char), (unsigned char)fname[0], all_users[tempname].user_files);
			fread(fname , sizeof(char), 4, all_users[tempname].user_files);
			fread(fname, sizeof(char), 32, all_users[tempname].user_files);
			fname[32] = 0;
			all_users[tempname].have_files[fname] = 1;
			fread(fname, sizeof(char), 322, all_users[tempname].user_files);
		}

		//初始化用户拥有的好友列表
		sprintf_s(fname, 1000, "user_friends/%s", charto16(all_users[tempname].userid));
		fopen_s(&(all_users[tempname].user_friends), fname, "ab+");
		fseek(all_users[tempname].user_friends, 0, SEEK_SET);
		while ((readnum = fread(fname, sizeof(char), 1, all_users[tempname].user_friends)) != 0)
		{
			fread(fname + 1, sizeof(char), (unsigned char)fname[0], all_users[tempname].user_friends);

			fread(fname , sizeof(char), 32, all_users[tempname].user_friends);
			fname[32] = 0;
			all_users[tempname].have_friends[tempname] = 1;
			fread(fname, sizeof(char), 130, all_users[tempname].user_files);
		}

		cnt++;
	}
	printf("%d个用户...\n", cnt);
	return;
}