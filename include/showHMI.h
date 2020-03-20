#ifndef __SHOWHMI_H__
#define __SHOWHMI_H__

extern int ShowHMI(char *TmpBuff, int Connect, int type);
extern int sendfile(char *FileName, int Connect);
extern int call(void *arg, int col, char **result, char **title);
extern int sendhead(char *FileName, int Connect, int type);
extern int urlDecode(  char* dest, const char* src);
extern unsigned char FromHex(unsigned char x);

#endif
