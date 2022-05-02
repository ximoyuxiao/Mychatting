#ifndef TOOLS_H__
#define TOOLS_H__
void getTime(char* timer);
ssize_t readBuff(int fd,void* buff,ssize_t count);
ssize_t writeBuff(int fd,void* buff,ssize_t count);
void Fclose(int fd);
off_t getFileSize(int fd);
#endif
