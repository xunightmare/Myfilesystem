#include "main.h"

/*指令集合*/
char*	command[]={"fmt","quit","mkdir","rmdir","cd","ls","mk","rm","vim", "adduser","userdel", "showusers","whoami", "chmod"};
extern char	path[40] = "";
extern User cur_user;


int main()
{
    char comm[30],name[30];
    char *arg[]={"vim", BUFF,NULL};
    char *arg_readonly[]={"vim", "-R", BUFF,NULL};
    int i,quit=0,choice,status;

    if(!(Disk = fopen(DISK,"r+"))){  // 如果磁盘文件不存在，先进行格式化创建磁盘文件文件
        printf("null");
        format_fs();
    }
    // 读出编号为1和2的磁盘块信息，用于调试
    Inode temp_inode;
    fseek(Disk, InodeBeg+sizeof(Inode), SEEK_SET);
    fread(&temp_inode, sizeof(Inode), 1, Disk);
    printf("uid=%d gid=%d right=%d %d %d\n", temp_inode.uid, temp_inode.gid, temp_inode.right_owner, temp_inode.right_group, temp_inode.right_others);
    Inode temp_inode2;
    fseek(Disk, InodeBeg+sizeof(Inode)*2, SEEK_SET);
    fread(&temp_inode2, sizeof(Inode), 1, Disk);
    printf("uid=%d gid=%d right=%d %d %d\n", temp_inode2.uid, temp_inode2.gid, temp_inode2.right_owner, temp_inode2.right_group, temp_inode2.right_others);

    init_fs();  //初始化文件系统
    show_users();  // 打印出所有的用户，用于调试

    // 用户登录
    login();
    memset(path, 0, strlen(path));
    strcpy(path, cur_user.username);

    while(1){
        printf("%s# ", path);
        scanf("%s", comm);
        choice=-1;

        for(i=0;i<CommanNum;++i){
            if(strcmp(comm, command[i])==0){
                choice=i;
                break;
            }
        }

        switch(choice){
            /*格式化文件系统*/
            case 0:
                if(cur_user.uid != 0){ // 设定只有root才能使用这个命令
                    printf("Only the root user can use fmt!!!\n");
                }else{
                    format_fs();
                }
                break;
                /*退出文件系统*/
            case 1:
                quit=1;
                break;
                /*创建子目录*/
            case 2:
                scanf("%s", name);
                make_file(inode_num, name,Directory);
                break;
                /*删除子目录*/
            case 3:
                scanf("%s", name);
                if(type_check(name) != Directory){
                    printf("rmdir: failed to remove '%s': Not a directory\n",name);
                    break;
                }
                del_file(inode_num,name,0);
                break;
                /*进入子目录*/
            case 4:
                scanf("%s",name);
                if(type_check(name)!=Directory){
                    printf("cd: %s: Not a directory\n",name);
                    break;
                }
                if(enter_dir(inode_num,name)){
                    change_path(name);//改变路径前缀
                }
                break;
                /*显示目录内容*/
            case 5:
                show_dir(inode_num);
                break;
                /*创建文件*/
            case 6:
                scanf("%s",name);
                make_file(inode_num,name,File);
                break;
                /*删除文件*/
            case 7:
                scanf("%s",name);
                if(type_check(name)!=File){
                    printf("rm: cannot remove '%s': Not a file\n",name);
                    break;
                }
                del_file(inode_num,name,0);
                break;
                /*对文件进行编辑*/
            case 8:
                scanf("%s",name);
                if(type_check(name)!=File){
                    printf("vim: cannot edit '%s': Not a file\n",name);
                    break;
                }
                if(check_read_right(name) == 1){
                    file_read(name);//将数据从文件写入BUFF
                    if(!fork()){
                        if(check_write_right(name) == 1){
                            execvp("vim", arg);
                        }else{
                            execvp("vim", arg_readonly);
                        }
                    }
                    wait(&status);
                    file_write(name);//将数据从BUFF写入文件
                }
                break;
            case 9:
                add_user();
                break;
            case 10:
                del_user();
                break;
            case 11:
                show_users();
                break;
            case 12:
                printf("username=%s uid=%d gid=%d passwd=%s flag=%d\n", cur_user.username, cur_user.uid, cur_user.gid, cur_user.passwd, cur_user.flag);
                break;
            case 13:scanf("%s", name);
                chmod(name);
                break;
            default:
                printf("%s command not found\n", comm);
        }

        if(quit) break;
    }
    close_fs();

    fclose(Disk);
    return 0;
}



