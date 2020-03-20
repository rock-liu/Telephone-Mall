#include "../include/public.h"
#include "../include/loginHMI.h"

int LoginHMI(char *FileName, int Connect, int type)
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
