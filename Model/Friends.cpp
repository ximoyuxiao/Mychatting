#include"Friends.h"

Friends::Friends(int uid,int fid,string fname):uid(uid),fuid(fid),fname(fname){}
Friends::~Friends(){}
void Friends::setUid(const int uid){this->uid = uid;}
void Friends::setFuid(const int fuid){this->fuid = fuid;}
void Friends::setFname(const string fname){this->fname = fname;}

int Friends::getUid(void){return uid;}
int Friends::getFuid(void){return fuid;}
string Friends::getFname(void){return fname;}
int Friends::tomsg(char* msg)
{
    sprintf(msg,"uid=%d&fuid=%d&fname=%s&",uid,fuid,fname.c_str());
    return strlen(msg);
}
void Friends::getObj(phase_msg &pmsg)
{
    pmsg.findInt("uid",uid);
    pmsg.findInt("fuid",fuid);
    pmsg.findKey("fname",fname);
}