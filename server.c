/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
   int sockfd, newsockfd, portno;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   if (argc < 2) {
      fprintf(stderr,"ERROR, no port provided\n");
      exit(1);
   }
   
   /* Creating socket for server */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
      error("ERROR opening socket");
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
      error("ERROR on binding");
      
   /* Listening to specified port */         
   listen(sockfd,5);
   
   /* accepting client request */
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
   if (newsockfd < 0) 
      error("ERROR on accept");
      
   /* reading data from client */
   bzero(buffer,256);
   n = read(newsockfd,buffer,255);
   if (n < 0) 
      error("ERROR reading from socket");
      
   /* analyzing data */   
   if(buffer[0] == '1')
   {
      fclose(fp);
      item = "0" + total;
      n = write(newsockfd,item,10);
      if (n < 0) 
         error("ERROR writing to socket");
      close(newsockfd);
   }
   else
   {
      int present = 0;
      char buffer[256];
      char upc_code[3];
      char num[4];
      char name[30];
      char price[4];
      int i = 0;
      strncpy(upc_code, buffer+1, 3);
      strncpy(num, buffer+4, 4);
      FILE *fp = fopen("data.txt","w+");
      /* Searching the database for the upc code */
      while(!feof(fp))
      {
         if (fread(buffer, 3, 1, fp) != 1)
            error("Error in read");
         if(buffer == upc_code)
            present = 1;
         fseek(fp, (i+1)*35, SEEK_SET);
      }
      if(!present)
      {
         //printf("UPC is not in database");
         n = write(newsockfd,"11",2);
         if (n < 0) 
            error("ERROR writing to socket");
      }
      if (fread(price, 4, 1, fp) != 1)
         error("Error in read");
      name = extract from file;
      total = price * atoi(num);
      item = "0" + price + name; 
      n = write(newsockfd,item,35);
      if (n < 0) 
         error("ERROR writing to socket");
   }
   /*printf("Here is the message: %s\n",buffer);
   n = write(newsockfd,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
   */
   close(sockfd);
   return 0; 
}
