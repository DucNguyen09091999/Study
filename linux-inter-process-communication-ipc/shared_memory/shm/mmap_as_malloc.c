#include <stdio.h>
#include <sys/mman.h>

/*mmap() has multiple purpose, this is the example where program simply do dynamic memory allocation using mmap() instead of malloc() call.
there is no external data source or memory mapping involved. this example simply show how mmapp() can be used as substituition for malloc() free()*/

#if 0
    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    int munmap(void *addr, size_t length);
#endif

int main(int argc, char *argv[])
{
    int N=5;
    int *mmapptr = mmap(NULL, /*The Os choose the starting virtual address of the memory, just like you do not have a control as to what address malloc() will return*/
                        N*sizeof(int), /*Size of memory int bytes being requested or allocation*/
                        PROT_READ | PROT_WRITE, /*Permission: Memory is readable/writeable*/
                        MAP_PRIVATE | /*This memory is not shareable with any other process, use MAP_SHARED instead*/
                        MAP_ANONYMOUS, /*This memory is not mapped to external data source but only RAM by default*/
                        0, /*FD 0, since no external data source is specified*/
                        0); /*offset value as zero, since no external data source is specified*/

    if (mmapptr == MAP_FAILED)
    {
        printf("Mapping failed\n");
        return -1;
    }

    for (int i = 0; i < N; i++)
    {
        mmapptr[i] = i*10;
    }

    for (int i = 0; i < N; i++)
    {
        printf("[%d]",mmapptr[i]);
    }
    printf("\n");

    int err = munmap(mmapptr, 10 * sizeof(int));
    if (err != 0)
    {
        printf("Unmapping failed\n");
        return -1;
    }
    return 0;
}