#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
    int p[2];
    int i;

    close(0);
    pipe(p);
    
    for(i = 2; i <= 35; i++){
        write(p[1], &i, 4);
    }
    close(p[1]);
    int prime;
    while(read(p[0], &i, 4) != 0){
        int pid = fork();
        if(pid == 0){
            int pChild[2];

            pipe(pChild);
            prime = i;

            fprintf(1, "prime %d\n", prime);
            while(read(p[0], &i, 4) != 0){
                if(i % prime != 0){
                    write(pChild[1], &i, 4);
                }
            }
            close(pChild[1]);
            close(p[0]);
            p[0] = pChild[0];
        }else{
            close(p[0]);
            int status;
            wait(&status);
            break;
        }
    }
    exit(0);
}