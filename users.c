//
// Created by xnightmare on 12/3/18.
//
#include "main.h"

// 新建用户
int add_user(){
    if(user_num >= MaxUserNum){   //检查用户数是否已满
        printf("Too many users!");
        return -1;
    }

    // 读取新用户的信息
    char username[MaxUserName];
    int uid;
    int gid;
    char passwd[MaxPasswd];

    do{
        printf("Please input the username of the new user: ");
        scanf("%s", username);
    }while(!check_username(username));

    do{
        printf("Please input the uid of the new user: ");
        scanf("%d", &uid);
    }while(!check_uid(uid));


    printf("Please input the gid of the new user: ");
    scanf("%d", &gid);
    printf("Please input the password of the new user: ");
    scanf("%s", passwd);
    printf("%s", passwd);


    // 写入新用户的信息
    User user;
    fseek(Disk, UserBeg+user_num*sizeof(User),SEEK_SET);
    fread(&user, sizeof(User),1,Disk);  // 从中读取一个inode的内容
    strcpy(user.username, username);
    user.uid = uid;
    user.gid = gid;
    strcpy(user.passwd, passwd);
    user.flag = -1;
    fseek(Disk, UserBeg+user_num*sizeof(User), SEEK_SET);
    fwrite(&user, sizeof(User), 1, Disk);

    User user3;

    // 修改上一用户的标志位
    if(user_num >= 1){
        User user2;
        fseek(Disk, UserBeg+(user_num-1)*sizeof(User), SEEK_SET);
        fread(&user2, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        user2.flag = 1;
        fseek(Disk, UserBeg+(user_num-1)*sizeof(User), SEEK_SET);
        fwrite(&user2, sizeof(User), 1, Disk);

        fseek(Disk, UserBeg+(user_num-1)*sizeof(User), SEEK_SET);
        fread(&user3, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
    }
    show_users();

    // 修改user_num的值
    user_num++;

}

// 删除用户
int del_user(){
    int uid;
    printf("Please input the uid to delete: ");
    scanf("%d", &uid);


    if(uid == 0){
        printf("Can't delete the root account!\n");
        return -1;
    }

    int flag = 0;  // 0表示没有找到，1表示找到了
    User user; // 保存找到的用户信息
    int no; // 记录用户在磁盘中的位置

    // 找到要删除的用户
    for(int i=0; i<user_num; i++){
        fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
        fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        if(user.uid == uid){
            printf("Find the user!\n");
            flag = 1;
            no = i;
            break;
        }
    }
    if(flag == 0){
        printf("Can't find the user!\n");
        return -1;   // 找不到用户，删除失败
    }

    if(user.flag == -1){ //如果是最后一个用户，就只修改前一个用户的指针即可
        User user2;
        fseek(Disk, UserBeg+(no-1)*sizeof(User), SEEK_SET);
        fread(&user2, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        user2.flag = -1;
        fseek(Disk, UserBeg+(no-1)*sizeof(User), SEEK_SET);
        fwrite(&user2, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
    }else{  // 如果不是最后一个用户，那需要把之前的用户依次前移
        User user3;
        for(int i=no+1; i<user_num; i++){
            fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
            fread(&user3, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
            fseek(Disk, UserBeg+(i-1)*sizeof(User), SEEK_SET);
            fwrite(&user3, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        }
    }
    user_num--;
}

int check_uid(int uid){
    for(int i=0; i<user_num; i++){
        User user;
        fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
        fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        if(user.uid == uid){
            printf("uid is already exist, please choose another one!\n");
            return 0;   // 验证失败，需要选择一个新的uid
        }
    }
    return 1;  //认证成功
}


int check_username(char * username){
    for(int i=0; i<user_num; i++){
        User user;
        fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
        fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        if(!strcmp(user.username, username)){
            printf("username is already exist, please choose another one!\n");
            return 0;   // 验证失败，需要选择一个新的username
        }
    }
    return 1;  //认证成功
}

int get_usernum(){
    for(int i=0; i<MaxUserNum; i++){
        User user;
        fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
        fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        if(user.flag == -1)
            return i+1;   // 用户验证通过
    }
    printf("Sorry, can't get the correct usernum\n");
    printf("exit ...\n");
    exit(-1);
}

int show_users(){ // 展示所有的用户信息
    printf("Totally %d user%c\n", user_num, (user_num>1)?'s':':');
    for(int i=0; i<MaxUserNum; i++){
        User user;
        fseek(Disk, UserBeg+i*sizeof(User), SEEK_SET);
        fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
        printf("username=%s uid=%d gid=%d passwd=%s flag=%d\n", user.username, user.uid, user.gid, user.passwd, user.flag);
    }
}