#include"Group.h"

Group::Group(int gid,int guid,string gname):gid(gid),guid(guid),gname(gname){}
Group::~Group(){}

void Group::setGid(const int gid){this->gid = gid;}
void Group::setGuid(const int guid){this->guid = guid;}
void Group::setGname(const string gname){this->gname = gname;}

int Group::getGid(void){return gid;}
int Group::getGuid(void){return guid;}
string Group::getGname(void){return gname;}
int Group::tomsg(char* msg)
{
    sprintf(msg,"gid=%d&guid=%d&gname=%s&",gid,guid,gname.c_str());
    return strlen(msg);
}
void Group::getObj(phase_msg &pmsg)
{
    pmsg.findInt("gid",gid);
    pmsg.findInt("guid",guid);
    pmsg.findKey("gname",gname);
}