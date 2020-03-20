#include "../include/public.h"
#include "../include/split.h"
#include "../include/loginHMI.h"
#include "../include/searchHMI.h"
#include "../include/showHMI.h"
#include "../include/detialshowHMI.h"

int main(int argc, const char *argv[])
{
	int Ret = 0;
	int listfd = 0;
	int Connect = 0;
	char TmpBuff[1024] = {0};
	char *url = NULL;
	struct sockaddr_in SerAddr;

	listfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listfd)
	{
		perror("fail to socket\n");
		return -1;
	}

	SerAddr.sin_family = AF_INET;
	SerAddr.sin_port = htons(80);
	SerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	Ret = bind(listfd, (struct sockaddr *)&SerAddr, sizeof(SerAddr));
	if (-1 == Ret)
	{
		perror("fail to bind\n");
		return -1;
	}

	listen(listfd,10);

	while (1)
	{
		Connect = accept(listfd, NULL, NULL);
		if (-1 == Connect)
		{
			perror("fail to accept\n");
			return -1;
		}

		Ret = recv(Connect, TmpBuff, sizeof(TmpBuff), 0);
		if (0 == recv)
		{
			perror("fail to recv\n");
			return -1;
		}

		char Tmp[1024] = {0};
		strcpy(Tmp, TmpBuff);

		printf("%s\n", TmpBuff);	

		url = Split(TmpBuff);

		if (0 == strcmp(url, "/"))
		{
			LoginHMI("/home/linux/Desktop/pu/TeleMall/1.html", Connect, 0);
		}
		else if (strstr(url, "jpg"))
		{
			char  path_name[128] = {0};
			sprintf(path_name, "/home/linux/Desktop/pu/TeleMall%s", url);
			LoginHMI(path_name, Connect, 1);
		}
		
		else if (strstr(url, "login"))
		{
			SearchHMI(Tmp, "/home/linux/Desktop/pu/TeleMall/2.html", Connect, 0);
		}
		else if (strstr(url, "jpg"))
		{
			char  path_name[128] = {0};
			sprintf(path_name, "/home/linux/Desktop/pu/TeleMall%s", url);
			SearchHMI(path_name, Connect, 1);
		}
		else if (strstr(url, "search"))
		{
			ShowHMI(Tmp, Connect, 0);
		}
		else if (strstr(url, "HMI"))
		{
			DetialShowHMI(Tmp, Connect, 0);
		}
	
		close(Connect);
	}

	close(listfd);

	return 0;
}
