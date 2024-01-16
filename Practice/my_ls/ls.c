#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>


//  struct stat { /* when _DARWIN_FEATURE_64_BIT_INODE is defined */
//      dev_t           st_dev;           /* ID of device containing file */
//      mode_t          st_mode;          /* Mode of file (see below) */
//      nlink_t         st_nlink;         /* Number of hard links */
//      ino_t           st_ino;           /* File serial number */
//      uid_t           st_uid;           /* User ID of the file */
//      gid_t           st_gid;           /* Group ID of the file */
//      dev_t           st_rdev;          /* Device ID */
//      struct timespec st_atimespec;     /* time of last access */
//      struct timespec st_mtimespec;     /* time of last data modification */
//      struct timespec st_ctimespec;     /* time of last status change */
//      struct timespec st_birthtimespec; /* time of file creation(birth) */
//      off_t           st_size;          /* file size, in bytes */
//      blkcnt_t        st_blocks;        /* blocks allocated for file */
//      blksize_t       st_blksize;       /* optimal blocksize for I/O */
//      uint32_t        st_flags;         /* user defined flags for file */
//      uint32_t        st_gen;           /* file generation number */
//      int32_t         st_lspare;        /* RESERVED: DO NOT USE! */
//      int64_t         st_qspare[2];     /* RESERVED: DO NOT USE! */
//  };

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{

   struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    struct stat file_stats;
    /* Check input parameters. */
    if (!argv[1])
    {
        exit(EXIT_FAILURE);
    }

    /* Open directory */
    dirp = opendir(argv[1]);
    if (dirp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        char full_name[_POSIX_PATH_MAX + 1];
        
        if (direntp->d_type == DT_REG)
        {
            strcpy(full_name,argv[1]);
            strcat(full_name, direntp->d_name);
            if ((stat(full_name, &file_stats)) == -1) 
            {
                    
                     perror("fstat");
                     exit(EXIT_FAILURE);
            }
              printf("File type:                ");

            switch (file_stats.st_mode & S_IFMT) {
             case S_IFBLK:  printf("block device\n");            break;
             case S_IFCHR:  printf("character device\n");        break;
             case S_IFDIR:  printf("directory\n");               break;
             case S_IFIFO:  printf("FIFO/pipe\n");               break;
             case S_IFLNK:  printf("symlink\n");                 break;
             case S_IFREG:  printf("regular file\n");            break;
             case S_IFSOCK: printf("socket\n");                  break;
             default:       printf("unknown?\n");                break;
             }

            printf("I-node number:            %ld\n", (long) file_stats.st_ino);

            printf("Mode:                     %lo (octal)\n",
                     (unsigned long) file_stats.st_mode);

            printf("Link count:               %ld\n", (long) file_stats.st_nlink);
             printf("Ownership:                UID=%ld   GID=%ld\n",
                     (long) file_stats.st_uid, (long) file_stats.st_gid);

            printf("Preferred I/O block size: %ld bytes\n",
                     (long) file_stats.st_blksize);
             printf("File size:                %lld bytes\n",
                     (long long) file_stats.st_size);
             printf("Blocks allocated:         %lld\n",
                     (long long) file_stats.st_blocks);

            printf("Last status change:       %s", ctime(&file_stats.st_ctime));
             printf("Last file access:         %s", ctime(&file_stats.st_atime));
             printf("Last file modification:   %s", ctime(&file_stats.st_mtime));
                 }



        
    }
    printf("\n");



    /* Finalize resources. */
    (void)closedir(dirp);
    return 0;
}