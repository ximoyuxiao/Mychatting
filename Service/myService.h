#include"../Tools/phase_msg.h"
#include"../DAO/GroupDao.h"
#include"../DAO/FriendsDao.h"
#include"../DAO/GroupFriendDao.h"
#include"../DAO/MessageTableDao.h"
#include"../DAO/UserInfoDao.h"

bool loginService(phase_msg& pmsg,int fd);
void registerService(phase_msg& pmsg,int fd);
void getFriendList(phase_msg& pmsg,int fd);
void addFriend(phase_msg& pmsg,int fd);
void delFriend(phase_msg& pmsg,int fd);
void getGroupList(phase_msg& pmsg,int fd);

void addGroup(phase_msg& pmsg,int fd);
void BulidGroup(phase_msg&pmsg,int fd);
void recvAllMsg(phase_msg&pmsg,int fd);
void downFile(phase_msg&pmsg,int fd);
