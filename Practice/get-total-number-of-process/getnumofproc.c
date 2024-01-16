#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/*
I had need in something like this not so long ago (my difference is I
needed recursive scan) so I added only some comments... Sorry for recursion
but I was short of time and this was only part of internal one-time tool.
*/
typedef int status;
#define DRV_SUCCESS 0
#define DRV_FAILED  -1
/* Print all the dirs starting from <path> [maybe recursive]. */
status num_of_process(const char *path, int *total_process, int *total_task)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;
    int Status = DRV_SUCCESS;

    *total_task = 0;
    *total_process = 0;
    
    /* Check input parameters. */
    if (!path)
    {
        Status = DRV_FAILED;
        exit(EXIT_FAILURE);
    }

    /* Open directory */
    dirp = opendir(path);
    if (dirp == NULL)
    {
        Status = DRV_FAILED;
        exit(EXIT_FAILURE);
    }

    /*loop through all task and process in /proc*/
    while ((direntp = readdir(dirp)) != NULL)
    {
        char buf[10];
        /*In case folder is '.' and '.'. Ignore it*/
        if (!((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0)))
        {
            /*in case the first letter is numbers -> count process*/
            if (atoi(strncpy(buf, direntp->d_name + 0,1)))
            {
                *total_process +=1;
            }
            else /*count task*/
            {
                *total_task+=1;
            }
            
        }
    }
    /* Finalize resources. */
    (void)closedir(dirp);
    return Status;
}

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    /*Declare variable*/
    int process_num;
    int task_num;
    int Status;

    /*Get number of process and task in /proc folder*/
    Status = num_of_process("/proc", &process_num, &task_num);

    printf("Total process is %d\n", process_num);
    printf("Total task is %d\n", task_num);
    return 0;
}