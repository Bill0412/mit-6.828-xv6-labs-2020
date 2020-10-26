#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p_child_write[2];
    int p_parent_write[2];
    int ret = pipe(p_child_write);
    if(ret == -1) exit(1);
    ret = pipe(p_parent_write);
    if(ret == -1) exit(1);

    int pid = fork();

    // the parent process
    if(pid < 0) {
        exit(1);
    }
    if(pid > 0) {
        char buf1[100];
        char str1[] = "ping";
        memset(buf1, '\0', sizeof(buf1));
        write(p_parent_write[1], str1, strlen(str1)+1);
        close(p_parent_write[1]);
        sleep(1);
        read(p_child_write[0], buf1, sizeof(buf1));
        close(p_child_write[0]);
        printf("%d: received %s\n", getpid(), buf1);
    } else { 
        // the child process
        char buf2[100];
        char str2[] = "pong";
        memset(buf2, '\0', sizeof(buf2));
        write(p_child_write[1], str2, strlen(str2)+1);
        close(p_child_write[1]);
        sleep(0.5);
        read(p_parent_write[0], buf2, sizeof(buf2));
        close(p_parent_write[0]);
        printf("%d: received %s\n", getpid(), buf2);
    }

    exit(0);
}