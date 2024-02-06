#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
    int i;

    if(argc != 2){
        fprintf(2, "Please enter a number.\n");
        exit(1);
    } else {
        i = atoi(argv[1]);
        sleep(i);
        fprintf(1, "%d\n", i);
        exit(0);
    }

}