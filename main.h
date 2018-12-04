#ifndef __MAIN_H__
#define __MAIN_H__

// 头文件区
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 全局宏区
#define InodeNum	1024//i节点数目
#define BlkNum		(80*1024)//磁盘块的数目
#define BlkSize		1024//磁盘块大小为1K
#define BlkPerNode	1024//每个文件包含的最大的磁盘块数目
#define DISK 		"disk.txt"
#define BUFF		"buff.txt"//读写文件时的缓冲文件
#define SuperBeg	0//超级块的起始地址
//#define InodeBeg sizeof(SuperBlk)
#define InodeBeg	(UserBeg+sizeof(User)*MaxUserNum)//i节点区起始地址
#define BlockBeg	(InodeBeg+InodeNum*sizeof(Inode))//数据区起始地址
#define MaxDirNum	(BlkPerNode*(BlkSize/sizeof(Dir)))//每个目录最大的文件数
#define DirPerBlk	(BlkSize/sizeof(Dir))//每个磁盘块包含的最大目录项
#define Directory	0
#define File		1
#define CommanNum	(sizeof(command)/sizeof(char*))//指令数目
#define MaxUserNum 10 // 文件系统最多支持的用户数目
#define UserBeg sizeof(SuperBlk)
#define MaxPasswd 20
#define MaxUserName 20 // 用户名的最大长度

//结构体区
typedef struct{
    int inode_map[InodeNum];//i节点位图
    int blk_map[BlkNum];//磁盘块位图
    int inode_used;//已被使用的i节点数目
    int blk_used;//已被使用的磁盘块数目
}SuperBlk;

typedef struct{
    int blk_identifier[BlkPerNode];//占用的磁盘块编号
    int blk_num;//占用的磁盘块数目
    int file_size;//文件的大小
    int type;//文件的类型
    int uid; // 文件的所有者
    int gid; // 文件所在组
    int right_owner;  // 拥有者的权限
    int right_group;  // 群组的权限
    int right_others;  // 其他人的权限
}Inode;

typedef struct{
    char name[30];//目录名
    short inode_num;//目录对应的inode
}Dir;

// 用户结构体
typedef struct{
    char username[MaxUserName];
    unsigned short uid;
    unsigned short gid;
    char passwd[MaxPasswd];
    int flag;  // 用于标志是否为最后一个用户，为最后一个用户为-1，否则为1
}User;

// 全局变量区
Dir 	dir_table[MaxDirNum];//将当前目录文件的内容都载入内存
int 	dir_num;//相应编号的目录项数
int	 	inode_num;//当前目录的inode编号
Inode 	curr_inode;//当前目录的inode结构
SuperBlk	super_blk;//文件系统的超级块
FILE*	Disk;
User users[MaxUserNum];  // 用于存储用户信息
int user_num;  // 记录已经有的用户的数量




// 函数接口区
// file.c中的函数
extern int open_dir(int);
extern int close_dir(int);//保存相应inode的目录
extern int show_dir(int);//显示目录

extern int make_file(int,char*,int);//创建新的目录或文件
extern int del_file(int,char*,int);//删除子目录
extern int enter_dir(int,char*);//进入子目录

extern int file_write(char*);//写文件
extern int file_read(char*);//读文件

extern int adjust_dir(char*);//删除子目录后，调整原目录，使中间无空隙

extern int check_name(int,char*);//检查重命名,返回-1表示名字不存在，否则返回相应inode
extern int type_check(char*);//确定文件的类型

extern int free_inode(int);//释放相应的inode
extern int apply_inode();//申请inode,返还相应的inode号，返还-1则INODE用完
extern int init_dir_inode(int,int);//初始化新建目录的inode
extern int init_file_inode(int);//初始化新建文件的inode

extern int free_blk(int);//释放相应的磁盘块
extern int get_blk(void);//获取磁盘块

extern void change_path(char*);
extern int check_read_right(char * filename);
extern int check_write_right(char * filename);

// fs.c中的函数
int init_fs(void);//初始化文件系统
int close_fs(void);//关闭文件系统
int format_fs(void);//格式化文件系统

// login.c中的函数
extern int login();
extern int add_user();
extern int get_usernum();
extern int check_uid(int);

//uses.c中的函数
extern int show_users();
extern int del_user();
extern int check_username(char * username);

// chmod.c中的函数
extern int chmod(char *filename);

#endif
