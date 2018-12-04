//
// Created by xnightmare on 12/2/18.
//

#include "main.h"

User cur_user;  // 用来存储当前的用户

int check_user(int uid, char * passwd);

int login(){
    int uid;
    char passwd[MaxPasswd];
    printf("\n----Login----\nPlease input your userid:");
    scanf("%u",&uid);
    printf("Please input your password:");
    scanf("%s", passwd);
    if(!check_user(uid, passwd)){
        printf("Sorry, login failed...\n");
        exit(0);
    }else{
        printf("Welcome, login success!\n");
    }
}


int check_user(int uid, char * passwd){
    for(int i=0; i<user_num; i++){
        User user;
        fseek(Disk,UserBeg+i*sizeof(User),SEEK_SET);
        fread(&user,sizeof(User),1,Disk);  // 从中读取一个inode的内容
        if((user.uid==uid) && (!strcmp(passwd, user.passwd))){
            strcpy(cur_user.username, user.username);
            cur_user.uid = user.uid;
            cur_user.gid = user.gid;
            strcpy(cur_user.passwd, user.passwd);
            cur_user.flag = user.flag;
            return 1;   // 用户验证通过
        }
    }
    return 0;
}

