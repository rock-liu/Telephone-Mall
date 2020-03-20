#include "../include/public.h"

char new_buf[10]={0};

unsigned char FromHex(unsigned char x)
{
	if(0 == x ) return -1;
	unsigned char y;
	if(x>='A' &&x<='Z') y = x-'A'+10;
	else if(x>='a' &&x <='z') y = x-'a'+10;
	else if(x>='0' && x<='9') y = x-'0';

	return y;
}

int urlDecode(  char* dest, const char* src)
{
	if(NULL ==src || NULL == dest)
	{
		return -1;

	}
	int len = strlen(src);
	int i =0 ;
	for(i = 0 ;i<len;i++)
	{

		if('+' == src[i]) strcat(dest,"");
		else if('%'==src[i])
		{
			//if(i+2len)return -1;
			unsigned char high = FromHex((unsigned char)src[++i]);
			unsigned char low = FromHex((unsigned char)src[++i]);
			unsigned char temp = high*16 +low;
			char temp2[5]={0};
			sprintf(temp2,"%c",temp);
			strcat(dest , temp2);
		}
	}
	return 0;
}

int sendhead(char *FileName, int Connect, int type)
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

	sendfile(FileName, Connect);

	return 0;
}

int sendfile(char *FileName, int Connect)
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

int call(void* arg,int col,char**result,char** title)
{

	FILE* fp =  (FILE*)arg;

	fprintf(fp,"<a href='HMI%s'><img src='%s'></a>",result[2],result[1]);
	fprintf(fp,"<a href='HMI%s'>%s</a>",result[2],result[0]);
	
	return 0;

}
int ShowHMI(char *TmpBuff, int Connect, int type)
{
	char *phone_name;

	strtok(TmpBuff, "=");
	phone_name = strtok(NULL, " ");

	urlDecode(new_buf, phone_name);

	FILE* fp = fopen("3.html","w+");
	if(NULL == fp)
	{
		perror("fopen error");
		exit(1);
	}
	fputs("<!DOCTYPE html>\n",fp);
	fputs("<html >\n",fp);
	fputs("<head>\n",fp);
	fputs("<meta charset='utf-8'>\n",fp);
	fputs("<title>手机品牌展示</title>\n",fp);
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
	sprintf(sql_cmd,"select goods_name,goods_img,goods_id from goods where goods_name like '%%%s%%';", new_buf);
	ret = sqlite3_exec(db,sql_cmd,call,fp,&errmsg);
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

	sendhead("/home/linux/Desktop/pu/TeleMall/src/3.html", Connect, 0);

	return 0;
}

