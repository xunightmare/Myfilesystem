//
// Created by xnightmare on 12/2/18.
//

#include "main.h"

extern char path[];
extern User cur_user;

int init_fs(void)
{
    fseek(Disk, SuperBeg, SEEK_SET);
    fread(&super_blk, sizeof(SuperBlk), 1, Disk);//读取超级块

    inode_num=0;//当前根目录的inode为0

    if(!open_dir(inode_num)){
        printf("CANT'T OPEN ROOT DIRECTORY\n");
        return 0;
    }

    user_num = get_usernum();

    return 1;
}

int close_fs(void)
{
    fseek(Disk,SuperBeg,SEEK_SET);
    fwrite(&super_blk, sizeof(SuperBlk), 1, Disk);
    close_dir(inode_num);
    return 1;
}

int format_fs(void)
{
    /*格式化inode_map,保留根目录*/
    memset(super_blk.inode_map, 0, sizeof(super_blk.inode_map));  // 将超级块中的所有inode位图全部清零，全部未使用
    super_blk.inode_map[0]=1;  // 保留第一个inode，因为第一个inode是根目录
    super_blk.inode_used=1;  // 设置已经被使用的结点数目为1
    /*格式化blk_map,保留第一个磁盘块给根目录*/
    memset(super_blk.blk_map, 0, sizeof(super_blk.blk_map));  // 和上面操作基本相同，只是换了对象
    super_blk.blk_map[0]=1;
    super_blk.blk_used=1;

    inode_num=0;//将当前目录改为根目录  当前inode的编号为0，即为根目录

    // 用覆盖的方式打开原来的文件
    Disk = fopen(DISK, "w");
    // 初始化超级块
    fseek(Disk, SuperBeg, SEEK_SET);
    fwrite(&super_blk, sizeof(SuperBlk), 1, Disk);//读取超级块


    //初始化根目录的i结点
    Inode temp;
    fseek(Disk, InodeBeg, SEEK_SET);    // 定位到根目录的i结点的位置
    fread(&temp, sizeof(Inode), 1, Disk);  // 从中读取一个inode的内容
    temp.blk_num = 1;
    temp.blk_identifier[0] = 0;
    temp.type = Directory;
    temp.file_size = 2*sizeof(Dir);
    temp.uid = 0;
    temp.gid = 0;
    temp.right_owner = 1;
    temp.right_group = 1;
    temp.right_others = 1;
    /*将初始化完毕的Inode结构写回*/
    fseek(Disk, InodeBeg, SEEK_SET);
    fwrite(&temp, sizeof(Inode), 1, Disk);

    Dir dot[2];
    strcpy(dot[0].name,".");//指向目录本身
    dot[0].inode_num=0;
    strcpy(dot[1].name,".."); // 根目录的上级目录也指向本身
    dot[1].inode_num=0;
    /*将新目录的数据写进数据块*/
    fseek(Disk, BlockBeg, SEEK_SET);
    fwrite(dot, sizeof(Dir), 2, Disk);

    /*读取根目录的i节点*/
    fseek(Disk, InodeBeg, SEEK_SET);
    fread(&curr_inode, sizeof(Inode), 1, Disk);  // 读取第一个inode存储到curr_inode中，当前inode是根目录的inode

    curr_inode.file_size = 2*sizeof(Dir);  // 因为还剩下两个Dir结构体
    curr_inode.blk_num = 1;  // 占用了一个磁盘块
    curr_inode.blk_identifier[0] = 0;//第零块磁盘一定是根目录的

    /*仅.和..目录项有效*/
    dir_num = 2;

    strcpy(dir_table[0].name,"."); // 当前路径下的全部目录文件
    dir_table[0].inode_num=0;
    strcpy(dir_table[1].name,"..");
    dir_table[1].inode_num=0;
    printf("1%s  %s", dir_table[0].name, dir_table[0].name);


    // 初始化用户信息
    User user;
    fseek(Disk, UserBeg, SEEK_SET);    // 定位到根目录的i结点的位置
    fread(&user, sizeof(User), 1, Disk);  // 从中读取一个inode的内容
    strcpy(user.username, "root");
    user.uid = 0;
    user.gid = 0;
    strcpy(user.passwd, "root");
    user.flag = -1;
    fseek(Disk, UserBeg, SEEK_SET);
    fwrite(&user, sizeof(User), 1, Disk);

    // 关闭文件后用“r+”模式重新打开
    fclose(Disk);
    Disk = fopen(DISK,"r+");
    strcpy(path, cur_user.username);

//    User user2;
//    fseek(Disk,UserBeg,SEEK_SET);    // 定位到根目录的i结点的位置
//    fread(&user2,sizeof(User),1,Disk);  // 从中读取一个inode的内容
//    printf("hhh%d   %s", user2.uid, user2.passwd);

    return 1;
}