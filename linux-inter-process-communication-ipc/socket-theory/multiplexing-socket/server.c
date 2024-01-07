#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
#define MAX_CLIENT_SUPPORTED    32

/*An array of file descriptors which the server process is maintaining in order to talk with the connected clients. 
    Master socket file descriptor is also a member of this array*/
int monitored_fd_set[MAX_CLIENT_SUPPORTED];

/*Each connected client's intermediate result is maintained in this client array. */
int client_result[MAX_CLIENT_SUPPORTED] = {0};

/*Remove all the file descriptors, if any, from the monitored_fd_set array*/
static void initiaze_monitor_fd_set()
{
    int i = 0;
    
    for (i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        /*Set all bit in array to -1 when starts servers. -1 is a representation that the array is empty. */
        monitored_fd_set[i] = -1;
    }
}

/*Add a new file descriptor to the monitored_fd_set array*/
static void add_to_monitored_fs_set(int socket_fds)
{
    int i = 0;

    for(i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] == -1)
        {
            /*when detect a emty position, add socket file descriptors. Find an empty slot in the monitored file descriptor set array.
            Add to monitored file descriptor set and takes an argument */
            monitored_fd_set[i] = socket_fds;
            break;
        }
    }
}

/*Remove the file descriptor from monitored_fd_set array*/

static void remove_from_monitored_fd_set(int socket_fds)
{
    int i = 0;

    for (i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] == socket_fds)
        {
            monitored_fd_set[i] = -1;
        }
    }
}

/*Clone all the FDs in monitored_fd_set array into fd_set data structure. */
static void refresh_fd_set(fd_set *fd_set_ptr)
{
    /*Clears/remove all file descriptor from fd_set_ptr.*/
    FD_ZERO(fd_set_ptr);
    int i = 0;
    for (i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] != -1)
        {
            /*Add monitored file descriptor monitored_fd_set[i] to fd_set_ptr. Recopies all the monitored file descriptors present in monitored_fd_set array to standard structure fd_set_ptr*/
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

/*Get the numerical max value among all FDs which server is monitoring*/
static int get_max_fd()
{
    int i = 0;
    int max = -1;

    for (i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] > max)
        {
            max = monitored_fd_set[i];
        }
    }
    return max;
}

int main(int argc, char *argv[])
{

    struct sockaddr_un name;
    #if 0
        struct sockaddr_un {
            sa_family_t sun_family; /*AF_UNIX */
            char        sun_path[108];
        }
    #endif

    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int comm_socket_fd = 0;
    int i;
    /*Initialize the monitored file descriptor set array with -1.
        When the server has booted up, server is not maintaining any file descriptor. */
    initiaze_monitor_fd_set();
    /*
     *In case the program exited inadvertently on the last run, remove the socket
     **/

    unlink(SOCKET_NAME);

    /*Create Master socket. */
    /*SOCK_DGRAM for datagram based communication*/
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connection_socket == -1)
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }
    printf("Master socket created successfully\n");

    /*Initialize*/
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*specify the socket cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /*Bind socket to socket name. */
    /*Purpose of bind() system call is that application() dictate the underlying operating system the criteria of receiving the data. Here, bind() system call is telling
     the Os that if sender process sends the data destined to socket "/tmp/Socket", then such data needs to be delivered to this process me (the server process)*/
    ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(struct sockaddr_un));
    if (ret == -1)
    {
       perror("Blind");
       exit(EXIT_FAILURE);
    }

    printf("Blind() call successfully\n");

    /*Prepare for accepting connection. The backlog size is set to 20. So, while one request is being processed other requests can be waiting*/

    ret = listen(connection_socket,20);
    if (ret == -1 )
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /*Add master socket to monitored set of FDs*/
    add_to_monitored_fs_set(connection_socket);

    /*This is the main loop for handling connections. */
    /*All server process usually run 24x7. Good server should always up and running and should never go down. */

    for(;;)
    {
        /*Copy the entire monitored FDs to readfds*/
        refresh_fd_set(&readfds);

        /*Waiting for incoming connection. */
        printf("Waiting on select() sys call\n");

        /*Call the select system call, server process blocks here. Linux OS keeps this process blocked until connection initiation request or data request
        arrives on any of the file descriptors in the 'readfds' set. */
        /*maximum numerical FD + 1 - Highest numbered file descriptor plus 1, file descriptor */
        select(get_max_fd()+1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(connection_socket, &readfds))
        {
            /*Data arrives on Master socket only when new client connects with the server (thau is, 'connect' call is invoked on client side)*/
            printf("New connection received recv, accept the connection. \n");

            data_socket = accept(connection_socket, NULL, NULL);

            if (data_socket == -1)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Connection accepted from client\n");

            add_to_monitored_fs_set (data_socket);
        }
        else
        {
            /*Data arrives on some other client FD*/

            /*Find the client which has send us the data request*/
            comm_socket_fd = -1;

            for (i =0; i < MAX_CLIENT_SUPPORTED; i++)
            {
                if (FD_ISSET(monitored_fd_set[i], &readfds))
                {
                    comm_socket_fd = monitored_fd_set[i];

                    /*Prepare the buffer to recv the data*/
                    memset(buffer, 0, BUFFER_SIZE);

                    /*Wait for next data package. Server is blocked here. Waiting for the data to arrive from client
                        'read' is a blocking system call*/
                    printf("Waiting for data from the client\n");

                    ret = read(comm_socket_fd, buffer, BUFFER_SIZE);

                    if (ret == -1)
                    {
                        perror("Read");
                        exit(EXIT_FAILURE);
                    }

                    /*Add received summand. */
                    memcpy(&data, buffer, sizeof(int));

                    if (data == 0)
                    {
                        /*Send result*/
                        memset(buffer,0,BUFFER_SIZE);
                        sprintf(buffer,"Result = %d",client_result[i]);
                        
                        printf("Sending final result back to client\n");
                        ret = write(comm_socket_fd, buffer, BUFFER_SIZE);

                        if (ret == -1)
                        {
                            perror("Write");
                            exit(EXIT_FAILURE);
                        }

                        /*Close socket. */
                        close(comm_socket_fd);
                        client_result[i] = 0;
                        remove_from_monitored_fd_set(comm_socket_fd);
                        continue; /*go to select() and block*/
                    }
                    client_result[i] += data;
                }
            }
        }
    } /*Go to select() and block*/
    close(connection_socket);
    remove_from_monitored_fd_set(connection_socket);
    printf("Connection closed. \n");

    /*Server should release resource before getting terminated. Unlink the socket. */
    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}