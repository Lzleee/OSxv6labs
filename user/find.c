#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  //static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

//   // Return blank-padded name.
//   if(strlen(p) >= DIRSIZ)
//     return p;
//   memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return p;
}

void
find(char *path, char *file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        if(strcmp(file, fmtname(path)) == 0){
            printf("%s\n", path);
        }
        //printf("hello %s, %d\n",fmtname(path), strcmp(file, fmtname(path)));
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            if(strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0){
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0';
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            //printf("%s\n", buf);
            find(buf, file);
        }
        break;
    }
    close(fd);
    return;
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(1, "parameter error\n");
        exit(1);
    }else{
        char *dir = argv[1];
        char *file = argv[2];

        find(dir, file);
    }

    exit(0);
}
