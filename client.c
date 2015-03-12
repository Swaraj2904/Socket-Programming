#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int totalc = 0;/* Total computed by client */
    int totals;/* Total computed by server */
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    /* Creating Socket */
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* Connecting to Server */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    /* Sending request */
    printf("Enter the transaction: ");
    bzero(buffer_out, 256);
    fgets(buffer_out, 255, stdin);
    n = write(sockfd, buffer_out, strlen(buffer_out));
    if (n < 0)
         error("ERROR writing to socket");

    /* Recieving response */
    bzero(buffer_in,256);
    n = read(sockfd, buffer_in, 255);
    if (n < 0)
         error("ERROR reading from socket");

    /* Analyzing Response */
    if(buffer_in[0] == '1')
    {
        if(buffer_in[1] == '0')
            error("Protocol Error");
        else
            printf("UPC not found in database");
    }
    if(buffer_out[0] == '0')
    {
        strncpy(price, buffer_in + 1, 4);
        strncpy(name, buffer_in + 5, strlen(buffer_in) - 5);
        strncpy(quantity, buffer_out + 4, 4);
        totalc += atoi(price) * atoi(quantity);
        printf("\n%s  %s  %s %s %d",upc_code, name, price, num, total);
    }
    else
    {
        strncpy(total, buffer_in + 1, 10);
        totals = atoi(total);
        if(totalc != totals)
            error("Error in computation");
        else
            printf("\nThe total is %d", totalc);
    }
    //printf("%s\n", buffer);
    close(sockfd);
    return 0;
}
