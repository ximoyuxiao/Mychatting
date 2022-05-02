#ifndef FRIEND_H__
#define FRIEND_H__
#include<string>
#include"../Tools/phase_msg.h"
using std::string;
class Friends
{
private:
    int uid;
    int fuid;
    string fname;
public:
    Friends(int uid=0,int fid=0,string fname="");
    ~Friends();
    void setUid(const int uid);
    void setFuid(const int fuid);
    void setFname(const string fname);

    int getUid(void);
    int getFuid(void);
    string getFname(void);
     int tomsg(char* msg);
    void getObj(phase_msg &pmsg);
};
typedef Friends* Friends_Ptr;
#endif