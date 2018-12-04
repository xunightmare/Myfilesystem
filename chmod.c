//
// Created by xnightmare on 12/3/18.
//
#include "main.h"

extern User cur_user;

int chmod(char *filename){
    if(cur_user.uid != 0){ // 设定只有root才能使用这个命令
        printf("Only the root user can use chmod!!!\n");
        return 0;
    }


    for(int i=0; i<dir_num; i++){
        if(!strcmp(filename, dir_table[i].name)){
            Inode temp;
            fseek(Disk, InodeBeg+dir_table[i].inode_num*sizeof(Inode), SEEK_SET);
            fread(&temp, sizeof(Inode), 1, Disk);
            printf("Please input the new right of owner: ");
            scanf("%d", &temp.right_owner);
            printf("Please input the new right of group: ");
            scanf("%d", &temp.right_group);
            printf("Please input the new right of others: ");
            scanf("%d", &temp.right_others);
            fseek(Disk, InodeBeg+dir_table[i].inode_num*sizeof(Inode), SEEK_SET);
            fwrite(&temp, sizeof(Inode), 1, Disk);
            return 1;
        }
    }
    printf("Can't find the file\n");
    return 0;
}
