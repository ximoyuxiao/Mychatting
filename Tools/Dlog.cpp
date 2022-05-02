#include<iostream>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include"Dlog.h"
using namespace std;
Dlog* Dlog::log = NULL;

Dlog::Dlog(const char* path){
    this->path = new char[strlen(path) + 1];
    strcpy(this->path,path);
}
void Dlog::setFileName()
{
    char filename[1024];
    char data[30];
    getDateString(data);
    sprintf(filename,"%s/%s.log",path,data);
    setFileName(filename);
}
void Dlog::setFileName(const char* filename){
    if(fileName)
        delete []fileName;
    this->fileName = new char[strlen(filename)+ 1];
    strcpy(this->fileName,filename);
}
void Dlog::getDateString(char* date){
    time_t now;
    time(&now);
    tm *phare = localtime(&now);
    sprintf(date,"%04d-%02d-%02d",phare->tm_year + 1900,phare->tm_mon + 1,phare->tm_mday);
}
void Dlog::gettimeString(char* ti){
    time_t now;
    time(&now);
    tm *phare = localtime(&now);
    sprintf(ti,"[%02d:%02d:%02d]",phare->tm_hour,phare->tm_min,phare->tm_sec);
}
Dlog* Dlog::init(const char* path){
    log = new Dlog(path);
    return log;
}

void Dlog::write_db_logger(const char* table,const char* columm,const char* err){
    setFileName();
    FILE *stdlog = fopen(fileName,"a");
    char tt[1024];
    gettimeString(tt);
    fprintf(stdlog,"TIME:%s  [DBERR]->[%s]->[%s]:%s\n",tt,table,columm,err);
    fclose(stdlog);
}
void Dlog::write_conf_logger(const char* head,const char* err){
    setFileName();
    FILE *stdlog = fopen(fileName,"a");
    char tt[1024];
    gettimeString(tt);
    fprintf(stdlog,"TIME:%s [CONFERR]->[%s]:%s\n",tt,head,err);
    fclose(stdlog);
}
void Dlog::judge_logger(const int soulution_id,const char* err){
    setFileName();
    FILE *stdlog = fopen(fileName,"a");
    char tt[1024];
    gettimeString(tt);
    fprintf(stdlog,"TIME:%s [JUDGEERR]->[%d]:%s\n",tt,soulution_id,err);
    fclose(stdlog);
}
void Dlog::write_normal_logger(const char* msg,const char* funcName){
    setFileName();
    FILE *stdlog = fopen(fileName,"a");
    char tt[1024];
    gettimeString(tt);
    fprintf(stdlog,"%s->[%s]:%s\n",tt,funcName,msg);
    fclose(stdlog);
}
void Dlog::del_log(const char * path){
    char del[1024];
    sprintf(del,"rm %s",path);
    system(del);
}
void Dlog::clear(const char* path){

}
Dlog* Dlog::getLog()
{
    if(log ==nullptr)
        log = new Dlog();
    return log;
}
bool Dlog::destoryLog()
{
    delete log;
    log = nullptr;
    return true;
}
Dlog::~Dlog(){
    delete path;
    if(fileName)
        delete fileName;
}