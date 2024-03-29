#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print all the dirs starting from <path> [maybe recursive]. */
int print_dirs(const char *path, int recursive)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;

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
                printf("%s\n", full_name);
                /*In case we don't want to get sub folder. recursive is 0*/
                if (recursive)
                    print_dirs(full_name, 1);
            }
        }
    }

    /* Finalize resources. */
    (void)closedir(dirp);
    return 0;
}

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        printf("Syntax Error\nHint:./list-all-dir <path>\n");
        return -1;
    }
        

    print_dirs(argv[1], 1);
    return 0;
}