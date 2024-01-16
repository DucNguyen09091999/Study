#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>

/*this program show how mmap() can be used to memory map the text file present on disk into process's virtual address space*/

static void breakpoint()
{

}

typedef struct student_ {
    int roll_no;
    int mark;
    int name[128];
    int city[128];
}student_t;


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("File path not mentioned\n");
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];

    /*Open the file in R/W mode*/
    int fd = open(file_path, O_RDWR);

    if (fd < 0)
    {
        printf("\n\"%s\"could not open\n", file_path);
        exit(EXIT_FAILURE);
    }

    /*Extract the size of the file*/
    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    if (err < 0)
    {
        printf("\n\"%s\"could not open\n", file_path);
        exit(EXIT_FAILURE);
    }

    char *ptr = mmap(NULL, /*Let kernel decide the starting address of mapping in virtual memory*/
                    statbuf.st_size, /*Memory size to Map*/
                    PROT_READ | PROT_WRITE, /*Map the memory in R/W mode, meaning the virtual memory can be R/W by the process.
                                                Note that this permission must be compatible with permissions with which the file is open using open()*/
                    MAP_SHARED, /*To see the change in the file, should be always MAP_SHARED*/,
                    fd, /*FD represents the identifier of the external data source, in this case it is a file*/,
                    0, /*Offset into the text file from which the mapping has to begin, pass zero to map from the beginning of the file*/);
    if (ptr == MAP_FAILED)
    {
        printf("Mapping failed errno = %d\n",errno);
        exit(EXIT_FAILURE);
    }

    /*Will not going to R/W into file using file-handling routines, hence close the fd, mmap can still use this file event if it's fd is closed*/
    close(fd);

    student_t stud = {123,90,"Abhishek", "Bangalore"};
    memcpy(ptr, &stud, sizeof(stud));
    msync(ptr, sizeof(stud),MS_SYNC);
    err = munmap(ptr,statbuf.st_size); /*Destroy the mapping once done*/

    if (err != 0)
    {
        printf("Unmapping failed\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
