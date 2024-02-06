#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
    int p[2];
    char args[1];

    pipe(p);
    if(fork() == 0){//child
        if(read(p[0], args, sizeof args) == 1){
            fprintf(1, "%d: received ping\n", getpid());
        }
        write(p[1], "1", 1);
    } else { //parent
        write(p[1], "1", 1);
        if(read(p[0], args, sizeof args) == 1){
            fprintf(1, "%d: received pong\n", getpid());
        }
    }
    close(p[0]);
    close(p[1]);
    exit(0);
}