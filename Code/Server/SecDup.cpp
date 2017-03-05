#include"handle.h"


#pragma comment(lib, "ws2_32.lib")


extern map<string, int> all_files;

extern map<string, struct userinf> all_users;

extern FILE *all_files_fp;
extern FILE *all_users_fp;


//�ļ���Ϊk��
extern int ramp_k;

//����m��������
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

//ϵͳ��ʼ��
void initialize();

int cnt;
//�̺߳���
unsigned int __stdcall  thread_run(void *sock);

int main()
{
	char address[16] = "10.170.66.160";
	int port=5678;
	ramp_k = 4;
	ramp_m = 4;
	printf("����ip��ַ��");
	scanf_s("%s", address, 16);
	printf("����˿ںţ�");
	scanf_s("%d", &port);
	printf("�������ݷֿ���k���������m��");
	scanf_s("%d%d", &ramp_k, &ramp_m);
	printf("ϵͳ��ʼ���С�����\n");

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
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr(address);  //�����IP��ַ
	sockAddr.sin_port = htons(port);  //�˿�
	if (bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) < 0)
	{
		printf("bind error!\n");
		system("pause");
		exit(0);
	}
	//�������״̬
	listen(servSock, 1000);

	initialize();

	filelock = CreateMutex(NULL, FALSE, NULL);
	userlock = CreateMutex(NULL, FALSE, NULL);


	printf("��ʼ������ɣ�\n");
	printf("���ڼ����û�������\n");

	cnt = 0;
	SOCKADDR clntAddr;
	int temp = sizeof(clntAddr);
	while (cnt!=1000)
	{
		//printf("%d\n",GetCurrentThreadId());
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &temp);
		printf("һ�û�������\n");
		cnt++;
		if (clntSock <= 0)
		{
			printf("error accept!");
			break;
		}
		_beginthreadex(NULL,0,thread_run,&clntSock,0,NULL);
		//��ͻ��˷�������
	}
	if (cnt == 1000)
		printf("�����û�������\n");
	closesocket(servSock);

	CloseHandle(filelock);
	CloseHandle(userlock);
	free(matrix);
	free(bitmatrix);

	for (int i = 0; i < ramp_k*ramp_m*ramp_w*ramp_w + 1; i++)
		free(schedule[i]);
	free(schedule);
	//��ֹ DLL ��ʹ��
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
	printf("һ���û�������!\n");
	cnt--;
	ExitThread(0);
	return 0;
}

void initialize()
{
	//��֤���е��ļ��ж�������
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
	//��ʼ�����е��ļ���Ϣ
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
	printf("ϵͳ����%d���ļ�,", cnt);
	cnt = 0;
	//��ʼ�����е��û�
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

		//��ʼ���û�ӵ�е��ļ�
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

		//��ʼ���û�ӵ�еĺ����б�
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
	printf("%d���û�...\n", cnt);
	return;
}