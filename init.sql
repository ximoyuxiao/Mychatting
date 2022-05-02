set names utf8mb4;
#1.创建表空间
create database Chatting;
# 2.创建用户
# CREATE USER 'moyutest'@'localhost' IDENTIFIED BY '199094212';
#3.授予用户表空间的权限
grant all privileges on Chatting.* to 'moyutest'@'localhost';
use Chatting;

#1、用户信息表格 （用户ID、用户昵称、用户密码）
create table UserInfo(
	uid  int primary key AUTO_INCREMENT,
    upass varchar(128) not null,
    uname varchar(64) not null
)DEFAULT CHARSET=utf8mb4;
ALTER TABLE UserInfo AUTO_INCREMENT = 1000;



#2、好友关系表格  (用户ID、好友ID、好友昵称）
create table Friends(
	uid int,
    fuid int,
    funame varchar(64),
    constraint pk_Friends PRIMARY KEY(uid,fuid),

    constraint fk_Friends_uid FOREIGN KEY(uid)
    REFERENCES UserInfo(uid) ON UPDATE CASCADE ON DELETE CASCADE,

    constraint fk_Friends_fuid FOREIGN KEY(fuid)
    REFERENCES UserInfo(uid) ON UPDATE CASCADE ON DELETE CASCADE
)DEFAULT CHARSET=utf8mb4;


#3、群表 (群ID、群主ID，群昵称)
create table GroupTable(
    gid int primary key AUTO_INCREMENT,
    guid int,
    gname varchar(64),

    constraint fk_GroupTable_guid FOREIGN KEY(guid)
	references UserInfo(uid) ON UPDATE CASCADE ON DELETE CASCADE
)DEFAULT CHARSET=utf8mb4;
ALTER TABLE GroupTable AUTO_INCREMENT = 1000;

#4、群友表      （群ID、用户ID）
create table GroupFriend(
	gid int,
    guid int,

    constraint  pk_GroupFriend PRIMARY KEY(gid,guid),

    constraint fk_GroupFrine_gid FOREIGN KEY(gid)
    REFERENCES GroupTable(gid) ON UPDATE CASCADE ON DELETE CASCADE,

    constraint fk_GroupFriend_guid FOREIGN KEY(guid)
	references UserInfo(uid) ON UPDATE CASCADE ON DELETE CASCADE
)DEFAULT CHARSET=utf8mb4;

#5、消息表      （用户ID、群聊ID|好友ID、消息类型、消息体，消息状态,创建时间）
#                                                 (待发送|已发送）
create table MessageTable(
    msgid int PRIMARY KEY AUTO_INCREMENT,
	uid int,
    toid int,
    mtype int,
    mData TEXT,
    mStatus INT,
    cTime varchar(20)
)DEFAULT CHARSET=utf8mb4;
ALTER TABLE MessageTable AUTO_INCREMENT = 1000;
