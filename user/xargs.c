#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

char*
readLine(char *buf)
{   

    char *p = buf;
    //char *tep = "";
    char tep[512];
    do{
        int num = read(0, tep, 1);
        if(num == 0){
            return (void *)0;
        }
        if(tep[0] == '\n'){
            break;
        }
        *p = tep[0];
        p++;
    }while(tep[0] != '\n');
    
    *p = '\0';
    return buf;
}

int
main(int argc, char *argv[])
{
    char *buf = "";

    char path[] = "./";
    char *func = argv[1];
    char *tail = path + strlen(path);
    strcpy(tail, func);
    func = tail;

    while(readLine(buf) != (void *)0){
        int pid = fork();
        if(pid == 0){
            int i;
            char *args[MAXARG];
            
            for(i = 1; i < argc; i++){
                args[i-1] = argv[i];
            }

            args[i-1] = buf;
            exec(func, args);
        }else{
            int status;
            wait(&status);
        }
    }
    exit(0);
}