CC = g++
CXXFLAGS  +=-Wall\
 	-I/usr/local/mysql/include/mysql\
 	-I/usr/include/mysql\
	-I/usr/include/hiredis/\
	-std=c++11 -g

CFLAGS +=-Wall\
	-I/usr/local/mysql/include/mysql\
 	-I/usr/include/mysql\
	-I/usr/include/hiredis/

LDFLAGS +=-L/usr/local/mysql/lib/mysql\
	-L/usr/lib/mysql\
	-L/usr/lib64/mysql\
	-L/usr/local/lib/

LIBS += -pthread\
	-lmysqlclient\
	-lhiredis
SERVER_OBJ=server.o\
	Tools/mydb.o\
	Tools/ThreadAdmin.o\
	Tools/phase_msg.o\
	Tools/tools.o\
	Tools/mlog.o\
	Tools/epoll.o\
	Tools/Tpool/threadpool.o\
	Tools/Tpool/cond.o\
	Tools/Tpool/locker.o\
	Tools/Tpool/sem.o\
	Tools/Redis/redis.o\
	Model/Friends.o\
	Model/Group.o\
	Model/GroupFriend.o\
	Model/MessageTable.o\
	Model/UserInfo.o\
	DAO/FriendsDao.o\
	DAO/GroupDao.o\
	DAO/GroupFriendDao.o\
	DAO/MessageTableDao.o\
	DAO/UserInfoDao.o\
	Service/myService.o
all:server

server:${SERVER_OBJ}
	${CC} $^ -o $@ ${LDFLAGS} ${LIBS}
clean:
	rm ${SERVER_OBJ} server
