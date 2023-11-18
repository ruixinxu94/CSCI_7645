//#include <stdio.h>
//#include <stdlib.h>
//#include "mqueue.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>  /* For O_* constants */
//#include <sys/stat.h>  /* For mode constants */
//#include <string.h>
//
//int main() {
//    mqd_t mq;
//    struct mq_attr attr;
//    char *message = "Hello, Message Queue!";
//    char buffer[1024];
////    unsigned int priority = 1;
//
//    // Set the attributes for the message queue
//    attr.mq_flags = 0;
//    attr.mq_maxmsg = 10;
//    attr.mq_msgsize = 1024;
//    attr.mq_curmsgs = 0;
//
//    // Create a message queue
//    mq = mq_open("/my_message_queue", O_CREAT | O_RDWR, 0644, &attr);
//    if (mq == (mqd_t)-1) {
//        perror("mq_open");
//        exit(1);
//    }
//
//    // Send a message to the queue
//    if (mq_send(mq, message, strlen(message) + 1, priority) == -1) {
//        perror("mq_send");
//        exit(1);
//    }
//
//    printf("Message sent: %s\n", message);
//
//    // Receive a message from the queue
//    if (mq_receive(mq, buffer, 1024, &priority) == -1) {
//        perror("mq_receive");
//        exit(1);
//    }
//
//    printf("Message received: %s\n", buffer);
//
//    // Close and remove the message queue
//    if (mq_close(mq) == -1) {
//        perror("mq_close");
//        exit(1);
//    }
//
//    if (mq_unlink("/my_message_queue") == -1) {
//        perror("mq_unlink");
//        exit(1);
//    }
//
//    return 0;
//}
//
//
