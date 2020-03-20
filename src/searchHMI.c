#include "../include/public.h"

int send_head(char *FileName, int Connect, int type)
{
	int i = 0;
	int Ret = 0;
	int fd = 0;
	int Num = 0;
	char *head[5];
	char TmpBuff[1024] = {0};
	char Buff[50] = {0};
	char File_type[128] = {0};
	struct stat st;

	head[0] = "HTTP/1.1 200 OK\r\n";
	head[1] = "Connection: Keep-close\r\n";
	if (0 == type) // 打开的网页是HTML
	{
		strcpy(File_type, "Content-Type: text/html;charset=utf-8\r\n");
	}
	else if (1 == type) // 打开的网页是图片形式
	{
		strcpy(File_type, "Content-Type: Content-Type: image/jpeg\r\n");	
	}
	head[2] = File_type;
	head[3] = "Sever:XUNFENG/1.1\r\n";

	Ret = stat(FileName, &st);
	if (-1 == Ret)
	{
		perror("fail to stat\n");
		return -1;
	}	
	sprintf(Buff, "content-length:%ld\r\n\r\n", st.st_size);
	head[4] = Buff;

	for (i = 0; i < 5; i++)
	{
		send(Connect, head[i], strlen(head[i]), 0);
	}
	send_file(FileName, Connect);

	return 0;
}

int send_file(char *FileName, int Connect)
{
	int fd = 0;
	int Num = 0;
	char TmpBuff[1024] = {0};

	fd = open(FileName, O_RDONLY);
	if (-1 == fd)
	{
		perror("fail to open\n");
		return -1;
	}
	while (1)
	{
		Num = read(fd, TmpBuff, 1024);
		if (Num <= 0)
		{
			break;
		}
		send(Connect, TmpBuff, Num, 0);
	}
	close(fd);

	return 0;
}

int callback (void *arg, int col, char **result, char **title)
{
	*(int *) arg = 1;
	return 0;
}

int SearchHMI(char *TmpBuff, char *FileName, int Connect, int type)
{
	int Ret = 0;
	int flag = 0;
	char sql_cmd[255] = {0};
	char *errmsg;
	char *pTmp_name;
	char *pTmp_passwd;
	sqlite3 *db;

	strtok(TmpBuff, "=");
	pTmp_name = strtok(NULL, "&");
	strtok(NULL, "=");
	pTmp_passwd = strtok(NULL, " ");

	Ret = sqlite3_open("./1.db", &db);
	if (SQLITE_OK != Ret)
	{
		fprintf(stderr, "can't open db%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	sprintf(sql_cmd, "select * from rock where name = '%s' and passwd = '%s';",pTmp_name, pTmp_passwd);
	Ret = sqlite3_exec(db, sql_cmd, callback, &flag, &errmsg);
	if (SQLITE_OK != Ret)
	{
		fprintf(stderr, "exec %s errmsg:%s\n", sql_cmd, errmsg);
		sqlite3_free(errmsg);
		sqlite3_close(db);
	}
	if (1 == flag)
	{
		send_head(FileName, Connect,0);	
	}

	sqlite3_close(db);

	return 0;
}
