#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET    "\x1b[0m"

/* Print all the dirs starting from <path> [maybe recursive]. */
int list_dir(const char *path)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;
    size_t line_len;
    /* Check input parameters. */
    if (!path)
    {
        exit(EXIT_FAILURE);
    }
    
    /*Get length of ingress path*/
    path_len = strlen(path);  

    if (!path || !path_len || (path_len > _POSIX_PATH_MAX))
    {
        exit(EXIT_FAILURE);
    }
        

    /* Open directory */
    dirp = opendir(path);
    if (dirp == NULL)
    {
        exit(EXIT_FAILURE);
    }
       

    while ((direntp = readdir(dirp)) != NULL)
    {
        /* For every directory entry... */
        struct stat fstat;
        char full_name[_POSIX_PATH_MAX + 1];

        /* Calculate full name, check we are in file length limts */
        if ((path_len + strlen(direntp->d_name) + 1) > _POSIX_PATH_MAX)
            continue;

        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
        {
            strcat(full_name, "/");
        }
            
        strcat(full_name, direntp->d_name);

        /*In case we want to get specific folder '.', '..'. Remove this condition*/
        if (!((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0)))
        {
            /* Print only if it is really directory. */
            if (stat(full_name, &fstat) < 0)
                continue;
            if (S_ISDIR(fstat.st_mode))
            {
                printf(ANSI_COLOR_BLUE  "%s  "ANSI_COLOR_RESET, direntp->d_name);
                line_len += strlen(direntp->d_name);
                //printf("%ld\t", line_len);
                if (line_len > 200)
                {
                    line_len = 0;
                    printf("\n");
                }
            }
        }
        
    }
    printf("\n");
    /* Finalize resources. */
    (void)closedir(dirp);
    return 0;
}

int main(int argc, char *argv[])
{
    char dir[1024][50];
    int dir_idx;
    int dir_num = 0;
    char buf[10];
    int argv_idx;
    if (argc < 2)
    {
        /*print file + folder in current directory*/
        getcwd(dir[0], sizeof(dir));
        printf("Current working directory: %s\n",dir[0]);
    } 
    else
    {
        for (argv_idx = 1; argv_idx < argc; argv_idx++)
        {
            atoi(strncpy(buf, argv[argv_idx] + 0,1));
            if(strcmp(buf, "-") == 0)
            {
                printf("%s is option\n", argv[argv_idx]);
            }
            else
            {
                strcpy(dir[dir_num], argv[argv_idx]);
                //printf("%s is directory\n", dir[dir_num]);
                dir_num +=1;
            }
        }

        //printf("Num of directory: %d\n",dir_num);
        for (dir_idx = 0; dir_idx < dir_num; dir_idx++)
        {
            printf("%s:\n", dir[dir_idx]);
            list_dir(dir[dir_idx]);
            if (dir_idx == 0)
            {
                printf("\n");
            }
            
            
        }
        
    }

    return 0;

}