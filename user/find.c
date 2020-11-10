#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Recursively find target in paths
void 
find(char* path, char* target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s**\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s**\n", path);
        close(fd);
        return;
    }

    if(st.type == T_DIR) {
        // if the path length is too long
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                return;
            }

            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                // do not recurse into "." and ".."
                if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    memmove(p, de.name, DIRSIZ);
                    p[DIRSIZ] = 0;
                    if(strcmp(de.name, target) == 0) {
                        printf("%s\n", buf);
                    } else {
                        find(buf, target);
                    }
                }
            }
    }
    
    close(fd);
}

int 
main(int argc, char* argv[])
{
    switch (argc)
    {
    // missing at least 1 argument
    case 1:
        printf("usage: find [directory] [file ...]");
        break;
    
    // find in current directory
    case 2:
        find(".", argv[1]);
        break;

    default:
        for(int i = 2; i < argc; ++i) {
            find(argv[1], argv[i]);
        }
        break;
    }

    exit(0);
}
