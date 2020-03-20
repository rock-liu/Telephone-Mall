#include "../include/public.h"

int sendhead_1(char *FileName, int Connect, int type)
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

	sendfile_1(FileName, Connect);

	return 0;
}

int sendfile_1(char *FileName, int Connect)
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

int call_1(void* arg,int col,char**result,char** title)
{

	FILE* fp =  (FILE*)arg;

	if (strlen(result[1]) != 0)
	{
		fprintf(fp,"<img src = '%s'>",result[1]);
	}
	else if (0 == strlen(result[1]))
	{
		fprintf(fp,"图片无法显示");
	}
	if (strlen(result[2]) != 0)
	{
		fprintf(fp,"keywords = %s",result[2]);
	}
	else if (0 == strlen(result[2]))
	{
		fprintf(fp, "暂无详细描述");
	}

	return 0;

}
int DetialShowHMI(char *TmpBuff, int Connect, int type)
{
	char *phone_id;

	strtok(TmpBuff, "I");
	phone_id = strtok(NULL, " ");
	printf("%s\n", phone_id);

	FILE* fp = fopen("4.html","w+");
	if(NULL == fp)
	{
		perror("fopen error");
		exit(1);
	}
	fputs("<!DOCTYPE html>\n",fp);
	fputs("<html >\n",fp);
	fputs("<head>\n",fp);
	fputs("<meta charset='utf-8'>\n",fp);
	fputs("<title>细节展示</title>\n",fp);
	fputs("</head>\n",fp);
	fputs("<body bgcolor = 'pink'>\n",fp);
	sqlite3 *db;
	char * errmsg;
	int ret = sqlite3_open("/home/linux/Desktop/pu/TeleMall/src/123.db",&db);
	if(SQLITE_OK != ret)
	{
		fprintf(stderr,"open db error %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	char sql_cmd[512]={0};
	sprintf(sql_cmd,"select goods_id,goods_img,keywords from goods where goods_id = %s;", phone_id);
	ret = sqlite3_exec(db,sql_cmd,call_1,fp,&errmsg);
	if(SQLITE_OK != ret)
	{
		fprintf(stderr,"exec error %s\n",errmsg);
		sqlite3_free(errmsg);
		sqlite3_close(db);
	}

	sqlite3_close(db);

	fputs("</body>\n",fp);
	fputs("</html>",fp);

	fclose(fp);

	sendhead_1("/home/linux/Desktop/pu/TeleMall/src/4.html", Connect, 0);

	return 0;
}

