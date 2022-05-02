#ifndef DLOG_H__
#define DLOG_H__
#define LOGPATH "log"
class Dlog
{
private:
    char*   path;  
    char*   fileName;
    static Dlog*   log;
    Dlog(const char* path = LOGPATH);
     ~Dlog();
    void setFileName(const char* filename);
    void setFileName();
    void getDateString(char* date);
    void gettimeString(char* time);
public:
    static Dlog* init(const char* path = LOGPATH);
    static Dlog*  getLog();
    void write_db_logger(const char* table,const char* columm,const char* err);
    void write_conf_logger(const char* head,const char* err);
    void judge_logger(const int soulution_id,const char* err);
    void write_normal_logger(const char* msg,const char* funcName="UNKNOWERR");
    void del_log(const char * path);
    void clear(const char* path);
    static bool destoryLog();
   
};
#endif
