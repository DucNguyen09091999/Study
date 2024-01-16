#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//You may also add more include directives as well.

// THIS VALUE CANNOT BE CHANGED.
// You should use this value when creating your buffer.
// And also to limit the amount of bytes each read CAN do.
#define BUFF_MAX 21
// ABOVE VALUE CANNOT BE CHANGED //

#define NAME "Brian Lai"

int main(int argc, char const *argv[]) {
    int input_file, output_file;
    char buffer[BUFF_MAX];

    if (argc < 3)
    {
        printf("Syntax Error\nHint: ./cp <src_file> <dest_file>\n");
        return -1;
    }

    // Open source file for read only
    input_file = open(argv[1], O_RDONLY);
    if (input_file < 0) {
        printf("Open for read failed: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Overwrite if file exists | Create if file does not exist | Write only
    output_file = open(argv[2], O_TRUNC | O_CREAT | O_WRONLY, S_IWUSR);
    if (output_file < 0) {
        printf("Open for write failed: %s\n", argv[2]);
        close(input_file);
        exit(EXIT_FAILURE);
    }

    // Copy permissions
    struct stat st;
    stat(argv[1], &st);
    chmod(argv[2], st.st_mode);

    // Copy contents with buffer limit of BUFF_MAX until entire file is copied
    ssize_t total_bytes_copied = 0;
    for (;;) {
        ssize_t bytes_read = read(input_file, buffer, BUFF_MAX);
        if (bytes_read <= 0) {
            break;
        }

        ssize_t bytes_written = write(output_file, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("bytes_written != bytes_read failed\n");
            printf("bytes_read: %zd\n", bytes_read);
            printf("bytes_written: %zd\n", bytes_written);
            close(input_file);
            close(output_file);
            exit(EXIT_FAILURE);
        }

        total_bytes_copied += bytes_written;
    }

    printf("File Copy Successful, %zd bytes copied\n", total_bytes_copied);

    close(input_file);
    close(output_file);
    
    return 0;
}