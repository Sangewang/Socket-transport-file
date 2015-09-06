#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main(int argc,char **argv)
{
  if(argc!=2)
  {
    printf("Usage:./%s ServerIPAddress\n",argv[0]);
    exit(1);
  }
  while(1)
  {
   struct sockaddr_in client_addr;
   memset(&client_addr,0,sizeof(client_addr));
   client_addr.sin_family=AF_INET;
   inet_pton(AF_INET,"0.0.0.0",&(client_addr.sin_addr));
   client_addr.sin_port = htons(0);
   int client_socket=socket(AF_INET,SOCK_STREAM,0);
   if(client_socket < 0)
   {
    printf("Create Socket Failed!\n");
    exit(1);
   }
   if(bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
   {
     printf("Client Bind Port Failed!\n");
     exit(1);
   }
   struct sockaddr_in server_addr;
   memset(&server_addr,0,sizeof(server_addr));
   server_addr.sin_family=AF_INET;
   if(inet_pton(AF_INET,argv[1],&server_addr.sin_addr)==0)
   {
    printf("Server IP Address Error!\n");
    exit(1);
  }

  server_addr.sin_port = htons(SERVER_PORT);
  socklen_t server_addr_length = sizeof(server_addr);
  if(connect(client_socket,(struct sockaddr*)&server_addr,server_addr_length)<0)
  {
    printf("Can Not Connect To %s!\n",argv[1]);
    exit(1);
  }
 
    char file_name[FILE_NAME_MAX_SIZE+1];
    memset(file_name,0,FILE_NAME_MAX_SIZE+1);
    printf("Please Input File Name On Server:\t");
    scanf("%s",file_name);

    char buffer[BUFFER_SIZE];
    memset(buffer,0,BUFFER_SIZE);
    strncpy(buffer,file_name,strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));
    send(client_socket,buffer,BUFFER_SIZE,0);
    FILE * fp = fopen(file_name,"w");
    if(NULL==fp)
    {
      printf("File:\t%s Can Not Open To Write\n",file_name);
      exit(1);
    }
    memset(buffer,0,BUFFER_SIZE);
    int length=0;
    while(length = recv(client_socket,buffer,BUFFER_SIZE,0))
    {
     // printf("if match judge!!\n");
     // printf("%s\n",buffer);
     // if(buffer=="")
     // {
       // printf("No match file,Please confirm!");
       // break;
     // }
      if(length < 0)
      {
        printf("Recieve Data From Server %s Failed!\n",argv[1]);
        break;
      }
      int write_length = fwrite(buffer,sizeof(char),length,fp);
      if(write_length < length)
      {
        printf("File:\t%s Write Failed\n",file_name);
        break;
      }
       memset(buffer,0,BUFFER_SIZE);
     }
     printf("Recieve File:\t %s From Server[%s] Finished\n",file_name,argv[1]);
     fclose(fp);
     close(client_socket);
  }
  return 0;
  
}



























