#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    /*command: getconf PAGE_SIZE*/
    printf("Page size of sysconf is %ld\n", sysconf(_SC_PAGESIZE));
    printf("Page size of getpagesize function is %d\n", getpagesize());
    return 0;
}