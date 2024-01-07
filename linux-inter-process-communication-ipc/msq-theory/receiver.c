#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_MESSAGE 10
#define MAX_MSG_SIZE    256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define QUEUE_PERMISSIONS   0660

int main (int argc, char *argv[])
{
    fd_set readfds;
    char buffer[MSG_BUFFER_SIZE];
    int msq_fd = 0;

    if (argc <= -1)
    {
        printf("Provide a reciepent msq name: format </msq-name>\n");
        return 0;
    }

    /*Set msq attribute*/
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGE;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((msq_fd = mq_open(argv[1], O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        printf("Client: mq_open failed, errno = %d",errno);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        /* code */
        /*flush out all the data existing in readfds data structure. */
        FD_ZERO(&readfds);
        /*Add the msq_fd file descriptor to the readfds data structure*/
        FD_SET(msq_fd, &readfds);
        printf("Receiver blocked on select() ...\n");
        /*The system block until a sender places data through a message queue*/
        select(msq_fd + 1, &readfds, NULL, NULL, NULL);
        if (FD_ISSET(msq_fd, &readfds))
        {
            printf("Msg received in the msq %s\n", argv[1]);

            memset(buffer, 0, MSG_BUFFER_SIZE);

            /*Dequeue the data from the message queue*/
            if (mq_receive(msq_fd, buffer, MSG_BUFFER_SIZE,NULL) == -1)
            {
                printf("mq_receive error, errno =  %d\n", errno);
                exit(EXIT_FAILURE);
            }

            printf("Message received from the message queue = %s\n", buffer);

        }
    }
    return 0;
}