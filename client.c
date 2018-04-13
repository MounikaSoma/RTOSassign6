#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
int main()
{
char msg[1024];
int k;
int socket_desc;
struct sockaddr_in client;
memset(&client,0,sizeof(client));
socket_desc=socket(AF_INET,SOCK_STREAM,0);
if(socket_desc==-1)
{
printf("error in socket creation");
exit(1);
}
client.sin_family=AF_INET;
client.sin_addr.s_addr=INADDR_ANY;
client.sin_port=3002;
k=connect(socket_desc,(struct sockaddr*)&client,sizeof(client));
if(k==-1)
{
printf("Error in connecting to server");
exit(1);
}
while(1)
{
fgets(msg,sizeof msg,stdin);
if(strncmp(msg,"end",3)==0)//Use “end” to end communication with server
break;
k=send(socket_desc,msg,100,0);
if(k==-1)
{
printf("Error in sending");
exit(1);
}
k=recv(socket_desc,msg,100,0);
if(k==-1)
{
printf("Error in receiving");
exit(1);
}
printf(" %s",msg);

}
close(socket_desc);
exit(0);
return 0;
}
