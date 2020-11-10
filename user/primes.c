#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[]) 
{   
    const int N = 34;
    int nums[N];
    for(int i = 0; i < N; ++i) { 
        nums[i] = i + 2;
    }
    
    int fd[2];

    pipe(fd);
    
    write(fd[1], nums, sizeof(nums));

    while(1) {
        int pid = fork();
        if(pid == -1) {
            exit(1);
        } else if(pid == 0) {
            // child process
            // fetch the nums from pipe
            close(fd[1]);
            read(fd[0], nums, N * 4);
            close(fd[0]);

            int divider = nums[0];

            if(divider == -1) {
                
                
                exit(0);
            }
            
            printf("prime %d\n", divider);
            
            for(int j = 0, i = 1; i < N; ++i) {
                if(nums[i] % divider != 0) {
                    nums[j++] = nums[i];
                }
                nums[i] = -1;
            }
        } else {
            // parent process
            wait(&pid);
            break;
        }
    }

    exit(0);
}