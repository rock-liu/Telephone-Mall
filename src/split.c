#include "../include/public.h"
#include "../include/split.h"

char *Split(char *pTmpBuff)
{
	char *method = NULL;
	char *url = NULL;
	char *version = NULL;

	method = strtok(pTmpBuff, " ");
	url = strtok(NULL, " ");
	version = strtok(NULL, "\r\n");

	return url;
}
