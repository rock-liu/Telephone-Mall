#ifndef __SEARCHHMI_H__
#define __SEARCHHMI_H__

extern int send_head(char *FileName, int Connect, int type);
extern int send_file(char *FileName, int Connect);
extern int searchHMI(char *TmpBuff, char *FileName, int Connect, int type);
extern int callback(void *arg, int col, char **result, char **title); 

#endif
