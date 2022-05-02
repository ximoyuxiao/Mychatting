#ifndef GROUP_H__
#define GROUP_H__
#include<string>
#include"../Tools/phase_msg.h"
using std::string;
class Group
{
private:
    int gid;
    int guid;
    string gname;
public:
    Group(int gid = 0,int guid=0,string gname="");
    ~Group();

    void setGid(const int gid);
    void setGuid(const int guid);
    void setGname(const string gname);

    int getGid(void);
    int getGuid(void);
    string getGname(void);
      int tomsg(char* msg);
    void getObj(phase_msg &pmsg);
};
typedef Group* Group_Ptr;
#endif