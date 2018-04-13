#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
int main()
{
char msg[1024];
int k;
socklen_t len;
int socket_desc,temp_socket_desc;
struct sockaddr_in server,client;
memset(&server,0,sizeof(server));
memset(&client,0,sizeof(client));
socket_desc=socket(AF_INET,SOCK_STREAM,0);
if(socket_desc==-1)
{
printf("Error in socket creation");
exit(1);
}
server.sin_family=AF_INET;
//giving the loop back address here
server.sin_addr.s_addr=inet_addr("127.0.0.1");
server.sin_port=3002;
k=bind(socket_desc,(struct sockaddr*)&server,sizeof(server));
if(k==-1){
printf("Error in binding");
exit(1);
}
k=listen(socket_desc,20);
if(k==-1)
{
printf("Error in listening");
exit(1);
}
len=sizeof(client);

temp_socket_desc=accept(socket_desc,(struct sockaddr*)&client,&len);
if(temp_socket_desc==-1)
{
printf("Error in temporary socket creation");
exit(1);
}
while(1)
{
k=recv(temp_socket_desc,msg,100,0);
if(k==-1)
{
printf("Error in receiving");
exit(1);
}
printf(" %s",msg);
//printf("\nEnter data to be send to client: ");
fgets(msg,sizeof msg,stdin);
if(strncmp(msg,"end",3)==0)
break;
k=send(temp_socket_desc,msg,100,0);
if(k==-1)
{
printf("Error in sending");
exit(1);
}
}
close(temp_socket_desc);
exit(0);
return 0;
}
