/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
//#include <fstream.h>

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
   char *buff;
   FILE *fp;
   int present = 0;
 
   char upc_code[3];
   char num[4];
   char name[30];
   char price[4];
   char item[35]; 
   int i = 0;
   int total;
   char total_s[10];
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
   while(1)
   {
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
      item[0] = '0';
      bzero(total_s,10);
      //itoa(total,total_s,10);
      n = write(newsockfd,item,35);
      if (n < 0) 
         error("ERROR writing to socket");
      close(newsockfd);
      close(sockfd);
   }
   else
   {
      
      strncpy(upc_code, buffer+1, 3);
      strncpy(num, buffer+4, 4);
      fp = fopen("data.txt","r");
      /* Searching the database for the upc code */
      while(!feof(fp))
      {
         buff = (char*)malloc(3*sizeof(char));
         if (fread(buff, 3, 1, fp) != 1)
            error("ERROR in read1");
         if( strcmp(buff,upc_code) == 0 )
         {
            present = 1;
            break;
         }
         i++;
         fseek(fp, i*40, SEEK_SET);
         
      }
      if(present == 0)
      {
         //printf("UPC is not in database");
         n = write(newsockfd,"1UPC is not found in database",27);
         if (n < 0) 
            error("ERROR writing to socket");
      }
      else
      {
      fseek(fp, i*40 + 5, SEEK_SET);
      if (fread(price, 4, 1, fp) != 1)
         error("ERROR in read2");
      fseek(fp, i*40 + 11, SEEK_SET);
      if (fread(name, 30, 1, fp) != 1)
         error("ERROR in read3");
      total += atoi(price) * atoi(num);
      
      
      
      item[0] = '0';
      n = write(newsockfd, item, 35);
      if (n < 0) 
         error("ERROR writing to socket");
      }
   }
   /*printf("Here is the message: %s\n",buffer);
   n = write(newsockfd,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
   */
   }
   return 0; 
}
