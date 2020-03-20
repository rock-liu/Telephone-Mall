#ifndef __DETIALSHOWHMI_H__
#define __DETIALSHOWHMI_H__

extern int DetialShowHMI(char *TmpBuff, int Connect, int type);
extern int sendfile_1(char *FileName, int Connect);
extern int call_1(void *arg, int col, char **result, char **title);
extern int sendhead_1(char *FileName, int Connect, int type);

#endif
